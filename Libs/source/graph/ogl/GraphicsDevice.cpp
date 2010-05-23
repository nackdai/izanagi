#include "graph/dx9/GraphicsDevice.h"
#include "graph/dx9/BaseTexture.h"
#include "graph/dx9/Texture.h"
#include "graph/dx9/CubeTexture.h"
#include "graph/dx9/Surface.h"
#include "graph/dx9/VertexBuffer.h"
#include "graph/dx9/IndexBuffer.h"
#include "graph/dx9/VertexShader.h"
#include "graph/dx9/PixelShader.h"
#include "graph/dx9/VertexDeclaration.h"
#include "graph/GraphDefs.h"

#include "graph/2d/2DRenderer.h"
#include "std/MemoryAllocator.h"

using namespace uranus;

CMemoryAllocator CGraphicsDevice::s_cDeviceAllocator;
CGraphicsDevice* CGraphicsDevice::s_pInstance = UN_NULL;

/**
* �C���X�^���X�쐬
*/
CGraphicsDevice* CGraphicsDevice::CreateGrapicsDevice(
	UN_UINT nBufSize,
	void* pDeviceBuffer)
{
	if (s_pInstance != UN_NULL) {
		// �쐬�ς݂Ȃ̂ŉ������Ȃ�
		return s_pInstance;
	}

	// �A���P�[�^������
	if (!s_cDeviceAllocator.Init(nBufSize, (UN_UINT8*)pDeviceBuffer)) {
		UN_ASSERT(UN_FALSE);
		return UN_NULL;
	}

	IMemoryAllocator* pAllocator = &s_cDeviceAllocator;

	UN_BOOL result = UN_TRUE;
	UN_UINT8* pBuf = UN_NULL;
	CGraphicsDevice* pInstance = UN_NULL;

	// �������m��
	pBuf = (UN_UINT8*)ALLOC_ZERO(pAllocator, sizeof(CGraphicsDevice));
	if (!(result = (pBuf != UN_NULL))) {
		UN_ASSERT(UN_FALSE);
		goto __EXIT__;
	}

	// �C���X�^���X�쐬
	pInstance = new(pBuf) CGraphicsDevice;
	{
		pInstance->m_pAllocator = pAllocator;

		// IDirect3D9 �C���^�[�t�F�[�X�̎擾
		pInstance->m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
		result = (pInstance->m_pD3D != UN_NULL);
		UN_ASSERT(result);

		// ���݂̉�ʃ��[�h�̎擾
		if (result) {
			HRESULT hr = pInstance->m_pD3D->GetAdapterDisplayMode(
							D3DADAPTER_DEFAULT,
							&pInstance->m_sDisplayMode);
			result = SUCCEEDED(hr);
			UN_ASSERT(result);
		}

		pInstance->AddRef();
	}

	if (!result) {
		goto __EXIT__;
	}

	s_pInstance = pInstance;
	
__EXIT__:
	if (!result) {
		if (pInstance != UN_NULL) {
			SAFE_RELEASE(pInstance);
		}
		else if (pBuf != UN_NULL) {
			pAllocator->Free(pBuf);
		}
	}

	return pInstance;
}

// �R���X�g���N�^
CGraphicsDevice::CGraphicsDevice()
{
	m_pAllocator = UN_NULL;

	m_pD3D = UN_NULL;

	m_pDevice = UN_NULL;
	ZeroMemory(&m_sPresentParameters, sizeof(m_sPresentParameters));

	m_hFocusWindow = UN_NULL;

	ZeroMemory(m_pTexture, sizeof(CTexture*) * TEX_STAGE_NUM);
	ZeroMemory(m_sSamplerState, sizeof(S_SAMPLER_STATE) * TEX_STAGE_NUM);

	ZeroMemory(&m_RenderState, sizeof(S_RENDER_STATE));

	m_p2DRenderer = UN_NULL;

	m_pResetTexture = UN_NULL;
	m_pResetVB = UN_NULL;
	m_pResetIB = UN_NULL;

	m_Flags.is_call_begin = UN_FALSE;
	m_Flags.is_render_2d = UN_FALSE;
	m_Flags.is_force_set_state = UN_FALSE;
	m_Flags.is_lost_device = UN_FALSE;

	m_pResetCallBack = UN_NULL;
	m_pLostDeviceCallBack = UN_NULL;
}

// �f�X�g���N�^
CGraphicsDevice::~CGraphicsDevice()
{
	SAFE_RELEASE(m_pRT);
	SAFE_RELEASE(m_pDepth);

	SAFE_RELEASE(m_p2DRenderer);

	ClearRenderState();

	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_pD3D);
}

// ���
void CGraphicsDevice::InternalRelease()
{	
	delete this;

	if (m_pAllocator != UN_NULL) {
		m_pAllocator->Free(this);
	}

	if (s_pInstance == this) {
		s_pInstance = UN_NULL;
	}
}

