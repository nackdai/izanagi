#include <stdafx.h>
#include "GraphicsDevice.h"
#include "Option.h"

CGraphicsDevice CGraphicsDevice::s_cInstance;

CGraphicsDevice::CGraphicsDevice()
{
	m_pD3D = NULL;
	m_pDevice = NULL;
	memset(&m_sPresentParameters, 0, sizeof(m_sPresentParameters));
}

CGraphicsDevice::~CGraphicsDevice()
{
	Release();
}

// 開放
void CGraphicsDevice::Release()
{
	SAFE_RELEASE(m_pD3D);
	SAFE_RELEASE(m_pDevice);
}

///////////////////////////////////////////////////////////
// For FontConverter

// 初期化
BOOL CGraphicsDevice::Init(HWND hWnd)
{
	if (m_pDevice != NULL) {
		return TRUE;
	}

	BOOL ret = TRUE;

	HRESULT hr;

	// IDirect3D9 インターフェースの取得
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	VRETURN(m_pD3D != NULL);

	// 現在の画面モードの取得
	D3DDISPLAYMODE d3ddm;
	hr = m_pD3D->GetAdapterDisplayMode(
			D3DADAPTER_DEFAULT,
			&d3ddm);
	VRETURN(SUCCEEDED(hr));

	// D3DPRESENT_PARAMETERS の設定
	{
		ZeroMemory(&m_sPresentParameters, sizeof(m_sPresentParameters));

		m_sPresentParameters.BackBufferCount = 1;

		m_sPresentParameters.BackBufferFormat = d3ddm.Format;

		m_sPresentParameters.Windowed = TRUE;		// 画面モード(ウインドウモード)
		m_sPresentParameters.BackBufferWidth = 1;	// バックバッファの幅
		m_sPresentParameters.BackBufferHeight = 1;	// バックバッファの高さ

		m_sPresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;

		// Zバッファの自動作成
		m_sPresentParameters.EnableAutoDepthStencil = FALSE;
	}

	if (FAILED(m_pD3D->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_NULLREF,
				hWnd,
				D3DCREATE_PUREDEVICE | D3DCREATE_HARDWARE_VERTEXPROCESSING,
				&m_sPresentParameters,
				&m_pDevice)))
	{
		// 結局失敗した
		ASSERT(FALSE);
		ret = FALSE;
    }

	return ret;
}

// テクスチャ作成
BOOL CGraphicsDevice::CreateTexture(
	UINT nWidth, UINT nHeight,
	D3DFORMAT fmt,
	LPDIRECT3DTEXTURE9* pTex,
	BOOL bIsRT/*= FALSE*/)
{
	HRESULT hr;

	if (bIsRT) {
		hr = m_pDevice->CreateTexture(
				nWidth, nHeight, 1,
				D3DUSAGE_RENDERTARGET,
				fmt,
				D3DPOOL_DEFAULT,
				pTex,
				NULL);
	}
	else {
		hr = m_pDevice->CreateTexture(
				nWidth, nHeight, 1,
				0,
				fmt,
				D3DPOOL_SYSTEMMEM,
				pTex,
				NULL);
	}

	ASSERT(SUCCEEDED(hr));
	return SUCCEEDED(hr);
}

void CGraphicsDevice::BeginScene()
{
	ASSERT(m_pDevice != NULL);

	m_pDevice->BeginScene();

	// レンダーステートも
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);

	// MSAA有効
	//m_pDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
	//m_pDevice->SetRenderState(D3DRS_MULTISAMPLEMASK, 0xffffffff);
}

void CGraphicsDevice::EndScene()
{
	ASSERT(m_pDevice != NULL);

	m_pDevice->EndScene();

	m_pDevice->Present(
		NULL,
		NULL,
		NULL,
		NULL);
}

void CGraphicsDevice::Clear(DWORD color)
{
	m_pDevice->Clear(
		0,
		NULL,
		D3DCLEAR_TARGET,
		color,
		1.0f,
		0);
}

BOOL CGraphicsDevice::SetRenderTarget(LPDIRECT3DSURFACE9 pSurf)
{
	HRESULT hr = m_pDevice->SetRenderTarget(0, pSurf);
	ASSERT(SUCCEEDED(hr));

	return SUCCEEDED(hr);
}

// レンダーターゲット作成
BOOL CGraphicsDevice::CreateRenderTarget(
	UINT nWidth, UINT nHeight,
	D3DFORMAT fmt,
	LPDIRECT3DSURFACE9* pSurf,
	INT nMSAANumber/*= 0*/)
{
	ASSERT(m_pDevice != NULL);

	// MSAAのサンプリング数は０ or ２〜１６
	if ((nMSAANumber != 0)
		&& ((2 > nMSAANumber) || (nMSAANumber > 16)))
	{
		ASSERT(FALSE);
		return FALSE;
	}

	D3DMULTISAMPLE_TYPE MultiSampleType = D3DMULTISAMPLE_NONE;
	DWORD MultiSampleQuality = 0;

	if (nMSAANumber > 0) {
		HRESULT hr;
		for (INT n = nMSAANumber; n >= 0; n--) {
			MultiSampleType = (D3DMULTISAMPLE_TYPE)(D3DMULTISAMPLE_NONE + n);

			hr = m_pD3D->CheckDeviceMultiSampleType(
					D3DADAPTER_DEFAULT, 
					D3DDEVTYPE_HAL,
					fmt,
					m_sPresentParameters.Windowed,
					MultiSampleType,
					&MultiSampleQuality);

			if (SUCCEEDED(hr)) {
				break;
			}
		}

		VRETURN(SUCCEEDED(hr));

		MultiSampleQuality = (MultiSampleQuality > 0 ? MultiSampleQuality - 1 : 0);
	}

	// TODO
	HRESULT hr = m_pDevice->CreateRenderTarget(
					nWidth,
					nHeight,
					fmt,
					MultiSampleType,
					MultiSampleQuality,
					FALSE,	// Lockしない
					pSurf,
					NULL);

	ASSERT(SUCCEEDED(hr));
	return SUCCEEDED(hr);
}

// フォント作成
BOOL CGraphicsDevice::CreateFont(
	LPD3DXFONT* pFont,
	const SOption& sOption)
{
	ASSERT(m_pDevice != NULL);

	BOOL ret = FALSE;

	// TODO
	// localeの設定
	//::setlocale(LC_ALL, _T("Japanese"));

	// TODO
	// フォント作成
	HRESULT hr = D3DXCreateFont(
					m_pDevice,
					sOption.fontSize,	// 高さ
					0,					// 幅
					sOption.isBold ? FW_BOLD : FW_NORMAL,	// 太さ
					1,										// ミップマップレベル
					sOption.isItalic,						// 斜体
					DEFAULT_CHARSET,						// キャラクタセット
					OUT_TT_ONLY_PRECIS,						// 出力精度
					CLEARTYPE_QUALITY,						// 品質
					DEFAULT_PITCH | FF_DONTCARE,			// ピッチ
					sOption.fontFace,
					pFont);

	ASSERT(SUCCEEDED(hr));
	return SUCCEEDED(hr);
}
