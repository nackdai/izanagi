#include "graph/ogl/GraphicsDevice.h"
#if 0
#include "graph/ogl/BaseTexture.h"
#include "graph/ogl/Texture.h"
#include "graph/ogl/CubeTexture.h"
#include "graph/ogl/Surface.h"
#include "graph/ogl/VertexBuffer.h"
#include "graph/ogl/IndexBuffer.h"
#include "graph/ogl/VertexShader.h"
#include "graph/ogl/PixelShader.h"
#include "graph/ogl/VertexDeclaration.h"
#endif
#include "graph/ogl/Surface.h"
#include "graph/GraphDefs.h"

#include "graph/2d/2DRenderer.h"
#include "std/allocator/MemoryAllocator.h"

using namespace uranus;

CStandardMemoryAllocator CGraphicsDevice::s_cDeviceAllocator;
CGraphicsDevice* CGraphicsDevice::s_pInstance = UN_NULL;

/**
* インスタンス作成
*/
CGraphicsDevice* CGraphicsDevice::CreateGrapicsDevice(
	UN_UINT nBufSize,
	void* pDeviceBuffer)
{
	if (s_pInstance != UN_NULL) {
		// 作成済みなので何もしない
		return s_pInstance;
	}

	// アロケータ初期化
	if (!s_cDeviceAllocator.Init(nBufSize, (UN_UINT8*)pDeviceBuffer)) {
		UN_ASSERT(UN_FALSE);
		return UN_NULL;
	}

	IMemoryAllocator* pAllocator = &s_cDeviceAllocator;

	UN_BOOL result = UN_TRUE;
	UN_UINT8* pBuf = UN_NULL;
	CGraphicsDevice* pInstance = UN_NULL;

	// メモリ確保
	pBuf = (UN_UINT8*)ALLOC_ZERO(pAllocator, sizeof(CGraphicsDevice));
	if (!(result = (pBuf != UN_NULL))) {
		UN_ASSERT(UN_FALSE);
		goto __EXIT__;
	}

	// インスタンス作成
	pInstance = new(pBuf) CGraphicsDevice;
	{
		pInstance->AddRef();
		pInstance->m_pAllocator = pAllocator;
		
		// Nothing is done...
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

// コンストラクタ
CGraphicsDevice::CGraphicsDevice()
{
	m_pAllocator = UN_NULL;

	m_hGLRC = UN_NULL;
	m_hWnd = UN_NULL;
	m_hDC = UN_NULL;

	FILL_ZERO(m_pTexture, sizeof(m_pTexture));
	FILL_ZERO(m_sSamplerState, sizeof(m_sSamplerState));

	FILL_ZERO(&m_RenderState, sizeof(S_RENDER_STATE));

	m_p2DRenderer = UN_NULL;

	m_pResetTexture = UN_NULL;
	m_pResetVB = UN_NULL;
	m_pResetIB = UN_NULL;

	m_Flags.is_call_begin = UN_FALSE;
	m_Flags.is_render_2d = UN_FALSE;
	m_Flags.is_force_set_state = UN_FALSE;
	m_Flags.is_lost_device = UN_FALSE;
}

// デストラクタ
CGraphicsDevice::~CGraphicsDevice()
{
	SAFE_RELEASE(m_pRT);
	SAFE_RELEASE(m_pDepth);

	SAFE_RELEASE(m_p2DRenderer);

	ClearRenderState();

	if (m_hGLRC != UN_NULL) {
		::wglMakeCurrent(UN_NULL, UN_NULL);
		::wglDeleteContext(m_hGLRC);
		m_hGLRC = UN_NULL;
	}
}

// 解放
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
* リセット
*/
UN_BOOL CGraphicsDevice::Reset(const SGraphicsDeviceInitParams& sParams)
{
	UN_BOOL ret = UN_TRUE;

	if (m_hGLRC == UN_NULL) {
		// 本体作成
		ret = CreateBody(sParams);

		if (ret) {
			// 2D描画初期化
			m_p2DRenderer = C2DRenderer::Create2DRenderer(
								this,
								m_pAllocator);
			ret = (m_p2DRenderer != UN_NULL);
			UN_ASSERT(ret);
		}
	}

	if (ret) {
		// ビューポート
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

		// レンダーステートの現在値を取得
		m_RenderState.GetParamsFromGraphicsDevice(this);

		// ステート
		// 強制設定
		m_Flags.is_force_set_state = UN_TRUE;
		SetDefaultRenderState();
		m_Flags.is_force_set_state = UN_FALSE;

		// サーフェスのリセット
		//（フレームバッファのリセット）
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
				// TODO
				// フレームバッファサーフェースを取ってくる・・・

				// 明示的に記述をセットする
				m_pRT->SetDesc();
				m_pDepth->SetDesc();

				// 現在のレンダーターゲットとして保持
				SAFE_REPLACE(m_RenderState.curRT[0], m_pRT);
				SAFE_REPLACE(m_RenderState.curDepth, m_pDepth);
			}
		}
	}

	return ret;
}