/**
* ���Z�b�g
*/
UN_BOOL CGraphicsDevice::Reset(const SGraphicsDeviceInitParams& sParams)
{
	UN_BOOL ret = UN_TRUE;

	if (m_pDevice == UN_NULL) {
		// �{�̍쐬
		ret = CreateBody(sParams);

		if (ret) {
			// 2D�`�揉����
			m_p2DRenderer = C2DRenderer::Create2DRenderer(
								this,
								m_pAllocator);
			ret = (m_p2DRenderer != UN_NULL);
			UN_ASSERT(ret);
		}
	}
	else {
		// ���Z�b�g
		ret = ResetInternal(sParams);
	}

	if (ret) {
		// �r���[�|�[�g
		SViewport vp;
		{
			vp.x = 0;
			vp.y = 0;
			vp.width = m_sPresentParameters.BackBufferWidth;
			vp.height = m_sPresentParameters.BackBufferHeight;
			vp.minZ = 0.0f;
			vp.maxZ = 1.0f;
		}
		SetViewport(vp);

		// �����_�[�X�e�[�g�̌��ݒl���擾
		m_RenderState.GetParamsFromGraphicsDevice(this);

		// �X�e�[�g
		// �����ݒ�
		m_Flags.is_force_set_state = UN_TRUE;
		SetDefaultRenderState();
		m_Flags.is_force_set_state = UN_FALSE;

		// �T�[�t�F�X�̃��Z�b�g
		//�i�t���[���o�b�t�@�̃��Z�b�g�j
		{
			if (m_pRT != UN_NULL) {
				m_pRT->Reset(UN_NULL, 0);
			}
			else {
				m_pRT = CSurface::CreateSurface(m_pAllocator);
			}

			if (m_pDepth != UN_NULL) {
				m_pDepth->Reset(UN_NULL, 0);
			}
			else {
				m_pDepth = CSurface::CreateSurface(m_pAllocator);
			}

			ret = ((m_pRT != UN_NULL) && (m_pDepth != UN_NULL));
			UN_ASSERT(ret);
			
			if (ret) {
				// �t���[���o�b�t�@�T�[�t�F�[�X������Ă���E�E�E
				m_pDevice->GetRenderTarget(0, &m_pRT->m_pSurface);
				m_pDevice->GetDepthStencilSurface(&m_pDepth->m_pSurface);

				// �����I�ɋL�q���Z�b�g����
				m_pRT->SetDesc();
				m_pDepth->SetDesc();

				// ���݂̃����_�[�^�[�Q�b�g�Ƃ��ĕێ�
				SAFE_REPLACE(m_RenderState.curRT[0], m_pRT);
				SAFE_REPLACE(m_RenderState.curDepth, m_pDepth);
			}
		}
	}

	return ret;
}

// �{�̍쐬
UN_BOOL CGraphicsDevice::CreateBody(const SGraphicsDeviceInitParams& sParams)
{
	UN_ASSERT(m_pD3D != UN_NULL);

	UN_BOOL ret = UN_TRUE;

	m_hFocusWindow = sParams.hFocusWindow;

	// D3DPRESENT_PARAMETERS �̐ݒ�
	{
		ZeroMemory(&m_sPresentParameters, sizeof(m_sPresentParameters));

		m_sPresentParameters.BackBufferCount = 2;

		m_sPresentParameters.Windowed = sParams.Windowed;					// ��ʃ��[�h(�E�C���h�E���[�h)
		m_sPresentParameters.BackBufferWidth = sParams.BackBufferWidth;		// �o�b�N�o�b�t�@�̕�
		m_sPresentParameters.BackBufferHeight = sParams.BackBufferHeight;	// �o�b�N�o�b�t�@�̍���

		// �o�b�N�o�b�t�@�̃t�H�[�}�b�g
		m_sPresentParameters.BackBufferFormat = m_sDisplayMode.Format;

		m_sPresentParameters.MultiSampleType = sParams.MultiSampleType;		// �}���`�E�T���v�����O�̎��

		// �X���b�v�G�t�F�N�g�̎��
		m_sPresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;

		m_sPresentParameters.hDeviceWindow = sParams.hDeviceWindow;

#if 1
		// Z�o�b�t�@�̎����쐬
		m_sPresentParameters.EnableAutoDepthStencil = UN_TRUE;
		//m_sPresentParameters.AutoDepthStencilFormat = D3DFMT_D24S8;
		m_sPresentParameters.AutoDepthStencilFormat = sParams.DepthStencilFormat;
#else
		// Z�o�b�t�@�������쐬���Ȃ�
		m_sPresentParameters.EnableAutoDepthStencil = UN_FALSE;
#endif

		// VSync�̃^�C�v
		m_sPresentParameters.PresentationInterval = sParams.PresentationInterval;
	}

	if (FAILED(m_pD3D->CreateDevice(
				sParams.Adapter, 
				sParams.DeviceType,
				sParams.hFocusWindow,
				sParams.BehaviorFlags,
				&m_sPresentParameters,
				&m_pDevice)))
	{
		// ���ǎ��s����
		ret = UN_FALSE;
    }

	return ret;
}

template <class _T>
void CGraphicsDevice::ReleaseResource(_T* pList)
{
	for (_T* p = pList; p != UN_NULL; p = p->m_pNext) {
		p->ReleaseResource();
	}
}

template <class _T>
void CGraphicsDevice::ResetResource(_T* pList)
{
	for (_T* p = pList; p != UN_NULL; p = p->m_pNext) {
		p->Reset();
	}
}

