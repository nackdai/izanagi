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

using namespace izanagi;

CGraphicsDevice* CGraphicsDevice::s_pInstance = IZ_NULL;

/**
* インスタンス作成
*/
CGraphicsDevice* CGraphicsDevice::CreateGrapicsDevice(
	IMemoryAllocator* allocator)
{
	if (s_pInstance != IZ_NULL) {
		// 作成済みなので何もしない
		return s_pInstance;
	}

	IZ_ASSERT(allocator != IZ_NULL);

	IZ_BOOL result = IZ_TRUE;
	IZ_UINT8* pBuf = IZ_NULL;
	CGraphicsDevice* pInstance = IZ_NULL;

	// メモリ確保
	pBuf = (IZ_UINT8*)ALLOC_ZERO(allocator, sizeof(CGraphicsDevice));
	if (!(result = (pBuf != IZ_NULL))) {
		IZ_ASSERT(IZ_FALSE);
		goto __EXIT__;
	}

	// インスタンス作成
	pInstance = new(pBuf) CGraphicsDevice;
	{
		pInstance->m_Allocator = allocator;

		// IDirect3D9 インターフェースの取得
		pInstance->m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
		result = (pInstance->m_pD3D != IZ_NULL);
		IZ_ASSERT(result);

		// 現在の画面モードの取得
		if (result) {
			HRESULT hr = pInstance->m_pD3D->GetAdapterDisplayMode(
							D3DADAPTER_DEFAULT,
							&pInstance->m_sDisplayMode);
			result = SUCCEEDED(hr);
			IZ_ASSERT(result);
		}

		pInstance->AddRef();
	}

	if (!result) {
		goto __EXIT__;
	}

	s_pInstance = pInstance;
	
__EXIT__:
	if (!result) {
		if (pInstance != IZ_NULL) {
			SAFE_RELEASE(pInstance);
		}
		else if (pBuf != IZ_NULL) {
			allocator->Free(pBuf);
		}
	}

	return pInstance;
}

// コンストラクタ
CGraphicsDevice::CGraphicsDevice()
{
	m_Allocator = IZ_NULL;

	m_pD3D = IZ_NULL;

	m_pDevice = IZ_NULL;
	FILL_ZERO(&m_sPresentParameters, sizeof(m_sPresentParameters));

	m_hFocusWindow = IZ_NULL;

	FILL_ZERO(m_pTexture, sizeof(m_pTexture));
	FILL_ZERO(m_sSamplerState, sizeof(m_sSamplerState));

	FILL_ZERO(&m_RenderState, sizeof(m_RenderState));

	m_p2DRenderer = IZ_NULL;

	m_pResetTexture = IZ_NULL;
	m_pResetVB = IZ_NULL;
	m_pResetIB = IZ_NULL;

	m_Flags.is_call_begin = IZ_FALSE;
	m_Flags.is_render_2d = IZ_FALSE;
	m_Flags.is_force_set_state = IZ_FALSE;
	m_Flags.is_lost_device = IZ_FALSE;

	m_pResetCallBack = IZ_NULL;
	m_pLostDeviceCallBack = IZ_NULL;
}

// デストラクタ
CGraphicsDevice::~CGraphicsDevice()
{
	SAFE_RELEASE(m_pRT);
	SAFE_RELEASE(m_pDepth);

	SAFE_RELEASE(m_p2DRenderer);

	ClearRenderState();

	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_pD3D);
}

// 解放
void CGraphicsDevice::InternalRelease()
{	
	delete this;

	if (m_Allocator != IZ_NULL) {
		m_Allocator->Free(this);
	}

	if (s_pInstance == this) {
		s_pInstance = IZ_NULL;
	}
}

