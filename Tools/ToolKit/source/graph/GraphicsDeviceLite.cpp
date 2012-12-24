#include "graph/GraphicsDeviceLite.h"
#include "graph/TextureLite.h"

using namespace izanagi;
using namespace tool;

CGraphicsDeviceLite* CGraphicsDeviceLite::s_pInstance = IZ_NULL;
izanagi::tool::CSimpleMemoryAllocator CGraphicsDeviceLite::s_cAllocator;

/**
* インスタンス作成
*/
CGraphicsDeviceLite* CGraphicsDeviceLite::CreateGraphicsDeviceLight(HWND hWnd)
{
    if (s_pInstance != IZ_NULL) {
        return s_pInstance;
    }

    IZ_ASSERT(hWnd != IZ_NULL);

    IMemoryAllocator* pAllocator = &s_cAllocator;

    // メモリ確保
    IZ_BYTE* pBuf = (IZ_BYTE*)ALLOC_ZERO(pAllocator, sizeof(CGraphicsDeviceLite));
    IZ_BOOL result = (pBuf != IZ_NULL);
    VGOTO(result, __EXIT__);

    // インスタンス作成
    s_pInstance = new(pBuf) CGraphicsDeviceLite;
    s_pInstance->AddRef();
    
    // 初期化
    result = s_pInstance->Init(hWnd);
    VGOTO(result, __EXIT__);

__EXIT__:
    if (!result) {
        if (s_pInstance != IZ_NULL) {
            SAFE_RELEASE(s_pInstance);
        }
        else if (pBuf != IZ_NULL) {
            FREE(pAllocator, pBuf);
        }
    }

    return s_pInstance;
}

// コンストラクタ
CGraphicsDeviceLite::CGraphicsDeviceLite()
{
    m_pD3D = NULL;
    m_pDevice = NULL;
    memset(&m_sPresentParameters, 0, sizeof(m_sPresentParameters));
}

// デストラクタ
CGraphicsDeviceLite::~CGraphicsDeviceLite()
{
    SAFE_RELEASE(m_pD3D);
    SAFE_RELEASE(m_pDevice);
}

// 解放
void CGraphicsDeviceLite::InternalRelease()
{
    delete this;
    FREE(&s_cAllocator, this);
}

// 初期化
BOOL CGraphicsDeviceLite::Init(HWND hWnd)
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

        m_sPresentParameters.Windowed = TRUE;       // 画面モード(ウインドウモード)
        m_sPresentParameters.BackBufferWidth = 1;   // バックバッファの幅
        m_sPresentParameters.BackBufferHeight = 1;  // バックバッファの高さ

        m_sPresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;

        // Zバッファの自動作成
        m_sPresentParameters.EnableAutoDepthStencil = FALSE;
    }

    // 描画はしないので、ほぼ確実に作成できる方法をとる
    if (FAILED(m_pD3D->CreateDevice(
                D3DADAPTER_DEFAULT,
                D3DDEVTYPE_NULLREF,
                hWnd,
                D3DCREATE_PUREDEVICE | D3DCREATE_HARDWARE_VERTEXPROCESSING,
                &m_sPresentParameters,
                &m_pDevice)))
    {
        // 結局失敗した
        IZ_ASSERT(FALSE);
        ret = FALSE;
    }

    return ret;
}

/**
* テクスチャ読み込み
*/
CTextureLite* CGraphicsDeviceLite::CreateTextureFromFile(
    LPCSTR lpszPath,
    IZ_BOOL needNonPow2/*= IZ_FALSE*/)
{
    CTextureLite* ret = CTextureLite::CreateTextureFromFile(
                            this,
                            GetAllocator(),
                            lpszPath,
                            needNonPow2);
    
    return ret;
}

// テクスチャ作成
CTextureLite* CGraphicsDeviceLite::CreateTexture(
    IZ_UINT nWidth,
    IZ_UINT nHeight,
    D3DFORMAT fmt)
{
    CTextureLite* ret = CTextureLite::CreateTexture(
                            this,
                            GetAllocator(),
                            nWidth, nHeight,
                            fmt);
    
    return ret;
}

// テクスチャ作成
CTextureLite* CGraphicsDeviceLite::CreateTexture(
    IZ_UINT nWidth,
    IZ_UINT nHeight,
    izanagi::graph::E_GRAPH_PIXEL_FMT fmt)
{
    CTextureLite* ret = CTextureLite::CreateTexture(
                            this,
                            GetAllocator(),
                            nWidth, nHeight,
                            fmt);
    
    return ret;
}

// テクスチャ保存
IZ_BOOL CGraphicsDeviceLite::SaveTexture(
    IZ_PCSTR path,
    CTextureLite* texture,
    TextureExportType type)
{
    IZ_BOOL ret = CTextureLite::SaveTexture(path, texture, type);
    return ret;
}
