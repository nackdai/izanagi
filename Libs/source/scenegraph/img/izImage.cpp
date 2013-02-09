#include "scenegraph/img/izImage.h"
#include "izIo.h"

using namespace izanagi;

/**
* インスタンス作成
*/
CImage* CImage::CreateImage(
    IMemoryAllocator* pAllocator,
    graph::CGraphicsDevice* pDevice,
    IInputStream* pInputStream)
{
    IZ_ASSERT(pAllocator != IZ_NULL);
    IZ_ASSERT(pDevice != IZ_NULL);
    IZ_ASSERT(pInputStream != IZ_NULL);

    IZ_BYTE* pBuf = IZ_NULL;
    CImage* pInstance = IZ_NULL;

    // ヘッダ読み込み
    S_IMG_HEADER sHeader;
    IZ_BOOL result = IZ_INPUT_READ(pInputStream, &sHeader, 0, sizeof(sHeader));
    VGOTO(result, __EXIT__);

    // マジックナンバーチェック
    result = IS_IMG_FORMAT(sHeader.magic);
    VGOTO(result, __EXIT__);

    // 確保サイズ
    size_t nSize = sizeof(CImage) + sizeof(graph::CTexture*) * sHeader.numTextures;

    // メモリ確保
    pBuf = (IZ_BYTE*)ALLOC_ZERO(pAllocator, nSize);
    result = (pBuf != IZ_NULL);
    VGOTO(result, __EXIT__);

    // インスタンス作成
    pInstance = new(pBuf) CImage;
    {
        pBuf += sizeof(CImage);

        pInstance->AddRef();

        pInstance->m_Allocator = pAllocator;
        pInstance->m_pTexture = (graph::CBaseTexture**)pBuf;

        memcpy(&pInstance->m_Header, &sHeader, sizeof(sHeader));
    }

    // テクスチャ読み込み
    result = pInstance->ReadTexture(
                pDevice,
                pInputStream);
    VGOTO(result, __EXIT__);

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

// データサイズを計算
IZ_UINT CImage::ComputeSize(
    IZ_UINT pitch,
    IZ_UINT height,
    graph::E_GRAPH_PIXEL_FMT fmt)
{
    IZ_UINT ret = pitch * height;

    if (fmt == graph::E_GRAPH_PIXEL_FMT_DXT1) {
        ret = (pitch * height) >> 2;
    }
    else if (fmt == graph::E_GRAPH_PIXEL_FMT_DXT3
                || fmt == graph::E_GRAPH_PIXEL_FMT_DXT5)
    {
        ret = (pitch * height) >> 2;
    }

    return ret;
}

// データ読み込み
void CImage::ReadData(
    IZ_UINT8* dst,
    IZ_UINT nPitch,
    IZ_UINT nHeight,
    graph::E_GRAPH_PIXEL_FMT nFmt,
    IInputStream* input)
{
#if 0
    IZ_UINT restSize = ComputeSize(nPitch, nHeight, nFmt);;

    for (IZ_UINT y = 0; y < nHeight; y++)
    {
        IZ_UINT readSize = (nPitch < restSize ? nPitch : restSize);

        IZ_BOOL result = IZ_INPUT_READ(input, dst, 0, readSize);
        IZ_ASSERT(result);

        dst += readSize;
        restSize -= readSize;
    }
#else
	IZ_UINT readSize = ComputeSize(nPitch, nHeight, nFmt);
	IZ_BOOL result = IZ_INPUT_READ(input, dst, 0, readSize);
    IZ_ASSERT(result);
#endif
}

// テクスチャ作成
graph::CBaseTexture* CImage::CreatePlaneTexture(
    graph::CGraphicsDevice* pDevice,
    IZ_UINT nWidth,
    IZ_UINT nHeight,
    IZ_UINT nMipLevel,
    graph::E_GRAPH_PIXEL_FMT nFmt,
    IInputStream* input)
{
    // NOTE
    // データセットするためにロックをしたいので、
    // 動的にテクスチャを作成する

    // テクスチャ作成
    graph::CTexture* pTex = IZ_NULL;
    pTex = pDevice->CreateTexture(
            nWidth,
            nHeight,
            nMipLevel,
            nFmt,
            graph::E_GRAPH_RSC_TYPE_STATIC);

    VRETURN_NULL(pTex != IZ_NULL);

    IZ_BOOL result = IZ_FALSE;

    for (IZ_UINT nLevel = 0; nLevel < nMipLevel; ++nLevel) {
        // データセット
        IZ_BYTE* data = IZ_NULL;
        IZ_UINT nPitch = pTex->Lock(
                            nLevel, 
                            reinterpret_cast<void**>(&data),
                            IZ_FALSE);

        result = (nPitch > 0);
        VGOTO(result, __EXIT__);

        ReadData(
            reinterpret_cast<IZ_UINT8*>(data),
            nPitch,
            nHeight,
            nFmt,
            input);

        pTex->Unlock(nLevel);

        nWidth >>= 1;
        nHeight >>= 1;
    }

__EXIT__:
    if (!result) {
        SAFE_RELEASE(pTex);
    }
    return pTex;
}

// キューブテクスチャ作成
graph::CBaseTexture* CImage::CreateCubeTexture(
    graph::CGraphicsDevice* pDevice,
    IZ_UINT nWidth,
    IZ_UINT nHeight,
    IZ_UINT nMipLevel,
    graph::E_GRAPH_PIXEL_FMT nFmt,
    IInputStream* input)
{
    // テクスチャ作成
    graph::CCubeTexture* pTex = IZ_NULL;
    pTex = pDevice->CreateCubeTexture(
            nWidth,
            nHeight,
            nMipLevel,
            nFmt,
            IZ_TRUE);

    VRETURN_NULL(pTex != IZ_NULL);

    IZ_BOOL result = IZ_FALSE;

    for (IZ_UINT nFace = 0; nFace < graph::E_GRAPH_CUBE_TEX_FACE_NUM; ++nFace) {
        IZ_UINT nW = nWidth;
        IZ_UINT nH = nHeight;

        for (IZ_UINT nLevel = 0; nLevel < nMipLevel; ++nLevel) {
            graph::E_GRAPH_CUBE_TEX_FACE face = static_cast<graph::E_GRAPH_CUBE_TEX_FACE>(nFace);

            // データセット
            IZ_BYTE* data = IZ_NULL;
            IZ_UINT nPitch = pTex->Lock(
                                face,
                                nLevel, 
                                reinterpret_cast<void**>(&data),
                                IZ_FALSE);

            result = (nPitch > 0);
            VGOTO(result, __EXIT__);

            ReadData(
                reinterpret_cast<IZ_UINT8*>(data),
                nPitch,
                nHeight,
                nFmt,
                input);

            pTex->Unlock(face, nLevel);

            nW >>= 1;
            nH >>= 1;
        }
    }

__EXIT__:
    if (!result) {
        SAFE_RELEASE(pTex);
    }
    return pTex;
}

// テクスチャ作成
graph::CBaseTexture* CImage::CreateTexture(
    graph::CGraphicsDevice* pDevice,
    const S_IMG_TEX_HEADER& sTexHeader,
    IInputStream* input)
{
    IZ_ASSERT(pDevice != IZ_NULL);

    IZ_UINT nWidth = 1 << sTexHeader.w;
    IZ_UINT nHeight = 1 << sTexHeader.h;

    IZ_UINT nMipLevel = sTexHeader.level;

    graph::E_GRAPH_PIXEL_FMT nFmt = sTexHeader.fmt;

    graph::E_GRAPH_TEX_TYPE nType = static_cast<graph::E_GRAPH_TEX_TYPE>(sTexHeader.type);

    // テクスチャ作成関数テーブル
    typedef graph::CBaseTexture* (*CreateTexFunc)(graph::CGraphicsDevice*, IZ_UINT, IZ_UINT, IZ_UINT, graph::E_GRAPH_PIXEL_FMT, IInputStream*);
    static CreateTexFunc FuncTbl[] = {
        CImage::CreatePlaneTexture,
        CImage::CreateCubeTexture,
        IZ_NULL,
    };
    IZ_C_ASSERT(COUNTOF(FuncTbl) == graph::E_GRAPH_TEX_TYPE_NUM);

    IZ_ASSERT(FuncTbl[nType] != IZ_NULL);

    graph::CBaseTexture* pTex = IZ_NULL;

    // テクスチャ作成
    pTex = (*FuncTbl[nType])(
                pDevice,
                nWidth,
                nHeight,
                nMipLevel,
                nFmt,
                input);

    IZ_ASSERT(pTex != IZ_NULL);
    return pTex;
}

// テクスチャ読み込み
IZ_BOOL CImage::ReadTexture(
    graph::CGraphicsDevice* pDevice,
    IInputStream* pInputStream)
{
    IZ_ASSERT(m_Allocator != IZ_NULL);
    IZ_ASSERT(pDevice != IZ_NULL);
    IZ_ASSERT(pInputStream != IZ_NULL);

    // ジャンプテーブル分飛ばす
    IZ_BOOL ret = pInputStream->Seek(m_Header.numTextures * sizeof(IZ_UINT), E_IO_STREAM_SEEK_POS_CUR);
    VGOTO(ret, __EXIT__);

    S_IMG_TEX_HEADER sTexHeader;

    // 読み込み
    for (IZ_UINT i = 0; i < m_Header.numTextures; ++i) {
        // ヘッダ読み込み
        ret = IZ_INPUT_READ(pInputStream, &sTexHeader, 0, sizeof(sTexHeader));
        VGOTO(ret, __EXIT__);

        // テクスチャ作成
        m_pTexture[i] = CreateTexture(
                            pDevice,
                            sTexHeader,
                            pInputStream);
        ret = (m_pTexture[i] != IZ_NULL);
        VGOTO(ret, __EXIT__);

        // パラメータセット
        m_pTexture[i]->SetAddress(
            static_cast<graph::E_GRAPH_TEX_ADDRESS>(sTexHeader.addressU),
            static_cast<graph::E_GRAPH_TEX_ADDRESS>(sTexHeader.addressV));
        m_pTexture[i]->SetFilter(
            static_cast<graph::E_GRAPH_TEX_FILTER>(sTexHeader.minFilter),
            static_cast<graph::E_GRAPH_TEX_FILTER>(sTexHeader.magFilter),
            static_cast<graph::E_GRAPH_TEX_FILTER>(sTexHeader.mipFilter));
    }

__EXIT__:
    return ret;
}
