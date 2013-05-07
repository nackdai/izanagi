#include "graph/TextureLite.h"
#include "graph/GraphicsDeviceLite.h"

// 力技だが・・・
#include "graph/ParamValueConverter.h"

using namespace izanagi;
using namespace tool;

namespace {
    // くっ・・・・
    D3DFORMAT PixelFormatTbl[] = {
        D3DFMT_A8B8G8R8,
        D3DFMT_A8R8G8B8,
        D3DFMT_A4R4G4B4,
        D3DFMT_A2R10G10B10,
        D3DFMT_A8,
        D3DFMT_R32F,
        D3DFMT_A16B16G16R16F,
        D3DFMT_A32B32G32R32F,
        D3DFMT_DXT1,
        D3DFMT_DXT3,
        D3DFMT_DXT5,
        D3DFMT_X8B8G8R8,
        D3DFMT_D24S8,
    };
    IZ_C_ASSERT(COUNTOF(PixelFormatTbl) == izanagi::graph::E_GRAPH_PIXEL_FMT_NUM);

    inline izanagi::graph::E_GRAPH_PIXEL_FMT _GetPixelFmt(D3DFORMAT fmt)
    {
        IZ_INT ret = 0;
        for (IZ_UINT i = 0; i < COUNTOF(PixelFormatTbl); i++) {
            if (PixelFormatTbl[i] == fmt) {
                ret = i;
                break;
            }
        }
        IZ_ASSERT(ret >= 0);
        return static_cast<izanagi::graph::E_GRAPH_PIXEL_FMT>(ret);
    }

    inline D3DFORMAT _GetDXPixelFmt(izanagi::graph::E_GRAPH_PIXEL_FMT fmt)
    {
        IZ_ASSERT(fmt < COUNTOF(PixelFormatTbl));

        if (fmt < COUNTOF(PixelFormatTbl)) {
            return PixelFormatTbl[fmt];
        }

        return D3DFMT_UNKNOWN;
    }

    inline IZ_BOOL _IsValidDXPixelFmt(D3DFORMAT fmt)
    {
        for (IZ_UINT i = 0; i < COUNTOF(PixelFormatTbl); i++) {
            if (PixelFormatTbl[i] == fmt) {
                return IZ_TRUE;
            }
        }
        return IZ_FALSE;
    }
}   // namespace

// ファイルからテクスチャ作成
CTextureLite* CTextureLite::CreateTextureFromFile(
    CGraphicsDeviceLite* pDevice,
    IMemoryAllocator* pAllocator,
    IZ_PCSTR lpszPath,
    IZ_BOOL needNonPow2/*= IZ_FALSE*/)
{
    IZ_ASSERT(pDevice != IZ_NULL);
    IZ_ASSERT(pAllocator != IZ_NULL);

    IZ_BYTE* pBuf = IZ_NULL;
    CTextureLite* pInstance = IZ_NULL;

    // テクスチャ情報取得
    D3DXIMAGE_INFO sImageInfo;

    HRESULT hr = D3DXGetImageInfoFromFile(
                    lpszPath,
                    &sImageInfo);
    IZ_BOOL result = SUCCEEDED(hr);
    VGOTO(result, __EXIT__);

    // メモリ確保
    pBuf = (IZ_BYTE*)ALLOC_ZERO(pAllocator, sizeof(CTextureLite));
    result = (pBuf != IZ_NULL);
    VGOTO(result, __EXIT__);

    // インスタンス作成
    pInstance = new(pBuf) CTextureLite;
    {
        pInstance->AddRef();
        pInstance->m_pAllocator = pAllocator;

        LPDIRECT3DDEVICE9 pD3DDevice = pDevice->GetRawInterface();

        // NOTE
        // 不明なピクセルフォーマットは強制的にRGBA8にする
        D3DFORMAT fmt = (_IsValidDXPixelFmt(sImageInfo.Format)
                            ? D3DFMT_FROM_FILE
                            : D3DFMT_A8B8G8R8);

        UINT sizeType = (needNonPow2 ? D3DX_DEFAULT_NONPOW2 : D3DX_DEFAULT);

        // テクスチャ読み込み
        // NOTE
        // Width、Heightは２のべき乗にして読み込む
        HRESULT hr = D3DXCreateTextureFromFileEx(
                        pD3DDevice,
                        lpszPath,
                        sizeType,           // Width
                        sizeType,           // Height
                        D3DX_FROM_FILE,     // MipLevels,
                        0,                  // Usage
                        fmt,                // Format
                        D3DPOOL_SYSTEMMEM,  // Pool,
                        D3DX_DEFAULT,       // Filter
                        D3DX_DEFAULT,       // MipFilter
                        0,
                        NULL,
                        NULL,
                        &pInstance->m_pTexture);
        result = SUCCEEDED(hr);
        VGOTO(result, __EXIT__);

        // テクスチャ情報取得
        pInstance->m_pTexture->GetLevelDesc(0, &pInstance->m_Desc);
        pInstance->m_nMipLevels = pInstance->m_pTexture->GetLevelCount();
        pInstance->m_nFmt = _GetPixelFmt(pInstance->m_Desc.Format);
    }
    
__EXIT__:
    if (!result) {
        if (pInstance != IZ_NULL) {
            SAFE_RELEASE(pInstance);
        }
        else if (pBuf != IZ_NULL) {
            FREE(pAllocator, pBuf);
        }
    }
    return pInstance;
}