UN_BOOL CGraphicsDevice::ResetInternal(const SGraphicsDeviceInitParams& sParams)
{
	// ���\�[�X���������
	{
		m_pRT->ReleaseResource();
		m_pDepth->ReleaseResource();

		m_p2DRenderer->ResetResource();

		ReleaseResource(m_pResetTexture);
		ReleaseResource(m_pResetVB);
		ReleaseResource(m_pResetIB);
	}

	// �N���A�������̂�ۑ����Ă���
	UN_UINT nBackBufferCount = m_sPresentParameters.BackBufferCount;
	D3DFORMAT fmt = m_sPresentParameters.BackBufferFormat;

	// �V�����ݒ肳������
	m_sPresentParameters.BackBufferWidth = sParams.BackBufferWidth;
	m_sPresentParameters.BackBufferHeight = sParams.BackBufferHeight;

	HRESULT hr = m_pDevice->Reset(&m_sPresentParameters);
	UN_ASSERT(hr != D3DERR_DRIVERINTERNALERROR);

	UN_BOOL ret = SUCCEEDED(hr);

	if (ret) {
		// �N���A���ꂽ�̂Őݒ�l�ɕύX����
		m_sPresentParameters.BackBufferCount = nBackBufferCount;
		m_sPresentParameters.BackBufferFormat = fmt;

		// ���\�[�X�����Z�b�g����
		{
			m_pRT->Reset(UN_NULL, 0);
			m_pDepth->Reset(UN_NULL, 0);

			m_p2DRenderer->Reset();

			ResetResource(m_pResetTexture);
			ResetResource(m_pResetVB);
			ResetResource(m_pResetIB);
		}

		// �R�[���o�b�N
		if (m_pResetCallBack != UN_NULL) {
			ret = (*m_pResetCallBack)();
			UN_ASSERT(ret);
		}
	}

	return ret;
}

/**
* �`��J�n
*/
UN_BOOL CGraphicsDevice::BeginRender(
	UN_DWORD nClearFlags,
	UN_COLOR nClearColor,
	UN_FLOAT fClearZ,
	UN_DWORD nClearStencil)
{
	UN_ASSERT(m_pDevice != NULL);

	UN_BOOL ret = UN_FALSE;

	if (!m_Flags.is_lost_device) {
		// �f�o�C�X���X�g���ĂȂ��̂ŁA�ʏ�̕`��J�n�������s��
		m_pDevice->BeginScene();
		m_Flags.is_call_begin = UN_TRUE;

		Clear(
			nClearFlags,
			nClearColor,
			fClearZ,
			nClearStencil);

		// �t���[���o�b�t�@�T�[�t�F�X�����݂̃T�[�t�F�X�Ƃ��ăZ�b�g
		SAFE_REPLACE(m_RenderState.curRT[0], m_pRT);
		SAFE_REPLACE(m_RenderState.curDepth, m_pDepth);

		// 2D�����J�n
		m_p2DRenderer->BeginFrame();

		ret = UN_TRUE;
	}

	return ret;
}

/**
* �`��I��
*/
void CGraphicsDevice::EndRender()
{
	// �O�̂���
	EndScene();

	if (m_Flags.is_call_begin) {
		m_pDevice->EndScene();
	}

	m_Flags.is_call_begin = UN_FALSE;

	// �N���A���Ă݂�
	ClearRenderState();
}

void CGraphicsDevice::ClearRenderState()
{
#if 0
	SAFE_RELEASE(m_RenderState.curVB);
	SAFE_RELEASE(m_RenderState.curIB);
	SAFE_RELEASE(m_RenderState.curVS);
	SAFE_RELEASE(m_RenderState.curPS);
	SAFE_RELEASE(m_RenderState.curVD);
#else
	SetVertexShader(UN_NULL);
	SetPixelShader(UN_NULL);
	SetVertexBuffer(0, 0, 0, UN_NULL);
	SetIndexBuffer(UN_NULL);

	SAFE_RELEASE(m_RenderState.curVD);
	SetFVF(0);
#endif

	for (UN_UINT i = 0; i < MAX_MRT_NUM; i++) {
		// �`�F�b�N
		UN_ASSERT(m_cRTMgr[i].IsEmpty());

		SAFE_RELEASE(m_RenderState.curRT[i]);
	}

	// �`�F�b�N
	UN_ASSERT(m_cDepthMgr.IsEmpty());

	SAFE_RELEASE(m_RenderState.curDepth);

	for (UN_UINT i = 0; i < TEX_STAGE_NUM; i++) {
		SetTexture(i, UN_NULL);
	}
}

#ifndef _CONV_CLEAR_FLAG
#define _CONV_CLEAR_FLAG(flag, mask, dst)	((flag) & (mask) ? (dst) : 0)
#else
UN_C_ASSERT(UN_FALSE);
#endif

/**
* �N���A
*/
void CGraphicsDevice::Clear(
	UN_DWORD nClearFlags,
	UN_COLOR nClearColor,
	UN_FLOAT fClearZ,
	UN_DWORD nClearStencil)
{
	UN_ASSERT(m_pDevice != NULL);

	if (nClearFlags > 0) {
		// DirectX �̃N���A�t���O�ɕϊ�����
		UN_DWORD nDXClearFlag = 0;
		nDXClearFlag |= _CONV_CLEAR_FLAG(nClearFlags, E_GRAPH_CLEAR_FLAG_COLOR,   D3DCLEAR_TARGET);
		nDXClearFlag |= _CONV_CLEAR_FLAG(nClearFlags, E_GRAPH_CLEAR_FLAG_DEPTH,   D3DCLEAR_ZBUFFER);
		nDXClearFlag |= _CONV_CLEAR_FLAG(nClearFlags, E_GRAPH_CLEAR_FLAG_STENCIL, D3DCLEAR_STENCIL);

		m_pDevice->Clear(
			0,
			NULL,
			nDXClearFlag,
			nClearColor,
			fClearZ,
			nClearStencil);
	}
}