// 本体作成
UN_BOOL CGraphicsDevice::CreateBody(const SGraphicsDeviceInitParams& sParams)
{
	m_hDC = ::GetDC(sParams.hWnd);
	VRETURN(m_hDC != UN_NULL);

	::PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),	// nSize
		1,								// nVersion
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,	// dwFlags
		PFD_TYPE_RGBA,					// iPixelType
		24,								// cColorBits
		0,								// cRedBits
		0,								// cRedShift
		0,								// cGreenBits
		0,								// cGreenShift
		0,								// cBlueBits
		0,								// cBlueShift
		8,								// cAlphaBits
		0,								// cAlphaShift
		0,								// cAccumBits
		0,								// cAccumRedBits
		0,								// cAccumGreenBits
		0,								// cAccumBlueBits
		0,								// cAccumAlphaBits
		sParams.DepthBits,				// cDepthBits
		sParams.StencilBits,			// cStencilBits
		0,								// cAuxBuffers
		PFD_MAIN_PLANE,					// iLayerType
		0,								// bReserved
		0,								// dwLayerMask
		0,								// dwVisibleMask
		0,								// dwDamageMask
	};

	UN_INT nPixelFmt = ::ChoosePixelFormat(
							m_hDC,
							&pfd);
	VRETURN(nPixelFmt != 0);

	UN_BOOL ret = ::SetPixelFormat(
					m_hDC,
					nPixelFmt,
					&pfd);
	VRETURN(ret);

	// Create context.
	m_hGLRC = ::wglCreateContext(m_hDC);
	ret = (m_hGLRC != UN_NULL);
	VRETURN(ret);

	ret = ::wglMakeCurrent(m_hDC, m_hGLRC);
	VRETURN(ret);

	m_hWnd = sParams.hWnd;

	{
		m_sPresentParameters.BackBufferWidth = sParams.BackBufferWidth;		// バックバッファの幅
		m_sPresentParameters.BackBufferHeight = sParams.BackBufferHeight;	// バックバッファの高さ
	}

	// TODO
	// バックバッファサイズの正当性のチェック？

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

/**
* 描画開始
*/
UN_BOOL CGraphicsDevice::BeginRender(
	UN_DWORD nClearFlags,
	UN_COLOR nClearColor,
	UN_FLOAT fClearZ,
	UN_DWORD nClearStencil)
{
	UN_ASSERT(m_hGLRC != UN_NULL);
	
	m_Flags.is_call_begin = UN_TRUE;

	Clear(
		nClearFlags,
		nClearColor,
		fClearZ,
		nClearStencil);

	// フレームバッファサーフェスを現在のサーフェスとしてセット
	SAFE_REPLACE(m_RenderState.curRT[0], m_pRT);
	SAFE_REPLACE(m_RenderState.curDepth, m_pDepth);

	// 2D処理開始
	m_p2DRenderer->BeginFrame();

	return UN_TRUE;
}

/**
* 描画終了
*/
void CGraphicsDevice::EndRender()
{
	// 念のため
	EndScene();

	if (m_Flags.is_call_begin) {
		::glEnd();
		::glFlush();
	}

	m_Flags.is_call_begin = UN_FALSE;

	// クリアしてみる
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
		// チェック
		UN_ASSERT(m_cRTMgr[i].IsEmpty());

		SAFE_RELEASE(m_RenderState.curRT[i]);
	}

	// チェック
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
* クリア
*/
void CGraphicsDevice::Clear(
	UN_DWORD nClearFlags,
	UN_COLOR nClearColor,
	UN_FLOAT fClearZ,
	UN_DWORD nClearStencil)
{
	if (nClearFlags > 0) {
		// OpenGL のクリアフラグに変換する
		UN_DWORD nClearFlag = 0;
		nClearFlag |= _CONV_CLEAR_FLAG(nClearFlags, E_GRAPH_CLEAR_FLAG_COLOR,   GL_COLOR_BUFFER_BIT);
		nClearFlag |= _CONV_CLEAR_FLAG(nClearFlags, E_GRAPH_CLEAR_FLAG_DEPTH,   GL_DEPTH_BUFFER_BIT);
		nClearFlag |= _CONV_CLEAR_FLAG(nClearFlags, E_GRAPH_CLEAR_FLAG_STENCIL, GL_STENCIL_BUFFER_BIT);

		CColor cClearClr(nClearColor);

		::glClearColor(
			cClearClr.GetRAsFloat(),
			cClearClr.GetGAsFloat(),
			cClearClr.GetBAsFloat(),
			cClearClr.GetAAsFloat());

		::glClear(nClearFlag);
	}
}