/**
* テクスチャ作成
*/
CTextureLite* CTextureLite::CreateTexture(
    CGraphicsDeviceLite* pDevice,
    IMemoryAllocator* pAllocator,
    IZ_UINT nWidth,
    IZ_UINT nHeight,
    D3DFORMAT fmt)
{
    IZ_ASSERT(pDevice != IZ_NULL);
    IZ_ASSERT(pAllocator != IZ_NULL);

    IZ_BYTE* pBuf = IZ_NULL;
    CTextureLite* pInstance = IZ_NULL;

    // メモリ確保
    pBuf = (IZ_BYTE*)ALLOC_ZERO(pAllocator, sizeof(CTextureLite));
    IZ_BOOL result = (pBuf != IZ_NULL);
    VGOTO(result, __EXIT__);

    // インスタンス作成
    pInstance = new(pBuf) CTextureLite;
    {
        pInstance->AddRef();
        pInstance->m_pAllocator = pAllocator;

        LPDIRECT3DDEVICE9 pD3DDevice = pDevice->GetRawInterface();

        // NOTE
        // 不明なピクセルフォーマットは強制的にRGBA8にする
        fmt = (_IsValidDXPixelFmt(fmt)
                ? fmt
                : D3DFMT_A8R8G8B8);

        HRESULT hr = D3DXCreateTexture(
                        pD3DDevice,
                        nWidth,             // width
                        nHeight,            // height
                        1,                  // mipmap
                        0,                  // Usage
                        fmt,                // Format
                        D3DPOOL_SYSTEMMEM,  // Pool
                        &pInstance->m_pTexture);
        result = SUCCEEDED(hr);
        VGOTO(result, __EXIT__);

        // テクスチャ情報取得
        pInstance->m_pTexture->GetLevelDesc(0, &pInstance->m_Desc);
        pInstance->m_nMipLevels = pInstance->m_pTexture->GetLevelCount();
        pInstance->m_nFmt = _GetPixelFmt(pInstance->m_Desc.Format);
    }
    
__EXIT__:
    if (!result) {
        if (pInstance != IZ_NULL) {
            SAFE_RELEASE(pInstance);
        }
        else if (pBuf != IZ_NULL) {
            FREE(pAllocator, pBuf);
        }
    }
    return pInstance;
}

// テクスチャ作成
CTextureLite* CTextureLite::CreateTexture(
    CGraphicsDeviceLite* pDevice,
    IMemoryAllocator* pAllocator,
    IZ_UINT nWidth,
    IZ_UINT nHeight,
    izanagi::graph::E_GRAPH_PIXEL_FMT fmt)
{
    D3DFORMAT dxFmt = _GetDXPixelFmt(fmt);
    VRETURN(dxFmt != D3DFMT_UNKNOWN);

    CTextureLite* ret = CreateTexture(
                            pDevice,
                            pAllocator,
                            nWidth,
                            nHeight,
                            dxFmt);
    return ret;
}


// テクスチャ保存
IZ_BOOL CTextureLite::SaveTexture(
    IZ_PCSTR path,
    CTextureLite* texture,
    TextureExportType type)
{
    IZ_ASSERT(path != IZ_NULL);
    IZ_ASSERT(texture != IZ_NULL);

    D3DXIMAGE_FILEFORMAT format = D3DXIFF_PNG;
    switch (type) {
    case TextureExportTypePNG:
        format = D3DXIFF_PNG;
        break;
    case TextureExportTypeHDR:
        format = D3DXIFF_HDR;
        break;
    default:
        format = D3DXIFF_PNG;
        break;
    }

    HRESULT hr = D3DXSaveTextureToFile(
                    path,
                    format,
                    texture->m_pTexture,
                    NULL);
    
    IZ_BOOL ret = SUCCEEDED(hr);
    return ret;
}

/**
* ロック
*/
IZ_UINT CTextureLite::Lock(
    UINT nLevel,
    void** data)
{
    IZ_ASSERT(m_pTexture != IZ_NULL);

    if (nLevel >= m_nMipLevels) {
        // レベル指定オーバー
        IZ_ASSERT(IZ_FALSE);
        return 0;
    }

    D3DLOCKED_RECT rect;
    HRESULT hr = m_pTexture->LockRect(
                    nLevel,
                    &rect,
                    NULL,
                    D3DLOCK_READONLY);
    IZ_ASSERT(SUCCEEDED(hr));

    UINT ret = 0;

    if (SUCCEEDED(hr)) {
        *data = rect.pBits;
        ret = rect.Pitch;
    }

    return ret;
}

/**
* アンロック
*/
IZ_BOOL CTextureLite::Unlock(IZ_UINT nLevel)
{
    IZ_ASSERT(m_pTexture != IZ_NULL);

    HRESULT hr = m_pTexture->UnlockRect(nLevel);
    IZ_BOOL ret = SUCCEEDED(hr);
    IZ_ASSERT(ret);

    return ret;
}