/**
* �V�[���`��J�n
*/
UN_BOOL CGraphicsDevice::BeginScene(
	UN_DWORD nClearFlags,
	UN_COLOR nClearColor/*= 0*/,
	UN_FLOAT fClearZ/*= 1.0f*/,
	UN_DWORD nClearStencil/*= 0*/)
{
	return BeginScene(
			UN_NULL, 0,
			UN_NULL,
			nClearFlags,
			nClearColor,
			fClearZ,
			nClearStencil);
}

UN_BOOL CGraphicsDevice::BeginScene(
	CSurface** pRT,
	UN_UINT nCount,
	UN_DWORD nClearFlags,
	UN_COLOR nClearColor/*= 0*/,
	UN_FLOAT fClearZ/*= 1.0f*/,
	UN_DWORD nClearStencil/*= 0*/)
{
	return BeginScene(
			pRT, nCount,
			UN_NULL,
			nClearFlags,
			nClearColor,
			fClearZ,
			nClearStencil);
}

UN_BOOL CGraphicsDevice::BeginScene(
	CSurface** pRT,
	UN_UINT nCount,
	CSurface* pDepth,
	UN_DWORD nClearFlags,
	UN_COLOR nClearColor/*= 0*/,
	UN_FLOAT fClearZ/*= 1.0f*/,
	UN_DWORD nClearStencil/*= 0*/)
{
	UN_ASSERT(m_pDevice != NULL);

	// TODO
	// MRT�͖����ŁE�E�E
	UN_ASSERT(nCount <= 1);

	UN_BOOL ret = UN_TRUE;

	if ((nCount > 0) && (pRT != UN_NULL)) {
		// �����_�[�^�[�Q�b�g�Z�b�g
		PushRenderTarget(pRT, nCount);
	}
	if (pDepth != UN_NULL) {
		// �[�x�E�X�e���V���Z�b�g
		PushDepthStencil(pDepth);
	}

	// �N���A
	Clear(
		nClearFlags,
		nClearColor,
		fClearZ,
		nClearStencil);

	return ret;
}

/**
* �V�[���`��I��
*/
void CGraphicsDevice::EndScene(UN_UINT flag/* = 0xffffffff*/)
{
	CSurface* pRTList[MAX_MRT_NUM];
	memset(pRTList, 0, sizeof(pRTList));

	UN_UINT nRTNum = 0;

	// �����_�[�^�[�Q�b�g
	for (UN_UINT i = 0; i < MAX_MRT_NUM; i++) {
		if ((flag & (1 << i)) > 0) {
			pRTList[i] = m_cRTMgr[i].Pop();
			nRTNum = (pRTList[i] != UN_NULL ? nRTNum + 1 : nRTNum);
		}
	}

	if (nRTNum > 0) {
		SetRenderTarget(pRTList, nRTNum);
	}

	if ((flag & E_GRAPH_END_SCENE_FLAG_DEPTH_STENCIL) > 0) {
		// �[�x�E�X�e���V��
		CSurface* pDepth = m_cDepthMgr.Pop();
		if (pDepth != UN_NULL) {
			SetDepthStencil(pDepth);
		}
	}
}

/**
* ����
*/
UN_BOOL CGraphicsDevice::Present()
{
	UN_BOOL ret = UN_TRUE;

	HRESULT hr = m_pDevice->Present(
					UN_NULL,
					UN_NULL,
					m_hFocusWindow,
					UN_NULL);

	if (hr == D3DERR_DEVICELOST) {
		// �f�o�C�X���X�g����
		m_Flags.is_lost_device = UN_TRUE;

		// �f�o�C�X���X�g�����Ƃ��̃R�[���o�b�N
		if (m_pLostDeviceCallBack != UN_NULL) {
			(*m_pLostDeviceCallBack)();
		}

		SGraphicsDeviceInitParams sParams;
		{
			sParams.BackBufferWidth = m_sPresentParameters.BackBufferWidth;
			sParams.BackBufferHeight = m_sPresentParameters.BackBufferHeight;
		}

		// ���A��������݂�
		ret = Reset(sParams);

		if (ret) {
			// ���A����
			m_Flags.is_lost_device = UN_FALSE;
		}
	}
	else {
		ret = SUCCEEDED(hr);
	}

	UN_ASSERT(ret);
	return ret;
}