/**
* シーン描画開始
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
	// MRTは無しで・・・
	UN_ASSERT(nCount <= 1);

	UN_BOOL ret = UN_TRUE;

	if ((nCount > 0) && (pRT != UN_NULL)) {
		// レンダーターゲットセット
		PushRenderTarget(pRT, nCount);
	}
	if (pDepth != UN_NULL) {
		// 深度・ステンシルセット
		PushDepthStencil(pDepth);
	}

	// クリア
	Clear(
		nClearFlags,
		nClearColor,
		fClearZ,
		nClearStencil);

	return ret;
}

/**
* シーン描画終了
*/
void CGraphicsDevice::EndScene(UN_UINT flag/* = 0xffffffff*/)
{
	CSurface* pRTList[MAX_MRT_NUM];
	memset(pRTList, 0, sizeof(pRTList));

	UN_UINT nRTNum = 0;

	// レンダーターゲット
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
		// 深度・ステンシル
		CSurface* pDepth = m_cDepthMgr.Pop();
		if (pDepth != UN_NULL) {
			SetDepthStencil(pDepth);
		}
	}
}

/**
* 同期
*/
UN_BOOL CGraphicsDevice::Present()
{
	UN_ASSERT(m_hDC != UN_NULL);

	UN_BOOL ret = ::SwapBuffers(m_hDC);
	UN_ASSERT(ret);

	return ret;
}