/**
* リセット
*/
IZ_BOOL CGraphicsDevice::Reset(const SGraphicsDeviceInitParams& sParams)
{
	IZ_BOOL ret = IZ_TRUE;

	if (m_pDevice == IZ_NULL) {
		// 本体作成
		ret = CreateBody(sParams);

		if (ret) {
			// 2D描画初期化
			m_p2DRenderer = C2DRenderer::Create2DRenderer(
								this,
								m_Allocator);
			ret = (m_p2DRenderer != IZ_NULL);
			IZ_ASSERT(ret);
		}
	}
	else {
		// リセット
		ret = ResetInternal(sParams);
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
		m_Flags.is_force_set_state = IZ_TRUE;
		SetDefaultRenderState();
		m_Flags.is_force_set_state = IZ_FALSE;

		// サーフェスのリセット
		//（フレームバッファのリセット）
		{
			if (m_pRT != IZ_NULL) {
				m_pRT->Reset(IZ_NULL, 0);
			}
			else {
				m_pRT = CSurface::CreateSurface(m_Allocator);
			}

			if (m_pDepth != IZ_NULL) {
				m_pDepth->Reset(IZ_NULL, 0);
			}
			else {
				m_pDepth = CSurface::CreateSurface(m_Allocator);
			}

			ret = ((m_pRT != IZ_NULL) && (m_pDepth != IZ_NULL));
			IZ_ASSERT(ret);
			
			if (ret) {
				// フレームバッファサーフェースを取ってくる・・・
				m_pDevice->GetRenderTarget(0, &m_pRT->m_pSurface);
				m_pDevice->GetDepthStencilSurface(&m_pDepth->m_pSurface);

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
IZ_BOOL CGraphicsDevice::CreateBody(const SGraphicsDeviceInitParams& sParams)
{
	IZ_ASSERT(m_pD3D != IZ_NULL);

	IZ_BOOL ret = IZ_TRUE;

	m_hFocusWindow = sParams.hFocusWindow;

	// D3DPRESENT_PARAMETERS の設定
	{
		FILL_ZERO(&m_sPresentParameters, sizeof(m_sPresentParameters));

		m_sPresentParameters.BackBufferCount = 2;

		m_sPresentParameters.Windowed = sParams.Windowed;					// 画面モード(ウインドウモード)
		m_sPresentParameters.BackBufferWidth = sParams.BackBufferWidth;		// バックバッファの幅
		m_sPresentParameters.BackBufferHeight = sParams.BackBufferHeight;	// バックバッファの高さ

		// バックバッファのフォーマット
		m_sPresentParameters.BackBufferFormat = m_sDisplayMode.Format;

		m_sPresentParameters.MultiSampleType = sParams.MultiSampleType;		// マルチ・サンプリングの種類

		// スワップエフェクトの種類
		m_sPresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;

		m_sPresentParameters.hDeviceWindow = sParams.hDeviceWindow;

#if 1
		// Zバッファの自動作成
		m_sPresentParameters.EnableAutoDepthStencil = IZ_TRUE;
		//m_sPresentParameters.AutoDepthStencilFormat = D3DFMT_D24S8;
		m_sPresentParameters.AutoDepthStencilFormat = sParams.DepthStencilFormat;
#else
		// Zバッファを自動作成しない
		m_sPresentParameters.EnableAutoDepthStencil = IZ_FALSE;
#endif

		// VSyncのタイプ
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
		// 結局失敗した
		ret = IZ_FALSE;
    }

	return ret;
}

template <class _T>
void CGraphicsDevice::ReleaseResource(_T* pList)
{
	for (_T* p = pList; p != IZ_NULL; p = p->m_pNext) {
		p->ReleaseResource();
	}
}

template <class _T>
void CGraphicsDevice::ResetResource(_T* pList)
{
	for (_T* p = pList; p != IZ_NULL; p = p->m_pNext) {
		p->Reset();
	}
}

IZ_BOOL CGraphicsDevice::ResetInternal(const SGraphicsDeviceInitParams& sParams)
{
	// リソースを解放する
	{
		m_pRT->ReleaseResource();
		m_pDepth->ReleaseResource();

		m_p2DRenderer->ResetResource();

		ReleaseResource(m_pResetTexture);
		ReleaseResource(m_pResetVB);
		ReleaseResource(m_pResetIB);
	}

	// クリアされるものを保存しておく
	IZ_UINT nBackBufferCount = m_sPresentParameters.BackBufferCount;
	D3DFORMAT fmt = m_sPresentParameters.BackBufferFormat;

	// 新しく設定されるもの
	m_sPresentParameters.BackBufferWidth = sParams.BackBufferWidth;
	m_sPresentParameters.BackBufferHeight = sParams.BackBufferHeight;

	HRESULT hr = m_pDevice->Reset(&m_sPresentParameters);
	IZ_ASSERT(hr != D3DERR_DRIVERINTERNALERROR);

	IZ_BOOL ret = SUCCEEDED(hr);

	if (ret) {
		// クリアされたので設定値に変更する
		m_sPresentParameters.BackBufferCount = nBackBufferCount;
		m_sPresentParameters.BackBufferFormat = fmt;

		// リソースをリセットする
		{
			m_pRT->Reset(IZ_NULL, 0);
			m_pDepth->Reset(IZ_NULL, 0);

			m_p2DRenderer->Reset();

			ResetResource(m_pResetTexture);
			ResetResource(m_pResetVB);
			ResetResource(m_pResetIB);
		}

		// コールバック
		if (m_pResetCallBack != IZ_NULL) {
			ret = (*m_pResetCallBack)();
			IZ_ASSERT(ret);
		}
	}

	return ret;
}

/**
* 描画開始
*/
IZ_BOOL CGraphicsDevice::BeginRender(
	IZ_DWORD nClearFlags,
	IZ_COLOR nClearColor,
	IZ_FLOAT fClearZ,
	IZ_DWORD nClearStencil)
{
	IZ_ASSERT(m_pDevice != NULL);

	IZ_BOOL ret = IZ_FALSE;

	if (!m_Flags.is_lost_device) {
		// デバイスロストしてないので、通常の描画開始処理を行う
		m_pDevice->BeginScene();
		m_Flags.is_call_begin = IZ_TRUE;

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

		ret = IZ_TRUE;
	}

	return ret;
}

/**
* 描画終了
*/
void CGraphicsDevice::EndRender()
{
	// 念のため
	EndScene();

	if (m_Flags.is_call_begin) {
		m_pDevice->EndScene();
	}

	m_Flags.is_call_begin = IZ_FALSE;

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
	SetVertexShader(IZ_NULL);
	SetPixelShader(IZ_NULL);
	SetVertexBuffer(0, 0, 0, IZ_NULL);
	SetIndexBuffer(IZ_NULL);

	SAFE_RELEASE(m_RenderState.curVD);
#endif

	for (IZ_UINT i = 0; i < MAX_MRT_NUM; ++i) {
		// チェック
		IZ_ASSERT(m_cRTMgr[i].IsEmpty());

		SAFE_RELEASE(m_RenderState.curRT[i]);
	}

	// チェック
	IZ_ASSERT(m_cDepthMgr.IsEmpty());

	SAFE_RELEASE(m_RenderState.curDepth);

	for (IZ_UINT i = 0; i < TEX_STAGE_NUM; ++i) {
		SetTexture(i, IZ_NULL);
	}
}

#ifndef _CONV_CLEAR_FLAG
#define _CONV_CLEAR_FLAG(flag, mask, dst)	((flag) & (mask) ? (dst) : 0)
#else
IZ_C_ASSERT(IZ_FALSE);
#endif

/**
* クリア
*/
void CGraphicsDevice::Clear(
	IZ_DWORD nClearFlags,
	IZ_COLOR nClearColor,
	IZ_FLOAT fClearZ,
	IZ_DWORD nClearStencil)
{
	IZ_ASSERT(m_pDevice != NULL);

	if (nClearFlags > 0) {
		// DirectX のクリアフラグに変換する
		IZ_DWORD nDXClearFlag = 0;
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
* シーン描画開始
*/
IZ_BOOL CGraphicsDevice::BeginScene(
	IZ_DWORD nClearFlags,
	IZ_COLOR nClearColor/*= 0*/,
	IZ_FLOAT fClearZ/*= 1.0f*/,
	IZ_DWORD nClearStencil/*= 0*/)
{
	return BeginScene(
			IZ_NULL, 0,
			IZ_NULL,
			nClearFlags,
			nClearColor,
			fClearZ,
			nClearStencil);
}

IZ_BOOL CGraphicsDevice::BeginScene(
	CSurface** pRT,
	IZ_UINT nCount,
	IZ_DWORD nClearFlags,
	IZ_COLOR nClearColor/*= 0*/,
	IZ_FLOAT fClearZ/*= 1.0f*/,
	IZ_DWORD nClearStencil/*= 0*/)
{
	return BeginScene(
			pRT, nCount,
			IZ_NULL,
			nClearFlags,
			nClearColor,
			fClearZ,
			nClearStencil);
}

IZ_BOOL CGraphicsDevice::BeginScene(
	CSurface** pRT,
	IZ_UINT nCount,
	CSurface* pDepth,
	IZ_DWORD nClearFlags,
	IZ_COLOR nClearColor/*= 0*/,
	IZ_FLOAT fClearZ/*= 1.0f*/,
	IZ_DWORD nClearStencil/*= 0*/)
{
	IZ_ASSERT(m_pDevice != NULL);

	// TODO
	// MRTは無しで・・・
	IZ_ASSERT(nCount <= 1);

	IZ_BOOL ret = IZ_TRUE;

	if ((nCount > 0) && (pRT != IZ_NULL)) {
		// レンダーターゲットセット
		PushRenderTarget(pRT, nCount);
	}
	if (pDepth != IZ_NULL) {
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
void CGraphicsDevice::EndScene(IZ_UINT flag/* = 0xffffffff*/)
{
	CSurface* pRTList[MAX_MRT_NUM];
	memset(pRTList, 0, sizeof(pRTList));

	IZ_UINT nRTNum = 0;

	// レンダーターゲット
	for (IZ_UINT i = 0; i < MAX_MRT_NUM; ++i) {
		if ((flag & (1 << i)) > 0) {
			pRTList[i] = m_cRTMgr[i].Pop();
			nRTNum = (pRTList[i] != IZ_NULL ? nRTNum + 1 : nRTNum);
		}
	}

	if (nRTNum > 0) {
		SetRenderTarget(pRTList, nRTNum);
	}

	if ((flag & E_GRAPH_END_SCENE_FLAG_DEPTH_STENCIL) > 0) {
		// 深度・ステンシル
		CSurface* pDepth = m_cDepthMgr.Pop();
		if (pDepth != IZ_NULL) {
			SetDepthStencil(pDepth);
		}
	}
}

/**
* 同期
*/
IZ_BOOL CGraphicsDevice::Present()
{
	IZ_BOOL ret = IZ_TRUE;

	HRESULT hr = m_pDevice->Present(
					IZ_NULL,
					IZ_NULL,
					m_hFocusWindow,
					IZ_NULL);

	if (hr == D3DERR_DEVICELOST) {
		// デバイスロストした
		m_Flags.is_lost_device = IZ_TRUE;

		// デバイスロストしたときのコールバック
		if (m_pLostDeviceCallBack != IZ_NULL) {
			(*m_pLostDeviceCallBack)();
		}

		SGraphicsDeviceInitParams sParams;
		{
			sParams.BackBufferWidth = m_sPresentParameters.BackBufferWidth;
			sParams.BackBufferHeight = m_sPresentParameters.BackBufferHeight;
		}

		// 復帰をこころみる
		ret = Reset(sParams);

		if (ret) {
			// 復帰した
			m_Flags.is_lost_device = IZ_FALSE;
		}
	}
	else {
		ret = SUCCEEDED(hr);
	}

	IZ_ASSERT(ret);
	return ret;
}

/**
* 2D描画開始
*/
IZ_BOOL CGraphicsDevice::Begin2D()
{
	IZ_ASSERT(m_p2DRenderer != IZ_NULL);

	// 描画開始
	IZ_BOOL ret = m_p2DRenderer->BeginDraw();

	if (ret) {
		// 2D描画用レンダーステート設定
		SaveRenderState();

		SetRenderState(E_GRAPH_RS_ZWRITEENABLE, IZ_FALSE);
		SetRenderState(E_GRAPH_RS_ZENABLE, IZ_FALSE);
		SetRenderState(E_GRAPH_RS_ALPHATESTENABLE, IZ_FALSE);
		SetRenderState(E_GRAPH_RS_ALPHABLENDENABLE, IZ_TRUE);
		SetRenderState(E_GRAPH_RS_BLENDMETHOD, E_GRAPH_ALPHA_BLEND_NORMAL);
		SetRenderState(E_GRAPH_RS_FILLMODE, E_GRAPH_FILL_MODE_SOLID);
		SetRenderState(E_GRAPH_RS_CULLMODE, E_GRAPH_CULL_DEFAULT);

		SetVertexShader(IZ_NULL);
		SetPixelShader(IZ_NULL);

		m_Flags.is_render_2d = IZ_TRUE;
	}

	return ret;
}

/**
* 2D描画終了
*/
IZ_BOOL CGraphicsDevice::End2D()
{
	IZ_BOOL ret = IZ_TRUE;

	if (m_Flags.is_render_2d) {
		IZ_ASSERT(m_p2DRenderer != IZ_NULL);

		// 描画終了
		ret = m_p2DRenderer->EndDraw(this);

		// レンダーステートを元に戻す
		if (ret) {
			LoadRenderState();
		}
	}

	m_Flags.is_render_2d = IZ_FALSE;

	return ret;
}

/**
* 2D描画バッファフラッシュ
*/
IZ_BOOL CGraphicsDevice::Flush2D()
{
	IZ_BOOL ret = m_p2DRenderer->Flush(this);
	return ret;
}

/**
* 2Dスプライト描画
*/
IZ_BOOL CGraphicsDevice::Draw2DSprite(
	const CFloatRect& rcSrc,
	const CIntRect& rcDst,
	const IZ_COLOR color/*= IZ_COLOR_RGBA(255, 255, 255, 255)*/)
{
	IZ_ASSERT(m_p2DRenderer != IZ_NULL);
	IZ_ASSERT(m_Flags.is_render_2d);

	IZ_BOOL ret = IZ_FALSE;

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
IZ_BOOL CGraphicsDevice::Draw2DSpriteEx(
	const CIntRect& rcSrc,
	const CIntRect& rcDst,
	const IZ_COLOR color/*= IZ_COLOR_RGBA(255, 255, 255, 255)*/)
{
	IZ_ASSERT(m_p2DRenderer != IZ_NULL);
	IZ_ASSERT(m_Flags.is_render_2d);

	IZ_BOOL ret = IZ_FALSE;

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
IZ_BOOL CGraphicsDevice::Draw2DRect(
	const CIntRect& rcDst,
	const IZ_COLOR color)
{
	IZ_ASSERT(m_p2DRenderer != IZ_NULL);
	IZ_ASSERT(m_Flags.is_render_2d);

	IZ_BOOL ret = IZ_FALSE;

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
IZ_BOOL CGraphicsDevice::Draw2DLine(
	const CIntPoint& ptStart,
	const CIntPoint& ptGoal,
	const IZ_COLOR color)
{
	IZ_ASSERT(m_p2DRenderer != IZ_NULL);
	IZ_ASSERT(m_Flags.is_render_2d);

	IZ_BOOL ret = IZ_FALSE;

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
	IZ_ASSERT(m_p2DRenderer != IZ_NULL);
	m_p2DRenderer->SetRenderOp(this, nOp);
}

/**
* 2D描画モード取得
*/
E_GRAPH_2D_RENDER_OP CGraphicsDevice::Get2DRenderOp() const
{
	IZ_ASSERT(m_p2DRenderer != IZ_NULL);
	return m_p2DRenderer->GetRenderOp();
}

/**
* テクスチャセット
*/
IZ_BOOL CGraphicsDevice::SetTexture(IZ_UINT nStage, CBaseTexture* pTex)
{
	IZ_ASSERT(m_pDevice != NULL);

	if (m_pTexture[nStage] == pTex) {
		// すでにセットされている
		return IZ_TRUE;
	}
	else {
		// 異なるテクスチャ
		if (m_Flags.is_render_2d) {
			// 2D描画中の場合
			// バッファに溜まっている分を描画してしまう
			if (!m_p2DRenderer->Flush(this)) {
				IZ_ASSERT(IZ_FALSE);
				return IZ_FALSE;
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

	return IZ_TRUE;
}

/**
* テクスチャ取得
*/
CBaseTexture* CGraphicsDevice::GetTexture(IZ_UINT nStage)
{
	IZ_ASSERT(nStage < TEX_STAGE_NUM);
	return m_pTexture[nStage];
}

// レンダーターゲットセット
IZ_BOOL CGraphicsDevice::PushRenderTarget(CSurface** pSurface, IZ_UINT num)
{
	IZ_ASSERT(pSurface != IZ_NULL);

	// TODO
	// MRTは許さない
	IZ_ASSERT(num == 1);
	IZ_ASSERT(num < MAX_MRT_NUM);

	IZ_BOOL ret = IZ_FALSE;

	// 現在のレンダーターゲットをプッシュ
	for (IZ_UINT i = 0; i < num; ++i) {
		if ((pSurface[i] != IZ_NULL)
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

void CGraphicsDevice::SetRenderTarget(CSurface** pSurface, IZ_UINT num)
{
	// レンダーターゲットを入れ替える
	for (IZ_UINT i = 0; i < num; ++i) {
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
IZ_BOOL CGraphicsDevice::PushDepthStencil(CSurface* pSurface)
{
	IZ_BOOL ret = IZ_FALSE;

	// 現在の深度をプッシュ
	if ((pSurface != IZ_NULL)
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
IZ_BOOL CGraphicsDevice::SetVertexBuffer(
	IZ_UINT nStreamIdx,
	IZ_UINT nOffsetByte,
	IZ_UINT nStride,
	CVertexBuffer* pVB)
{
	if (m_RenderState.curVB == pVB) {
		// すでに設定されている
		return IZ_TRUE;
	}

#if 0
	if (pVB != IZ_NULL) {
		HRESULT hr = m_pDevice->SetStreamSource(
						nStreamIdx,
						pVB->GetRawInterface(),
						nOffsetByte,
						nStride);
#else
	{
		HRESULT hr = m_pDevice->SetStreamSource(
						nStreamIdx,
						pVB != IZ_NULL ? pVB->GetRawInterface() : IZ_NULL,
						nOffsetByte,
						nStride);
#endif
		VRETURN(SUCCEEDED(hr));

		// 現在設定されているものとして保持
		SAFE_REPLACE(m_RenderState.curVB, pVB);
	}

	return IZ_TRUE;
}

/**
* インデックスバッファセット
*/
IZ_BOOL CGraphicsDevice::SetIndexBuffer(CIndexBuffer* pIB)
{
	if (m_RenderState.curIB == pIB) {
		// すでに設定されている
		return IZ_TRUE;
	}

#if 0
	if (pIB != IZ_NULL) {
		HRESULT hr = m_pDevice->SetIndices(pIB->GetRawInterface());
#else
	{
		HRESULT hr = m_pDevice->SetIndices(
						pIB != IZ_NULL ? pIB->GetRawInterface() : IZ_NULL);
#endif
		VRETURN(SUCCEEDED(hr));

		// 現在設定されているものとして保持
		SAFE_REPLACE(m_RenderState.curIB, pIB);
	}

	return IZ_TRUE;
}

/**
* インデックスバッファ描画
*/
IZ_BOOL CGraphicsDevice::DrawIndexedPrimitive(
	E_GRAPH_PRIM_TYPE prim_type,
	IZ_UINT nBaseIdx,
	IZ_UINT nMinIdx,
	IZ_UINT nVtxNum,
	IZ_UINT nStartIdx,
	IZ_UINT nPrimCnt)
{
	HRESULT hr = m_pDevice->DrawIndexedPrimitive(
					IZ_GET_TARGET_PRIM_TYPE(prim_type),
					nBaseIdx,
					nMinIdx,
					nVtxNum,
					nStartIdx,
					nPrimCnt);
	VRETURN(SUCCEEDED(hr));

	return IZ_TRUE;
}

/**
* インデックスバッファなし描画
*/
IZ_BOOL CGraphicsDevice::DrawPrimitive(
	E_GRAPH_PRIM_TYPE prim_type,
	IZ_UINT nStartIdx,
	IZ_UINT nPrimCnt)
{
	HRESULT hr = m_pDevice->DrawPrimitive(
					IZ_GET_TARGET_PRIM_TYPE(prim_type),
					nStartIdx,
					nPrimCnt);
	VRETURN(SUCCEEDED(hr));

	return IZ_TRUE;
}

/**
* 頂点シェーダセット
*/
IZ_BOOL CGraphicsDevice::SetVertexShader(CVertexShader* pVS)
{
	if (m_RenderState.curVS == pVS) {
		// すでに設定されている
		return IZ_TRUE;
	}

#if 0
	if (pVS != IZ_NULL) {
		HRESULT hr = m_pDevice->SetVertexShader(pVS->GetRawInterface());
#else
	{
		HRESULT hr = m_pDevice->SetVertexShader(
						pVS != IZ_NULL ? pVS->GetRawInterface() : IZ_NULL);
#endif
		VRETURN(SUCCEEDED(hr));

		// 現在設定されているものとして保持
		SAFE_REPLACE(m_RenderState.curVS, pVS);
	}

	return IZ_TRUE;
}

/**
* ピクセルシェーダセット
*/
IZ_BOOL CGraphicsDevice::SetPixelShader(CPixelShader* pPS)
{
	if (m_RenderState.curPS == pPS) {
		// すでに設定されている
		return IZ_TRUE;
	}

#if 0
	if (pPS != IZ_NULL) {
		HRESULT hr = m_pDevice->SetPixelShader(pPS->GetRawInterface());
#else
	{
		HRESULT hr = m_pDevice->SetPixelShader(
						pPS != IZ_NULL ? pPS->GetRawInterface() : IZ_NULL);
#endif
		VRETURN(SUCCEEDED(hr));

		// 現在設定されているものとして保持
		SAFE_REPLACE(m_RenderState.curPS, pPS);
	}

	return IZ_TRUE;
}

/**
* 頂点宣言セット
*/
IZ_BOOL CGraphicsDevice::SetVertexDeclaration(CVertexDeclaration* pVD)
{
	if (m_RenderState.curVD == pVD) {
		// すでに設定されている
		return IZ_TRUE;
	}

	if (pVD != IZ_NULL) {
		HRESULT hr = m_pDevice->SetVertexDeclaration(pVD->GetRawInterface());
		VRETURN(SUCCEEDED(hr));
	}

	// 現在設定されているものとして保持
	SAFE_REPLACE(m_RenderState.curVD, pVD);

	return IZ_TRUE;
}

/**
* ビューポートセット
*/
IZ_BOOL CGraphicsDevice::SetViewport(const SViewport& vp)
{
	if (m_Flags.is_render_2d) {
		// TODO
		// 2D描画中は不可
		return IZ_TRUE;
	}

	IZ_BOOL ret = IZ_TRUE;
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

	IZ_ASSERT(ret);
	return ret;
}

/**
* デフォルトのレンダーステートを設定
*/
void CGraphicsDevice::SetDefaultRenderState()
{
	SetRenderState(E_GRAPH_RS_ZWRITEENABLE, IZ_TRUE);
	SetRenderState(E_GRAPH_RS_ZENABLE, IZ_TRUE);
	SetRenderState(E_GRAPH_RS_ZFUNC, E_GRAPH_CMP_FUNC_LESSEQUAL);

	SetRenderState(E_GRAPH_RS_ALPHATESTENABLE, IZ_TRUE);
	SetRenderState(E_GRAPH_RS_ALPHAREF, 0);
	SetRenderState(E_GRAPH_RS_ALPHAFUNC, E_GRAPH_CMP_FUNC_ALWAYS);

	SetRenderState(E_GRAPH_RS_ALPHABLENDENABLE, IZ_TRUE);
	SetRenderState(E_GRAPH_RS_BLENDMETHOD, E_GRAPH_ALPHA_BLEND_NORMAL);

	SetRenderState(E_GRAPH_RS_FILLMODE, E_GRAPH_FILL_MODE_SOLID);

	SetRenderState(E_GRAPH_RS_CULLMODE, E_GRAPH_CULL_DEFAULT);

	// うーん・・・
	for (IZ_UINT i = 0; i < TEX_STAGE_NUM; ++i) {
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
IZ_BOOL CGraphicsDevice::SaveRenderState()
{
	return m_RenderState.Save();
}

/**
* レンダーステート復帰
*/
IZ_BOOL CGraphicsDevice::LoadRenderState()
{
	return m_RenderState.Load(this);
}

/**
* レンダーステート一括セット
*/
void CGraphicsDevice::SetRenderState(const S_RENDER_STATE& sRS)
{
	SetViewport(sRS.vp);

	for (IZ_UINT i = 0; i < E_GRAPH_RS_NUM; ++i) {
		SetRenderState(
			static_cast<E_GRAPH_RENDER_STATE>(i),
			sRS.dwRS[i]);
	}

	// シザー矩形
	SetScissorTestRect(sRS.rcScissor);
}