/**
* 2D�`��J�n
*/
UN_BOOL CGraphicsDevice::Begin2D()
{
	UN_ASSERT(m_p2DRenderer != UN_NULL);

	// �`��J�n
	UN_BOOL ret = m_p2DRenderer->BeginDraw();

	if (ret) {
		// 2D�`��p�����_�[�X�e�[�g�ݒ�
		SaveRenderState();

		SetRenderState(E_GRAPH_RS_ZWRITEENABLE, UN_FALSE);
		SetRenderState(E_GRAPH_RS_ZENABLE, UN_FALSE);
		SetRenderState(E_GRAPH_RS_ALPHATESTENABLE, UN_FALSE);
		SetRenderState(E_GRAPH_RS_ALPHABLENDENABLE, UN_TRUE);
		SetRenderState(E_GRAPH_RS_BLENDMETHOD, E_GRAPH_ALPHA_BLEND_NORMAL);
		SetRenderState(E_GRAPH_RS_FILLMODE, D3DFILL_SOLID);
		SetRenderState(E_GRAPH_RS_CULLMODE, D3DCULL_CW);

		SetVertexShader(UN_NULL);
		SetPixelShader(UN_NULL);

		m_Flags.is_render_2d = UN_TRUE;
	}

	return ret;
}

/**
* 2D�`��I��
*/
UN_BOOL CGraphicsDevice::End2D()
{
	UN_BOOL ret = UN_TRUE;

	if (m_Flags.is_render_2d) {
		UN_ASSERT(m_p2DRenderer != UN_NULL);

		// �`��I��
		ret = m_p2DRenderer->EndDraw(this);

		// �����_�[�X�e�[�g�����ɖ߂�
		if (ret) {
			LoadRenderState();
		}
	}

	m_Flags.is_render_2d = UN_FALSE;

	return ret;
}

/**
* 2D�`��o�b�t�@�t���b�V��
*/
UN_BOOL CGraphicsDevice::Flush2D()
{
	UN_BOOL ret = m_p2DRenderer->Flush(this);
	return ret;
}

/**
* 2D�X�v���C�g�`��
*/
UN_BOOL CGraphicsDevice::Draw2DSprite(
	const CFloatRect& rcSrc,
	const CIntRect& rcDst,
	const UN_COLOR color/*= UN_COLOR_RGBA(255, 255, 255, 255)*/)
{
	UN_ASSERT(m_p2DRenderer != UN_NULL);
	UN_ASSERT(m_Flags.is_render_2d);

	UN_BOOL ret = UN_FALSE;

	if (m_Flags.is_render_2d) {
		ret = m_p2DRenderer->DrawSprite(
				this,
				rcSrc, rcDst,
				color);
	}

	return ret;
}

/**
* 2D�X�v���C�g�`��
*/
UN_BOOL CGraphicsDevice::Draw2DSpriteEx(
	const CIntRect& rcSrc,
	const CIntRect& rcDst,
	const UN_COLOR color/*= UN_COLOR_RGBA(255, 255, 255, 255)*/)
{
	UN_ASSERT(m_p2DRenderer != UN_NULL);
	UN_ASSERT(m_Flags.is_render_2d);

	UN_BOOL ret = UN_FALSE;

	if (m_Flags.is_render_2d) {
		ret = m_p2DRenderer->DrawSpriteEx(
				this,
				rcSrc, rcDst,
				color);
	}

	return ret;
}

/**
* 2D��`�`��
*/
UN_BOOL CGraphicsDevice::Draw2DRect(
	const CIntRect& rcDst,
	const UN_COLOR color)
{
	UN_ASSERT(m_p2DRenderer != UN_NULL);
	UN_ASSERT(m_Flags.is_render_2d);

	UN_BOOL ret = UN_FALSE;

	if (m_Flags.is_render_2d) {
		ret = m_p2DRenderer->DrawRect(
				this,
				rcDst,
				color);
	}

	return ret;
}

/**
* 2D���C���`��
*/
UN_BOOL CGraphicsDevice::Draw2DLine(
	const CIntPoint& ptStart,
	const CIntPoint& ptGoal,
	const UN_COLOR color)
{
	UN_ASSERT(m_p2DRenderer != UN_NULL);
	UN_ASSERT(m_Flags.is_render_2d);

	UN_BOOL ret = UN_FALSE;

	if (m_Flags.is_render_2d) {
		ret = m_p2DRenderer->DrawLine(
				this,
				ptStart, ptGoal,
				color);
	}

	return ret;
}

/**
* 2D�`�惂�[�h�Z�b�g
*/
void CGraphicsDevice::Set2DRenderOp(E_GRAPH_2D_RENDER_OP nOp)
{
	UN_ASSERT(m_p2DRenderer != UN_NULL);
	m_p2DRenderer->SetRenderOp(this, nOp);
}

/**
* 2D�`�惂�[�h�擾
*/
E_GRAPH_2D_RENDER_OP CGraphicsDevice::Get2DRenderOp() const
{
	UN_ASSERT(m_p2DRenderer != UN_NULL);
	return m_p2DRenderer->GetRenderOp();
}