/**
* 2D描画開始
*/
UN_BOOL CGraphicsDevice::Begin2D()
{
	UN_ASSERT(m_p2DRenderer != UN_NULL);

	// 描画開始
	UN_BOOL ret = m_p2DRenderer->BeginDraw();

	if (ret) {
		// 2D描画用レンダーステート設定
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
* 2D描画終了
*/
UN_BOOL CGraphicsDevice::End2D()
{
	UN_BOOL ret = UN_TRUE;

	if (m_Flags.is_render_2d) {
		UN_ASSERT(m_p2DRenderer != UN_NULL);

		// 描画終了
		ret = m_p2DRenderer->EndDraw(this);

		// レンダーステートを元に戻す
		if (ret) {
			LoadRenderState();
		}
	}

	m_Flags.is_render_2d = UN_FALSE;

	return ret;
}

/**
* 2D描画バッファフラッシュ
*/
UN_BOOL CGraphicsDevice::Flush2D()
{
	UN_BOOL ret = m_p2DRenderer->Flush(this);
	return ret;
}

/**
* 2Dスプライト描画
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
* 2Dスプライト描画
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
* 2D矩形描画
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
* 2Dライン描画
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
* 2D描画モードセット
*/
void CGraphicsDevice::Set2DRenderOp(E_GRAPH_2D_RENDER_OP nOp)
{
	UN_ASSERT(m_p2DRenderer != UN_NULL);
	m_p2DRenderer->SetRenderOp(this, nOp);
}

/**
* 2D描画モード取得
*/
E_GRAPH_2D_RENDER_OP CGraphicsDevice::Get2DRenderOp() const
{
	UN_ASSERT(m_p2DRenderer != UN_NULL);
	return m_p2DRenderer->GetRenderOp();
}

/**
* テクスチャセット
*/
UN_BOOL CGraphicsDevice::SetTexture(UN_UINT nStage, CBaseTexture* pTex)
{
	UN_ASSERT(m_pDevice != NULL);

	if (m_pTexture[nStage] == pTex) {
		// すでにセットされている
		return UN_TRUE;
	}
	else {
		// 異なるテクスチャ
		if (m_Flags.is_render_2d) {
			// 2D描画中の場合
			// バッファに溜まっている分を描画してしまう
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

	// 保持しておく
	m_pTexture[nStage] = pTex;

	// うーん・・・
	// ステート
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
* テクスチャ取得
*/
CBaseTexture* CGraphicsDevice::GetTexture(UN_UINT nStage)
{
	UN_ASSERT(nStage < TEX_STAGE_NUM);
	return m_pTexture[nStage];
}

// レンダーターゲットセット
UN_BOOL CGraphicsDevice::PushRenderTarget(CSurface** pSurface, UN_UINT num)
{
	UN_ASSERT(pSurface != UN_NULL);

	// TODO
	// MRTは許さない
	UN_ASSERT(num == 1);
	UN_ASSERT(num < MAX_MRT_NUM);

	UN_BOOL ret = UN_FALSE;

	// 現在のレンダーターゲットをプッシュ
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
	// レンダーターゲットを入れ替える
	for (UN_UINT i = 0; i < num; i++) {
		if (m_RenderState.curRT[i] != pSurface[i]) {
			m_pDevice->SetRenderTarget(i, pSurface[i]->GetRawInterface());
			SAFE_REPLACE(m_RenderState.curRT[i], pSurface[i]);
		}
	}

	// TODO
	// 強制的に０番目のサーフェスのサイズにビューポートを変換する
	SViewport vp;
	memcpy(&vp, &m_RenderState.vp, sizeof(vp));
	vp.x = vp.y = 0;
	vp.width = pSurface[0]->GetWidth();
	vp.height = pSurface[0]->GetHeight();
	SetViewport(vp);
}

// 深度・ステンシルセット
UN_BOOL CGraphicsDevice::PushDepthStencil(CSurface* pSurface)
{
	UN_BOOL ret = UN_FALSE;

	// 現在の深度をプッシュ
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
		// レンダーターゲットを入れ替える
		m_pDevice->SetDepthStencilSurface(pSurface->GetRawInterface());
		SAFE_REPLACE(m_RenderState.curDepth, pSurface);
	}
}

/**
* 頂点バッファセット
*/
UN_BOOL CGraphicsDevice::SetVertexBuffer(
	UN_UINT nStreamIdx,
	UN_UINT nOffsetByte,
	UN_UINT nStride,
	CVertexBuffer* pVB)
{
	if (m_RenderState.curVB == pVB) {
		// すでに設定されている
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

		// 現在設定されているものとして保持
		SAFE_REPLACE(m_RenderState.curVB, pVB);
	}

	return UN_TRUE;
}

/**
* FVFセット
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
* インデックスバッファセット
*/
UN_BOOL CGraphicsDevice::SetIndexBuffer(CIndexBuffer* pIB)
{
	if (m_RenderState.curIB == pIB) {
		// すでに設定されている
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

		// 現在設定されているものとして保持
		SAFE_REPLACE(m_RenderState.curIB, pIB);
	}

	return UN_TRUE;
}

/**
* インデックスバッファ描画
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
* インデックスバッファなし描画
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
* 頂点シェーダセット
*/
UN_BOOL CGraphicsDevice::SetVertexShader(CVertexShader* pVS)
{
	if (m_RenderState.curVS == pVS) {
		// すでに設定されている
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

		// 現在設定されているものとして保持
		SAFE_REPLACE(m_RenderState.curVS, pVS);
	}

	return UN_TRUE;
}

/**
* ピクセルシェーダセット
*/
UN_BOOL CGraphicsDevice::SetPixelShader(CPixelShader* pPS)
{
	if (m_RenderState.curPS == pPS) {
		// すでに設定されている
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

		// 現在設定されているものとして保持
		SAFE_REPLACE(m_RenderState.curPS, pPS);
	}

	return UN_TRUE;
}

/**
* 頂点宣言セット
*/
UN_BOOL CGraphicsDevice::SetVertexDeclaration(CVertexDeclaration* pVD)
{
	if (m_RenderState.curVD == pVD) {
		// すでに設定されている
		return UN_TRUE;
	}

	if (pVD != UN_NULL) {
		HRESULT hr = m_pDevice->SetVertexDeclaration(pVD->GetRawInterface());
		VRETURN(SUCCEEDED(hr));

		// TODO
		// FVFを強制クリア
		SetFVF(0);
	}

	// 現在設定されているものとして保持
	SAFE_REPLACE(m_RenderState.curVD, pVD);

	return UN_TRUE;
}

/**
* ビューポートセット
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
* デフォルトのレンダーステートを設定
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

	// うーん・・・
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
* レンダーステート保存
*/
UN_BOOL CGraphicsDevice::SaveRenderState()
{
	return m_RenderState.Save();
}

/**
* レンダーステート復帰
*/
UN_BOOL CGraphicsDevice::LoadRenderState()
{
	return m_RenderState.Load(this);
}

/**
* レンダーステート一括セット
*/
void CGraphicsDevice::SetRenderState(const S_RENDER_STATE& sRS)
{
	SetViewport(sRS.vp);

	for (UN_UINT i = 0; i < E_GRAPH_RS_NUM; i++) {
		SetRenderState(
			static_cast<E_GRAPH_RENDER_STATE>(i),
			sRS.dwRS[i]);
	}

	// シザー矩形
	SetScissorTestRect(sRS.rcScissor);
}