/**
* �e�N�X�`���Z�b�g
*/
UN_BOOL CGraphicsDevice::SetTexture(UN_UINT nStage, CBaseTexture* pTex)
{
	UN_ASSERT(m_pDevice != NULL);

	if (m_pTexture[nStage] == pTex) {
		// ���łɃZ�b�g����Ă���
		return UN_TRUE;
	}
	else {
		// �قȂ�e�N�X�`��
		if (m_Flags.is_render_2d) {
			// 2D�`�撆�̏ꍇ
			// �o�b�t�@�ɗ��܂��Ă��镪��`�悵�Ă��܂�
			if (!m_p2DRenderer->Flush(this)) {
				UN_ASSERT(UN_FALSE);
				return UN_FALSE;
			}
		}
	}

	HRESULT hr = m_pDevice->SetTexture(
					nStage,
					pTex != NULL ? pTex->GetTexHandle() : NULL);
	VRETURN(SUCCEEDED(hr));

	// �ێ����Ă���
	m_pTexture[nStage] = pTex;

	// ���[��E�E�E
	// �X�e�[�g
	if (pTex != NULL) {
		// MIN_FILTER
		SetSamplerStateFilter(
			nStage,
			E_GRAPH_SAMPLER_STATE_TYPE_MINFILTER,
			m_sSamplerState[nStage].minFilter,
			pTex->GetState().minFilter);

		// MAG_FILTER
		SetSamplerStateFilter(
			nStage,
			E_GRAPH_SAMPLER_STATE_TYPE_MAGFILTER,
			m_sSamplerState[nStage].magFilter,
			pTex->GetState().magFilter);

		// MIP_FILTER
		if (pTex->GetMipMapNum() > 1) {
			SetSamplerStateFilter(
				nStage,
				E_GRAPH_SAMPLER_STATE_TYPE_MIPFILTER,
				m_sSamplerState[nStage].mipFilter,
				pTex->GetState().mipFilter);
		}

		// ADDRESS_U
		SetSamplerStateAddr(
			nStage,
			E_GRAPH_SAMPLER_STATE_TYPE_ADDRESSU,
			m_sSamplerState[nStage].addressU,
			pTex->GetState().addressU);

		// ADDRESS_V
		SetSamplerStateAddr(
			nStage,
			E_GRAPH_SAMPLER_STATE_TYPE_ADDRESSV,
			m_sSamplerState[nStage].addressV,
			pTex->GetState().addressV);
	}

	return UN_TRUE;
}

/**
* �e�N�X�`���擾
*/
CBaseTexture* CGraphicsDevice::GetTexture(UN_UINT nStage)
{
	UN_ASSERT(nStage < TEX_STAGE_NUM);
	return m_pTexture[nStage];
}

// �����_�[�^�[�Q�b�g�Z�b�g
UN_BOOL CGraphicsDevice::PushRenderTarget(CSurface** pSurface, UN_UINT num)
{
	UN_ASSERT(pSurface != UN_NULL);

	// TODO
	// MRT�͋����Ȃ�
	UN_ASSERT(num == 1);
	UN_ASSERT(num < MAX_MRT_NUM);

	UN_BOOL ret = UN_FALSE;

	// ���݂̃����_�[�^�[�Q�b�g���v�b�V��
	for (UN_UINT i = 0; i < num; i++) {
		if ((pSurface[i] != UN_NULL)
			&& (m_RenderState.curRT[i] != pSurface[i]))
		{
			ret = m_cRTMgr[i].Push(m_RenderState.curRT[i]);
			if (!ret) {
				break;
			}
		}
	}

	if (ret) {
		SetRenderTarget(pSurface, num);
	}

	return ret;
}

void CGraphicsDevice::SetRenderTarget(CSurface** pSurface, UN_UINT num)
{
	// �����_�[�^�[�Q�b�g�����ւ���
	for (UN_UINT i = 0; i < num; i++) {
		if (m_RenderState.curRT[i] != pSurface[i]) {
			m_pDevice->SetRenderTarget(i, pSurface[i]->GetRawInterface());
			SAFE_REPLACE(m_RenderState.curRT[i], pSurface[i]);
		}
	}

	// TODO
	// �����I�ɂO�Ԗڂ̃T�[�t�F�X�̃T�C�Y�Ƀr���[�|�[�g��ϊ�����
	SViewport vp;
	memcpy(&vp, &m_RenderState.vp, sizeof(vp));
	vp.x = vp.y = 0;
	vp.width = pSurface[0]->GetWidth();
	vp.height = pSurface[0]->GetHeight();
	SetViewport(vp);
}

// �[�x�E�X�e���V���Z�b�g
UN_BOOL CGraphicsDevice::PushDepthStencil(CSurface* pSurface)
{
	UN_BOOL ret = UN_FALSE;

	// ���݂̐[�x���v�b�V��
	if ((pSurface != UN_NULL)
		&& (m_RenderState.curDepth != pSurface))
	{
		ret = m_cDepthMgr.Push(m_RenderState.curDepth);
	}

	if (ret) {
		SetDepthStencil(pSurface);
	}

	return ret;
}

void CGraphicsDevice::SetDepthStencil(CSurface* pSurface)
{
	if (m_RenderState.curDepth != pSurface) {
		// �����_�[�^�[�Q�b�g�����ւ���
		m_pDevice->SetDepthStencilSurface(pSurface->GetRawInterface());
		SAFE_REPLACE(m_RenderState.curDepth, pSurface);
	}
}

/**
* ���_�o�b�t�@�Z�b�g
*/
UN_BOOL CGraphicsDevice::SetVertexBuffer(
	UN_UINT nStreamIdx,
	UN_UINT nOffsetByte,
	UN_UINT nStride,
	CVertexBuffer* pVB)
{
	if (m_RenderState.curVB == pVB) {
		// ���łɐݒ肳��Ă���
		return UN_TRUE;
	}

#if 0
	if (pVB != UN_NULL) {
		HRESULT hr = m_pDevice->SetStreamSource(
						nStreamIdx,
						pVB->GetRawInterface(),
						nOffsetByte,
						nStride);
#else
	{
		HRESULT hr = m_pDevice->SetStreamSource(
						nStreamIdx,
						pVB != UN_NULL ? pVB->GetRawInterface() : UN_NULL,
						nOffsetByte,
						nStride);
#endif
		VRETURN(SUCCEEDED(hr));

		// ���ݐݒ肳��Ă�����̂Ƃ��ĕێ�
		SAFE_REPLACE(m_RenderState.curVB, pVB);
	}

	return UN_TRUE;
}

/**
* FVF�Z�b�g
*/
UN_BOOL CGraphicsDevice::SetFVF(UN_DWORD dwFVF)
{
	if (m_RenderState.curFVF != dwFVF) {
		HRESULT hr = m_pDevice->SetFVF(dwFVF);
		VRETURN(SUCCEEDED(hr));

		m_RenderState.curFVF = dwFVF;
	}

	return UN_TRUE;
}

/**
* �C���f�b�N�X�o�b�t�@�Z�b�g
*/
UN_BOOL CGraphicsDevice::SetIndexBuffer(CIndexBuffer* pIB)
{
	if (m_RenderState.curIB == pIB) {
		// ���łɐݒ肳��Ă���
		return UN_TRUE;
	}

#if 0
	if (pIB != UN_NULL) {
		HRESULT hr = m_pDevice->SetIndices(pIB->GetRawInterface());
#else
	{
		HRESULT hr = m_pDevice->SetIndices(
						pIB != UN_NULL ? pIB->GetRawInterface() : UN_NULL);
#endif
		VRETURN(SUCCEEDED(hr));

		// ���ݐݒ肳��Ă�����̂Ƃ��ĕێ�
		SAFE_REPLACE(m_RenderState.curIB, pIB);
	}

	return UN_TRUE;
}

/**
* �C���f�b�N�X�o�b�t�@�`��
*/
UN_BOOL CGraphicsDevice::DrawIndexedPrimitive(
	E_GRAPH_PRIM_TYPE prim_type,
	UN_UINT nBaseIdx,
	UN_UINT nMinIdx,
	UN_UINT nVtxNum,
	UN_UINT nStartIdx,
	UN_UINT nPrimCnt)
{
	HRESULT hr = m_pDevice->DrawIndexedPrimitive(
					UN_GET_TARGET_PRIM_TYPE(prim_type),
					nBaseIdx,
					nMinIdx,
					nVtxNum,
					nStartIdx,
					nPrimCnt);
	VRETURN(SUCCEEDED(hr));

	return UN_TRUE;
}

/**
* �C���f�b�N�X�o�b�t�@�Ȃ��`��
*/
UN_BOOL CGraphicsDevice::DrawPrimitive(
	E_GRAPH_PRIM_TYPE prim_type,
	UN_UINT nStartIdx,
	UN_UINT nPrimCnt)
{
	HRESULT hr = m_pDevice->DrawPrimitive(
					UN_GET_TARGET_PRIM_TYPE(prim_type),
					nStartIdx,
					nPrimCnt);
	VRETURN(SUCCEEDED(hr));

	return UN_TRUE;
}

/**
* ���_�V�F�[�_�Z�b�g
*/
UN_BOOL CGraphicsDevice::SetVertexShader(CVertexShader* pVS)
{
	if (m_RenderState.curVS == pVS) {
		// ���łɐݒ肳��Ă���
		return UN_TRUE;
	}

#if 0
	if (pVS != UN_NULL) {
		HRESULT hr = m_pDevice->SetVertexShader(pVS->GetRawInterface());
#else
	{
		HRESULT hr = m_pDevice->SetVertexShader(
						pVS != UN_NULL ? pVS->GetRawInterface() : UN_NULL);
#endif
		VRETURN(SUCCEEDED(hr));

		// ���ݐݒ肳��Ă�����̂Ƃ��ĕێ�
		SAFE_REPLACE(m_RenderState.curVS, pVS);
	}

	return UN_TRUE;
}

/**
* �s�N�Z���V�F�[�_�Z�b�g
*/
UN_BOOL CGraphicsDevice::SetPixelShader(CPixelShader* pPS)
{
	if (m_RenderState.curPS == pPS) {
		// ���łɐݒ肳��Ă���
		return UN_TRUE;
	}

#if 0
	if (pPS != UN_NULL) {
		HRESULT hr = m_pDevice->SetPixelShader(pPS->GetRawInterface());
#else
	{
		HRESULT hr = m_pDevice->SetPixelShader(
						pPS != UN_NULL ? pPS->GetRawInterface() : UN_NULL);
#endif
		VRETURN(SUCCEEDED(hr));

		// ���ݐݒ肳��Ă�����̂Ƃ��ĕێ�
		SAFE_REPLACE(m_RenderState.curPS, pPS);
	}

	return UN_TRUE;
}

/**
* ���_�錾�Z�b�g
*/
UN_BOOL CGraphicsDevice::SetVertexDeclaration(CVertexDeclaration* pVD)
{
	if (m_RenderState.curVD == pVD) {
		// ���łɐݒ肳��Ă���
		return UN_TRUE;
	}

	if (pVD != UN_NULL) {
		HRESULT hr = m_pDevice->SetVertexDeclaration(pVD->GetRawInterface());
		VRETURN(SUCCEEDED(hr));

		// TODO
		// FVF�������N���A
		SetFVF(0);
	}

	// ���ݐݒ肳��Ă�����̂Ƃ��ĕێ�
	SAFE_REPLACE(m_RenderState.curVD, pVD);

	return UN_TRUE;
}

/**
* �r���[�|�[�g�Z�b�g
*/
UN_BOOL CGraphicsDevice::SetViewport(const SViewport& vp)
{
	UN_BOOL ret = UN_TRUE;
	HRESULT hr = S_OK;

	if ((m_RenderState.vp.width != vp.width)
		|| (m_RenderState.vp.height != vp.height)
		|| (m_RenderState.vp.x != vp.x)
		|| (m_RenderState.vp.y != vp.y)
		|| (m_RenderState.vp.minZ != vp.minZ)
		|| (m_RenderState.vp.maxZ != vp.maxZ))
	{
		D3D_VIEWPORT sD3DViewport;
		{
			sD3DViewport.X = vp.x;
			sD3DViewport.Y = vp.y;
			sD3DViewport.Width = vp.width;
			sD3DViewport.Height = vp.height;
			sD3DViewport.MinZ = vp.minZ;
			sD3DViewport.MaxZ = vp.maxZ;
		}

		hr = m_pDevice->SetViewport(&sD3DViewport);

		ret = SUCCEEDED(hr);
		if (ret) {
			memcpy(&m_RenderState.vp, &vp, sizeof(vp));
		}
	}

	UN_ASSERT(ret);
	return ret;
}

/**
* �f�t�H���g�̃����_�[�X�e�[�g��ݒ�
*/
void CGraphicsDevice::SetDefaultRenderState()
{
	SetRenderState(E_GRAPH_RS_ZWRITEENABLE, UN_TRUE);
	SetRenderState(E_GRAPH_RS_ZENABLE, UN_TRUE);
	SetRenderState(E_GRAPH_RS_ZFUNC, D3DCMP_LESSEQUAL);

	SetRenderState(E_GRAPH_RS_ALPHATESTENABLE, UN_TRUE);
	SetRenderState(E_GRAPH_RS_ALPHAREF, 0);
	SetRenderState(E_GRAPH_RS_ALPHAFUNC, D3DCMP_ALWAYS);

	SetRenderState(E_GRAPH_RS_ALPHABLENDENABLE, UN_TRUE);
	SetRenderState(E_GRAPH_RS_BLENDMETHOD, E_GRAPH_ALPHA_BLEND_NORMAL);

	SetRenderState(E_GRAPH_RS_FILLMODE, D3DFILL_SOLID);
	SetRenderState(E_GRAPH_RS_CULLMODE, D3DCULL_CW);

	// ���[��E�E�E
	for (UN_UINT i = 0; i < TEX_STAGE_NUM; i++) {
		// MIN_FILTER
		SetSamplerStateFilter(
			i, 
			E_GRAPH_SAMPLER_STATE_TYPE_MINFILTER,
			m_sSamplerState[i].minFilter,
			E_GRAPH_TEX_FILTER_LINEAR);

		// MAG_FILTER
		SetSamplerStateFilter(
			i,
			E_GRAPH_SAMPLER_STATE_TYPE_MAGFILTER,
			m_sSamplerState[i].magFilter,
			E_GRAPH_TEX_FILTER_LINEAR);

		// MIP_FILTER
		SetSamplerStateFilter(
			i,
			E_GRAPH_SAMPLER_STATE_TYPE_MIPFILTER,
			m_sSamplerState[i].mipFilter,
			E_GRAPH_TEX_FILTER_LINEAR);

		// ADDRESS_U
		SetSamplerStateAddr(
			i,
			E_GRAPH_SAMPLER_STATE_TYPE_ADDRESSU,
			m_sSamplerState[i].addressU,
			E_GRAPH_TEX_ADDRESS_CLAMP);

		// ADDRESS_V
		SetSamplerStateAddr(
			i, 
			E_GRAPH_SAMPLER_STATE_TYPE_ADDRESSV,
			m_sSamplerState[i].addressV,
			E_GRAPH_TEX_ADDRESS_CLAMP);
	}
}

/**
* �����_�[�X�e�[�g�ۑ�
*/
UN_BOOL CGraphicsDevice::SaveRenderState()
{
	return m_RenderState.Save();
}

/**
* �����_�[�X�e�[�g���A
*/
UN_BOOL CGraphicsDevice::LoadRenderState()
{
	return m_RenderState.Load(this);
}

/**
* �����_�[�X�e�[�g�ꊇ�Z�b�g
*/
void CGraphicsDevice::SetRenderState(const S_RENDER_STATE& sRS)
{
	SetViewport(sRS.vp);

	for (UN_UINT i = 0; i < E_GRAPH_RS_NUM; i++) {
		SetRenderState(
			static_cast<E_GRAPH_RENDER_STATE>(i),
			sRS.dwRS[i]);
	}

	// �V�U�[��`
	SetScissorTestRect(sRS.rcScissor);
}
