#include "ImageBuilderImpl.h"
#include "ImageReaderImpl.h"
#include "izImageLib.h"
#include "AppError.h"
#include "izToolKit.h"

CImageBuilder CImageBuilder::s_cInstance;

void CImageBuilder::startDocument()
{
    m_State = StateNormal;
    m_IsStartRoot = IZ_FALSE;
}

void CImageBuilder::endDeocument()
{
}

namespace attr_type {
    static const char* ROOT = "textures";

    static const char* PLANE   = "plane";
    static const char* CUBE    = "cube";
    static const char* VOLUME  = "volume";

    static const char* ELEMENT = "element";

    static const char* PATH       = "path";
    static const char* FMT        = "fmt";
    static const char* MIPMAP     = "mipmap";
    static const char* ADDR_U     = "addrU";
    static const char* ADDR_V     = "addrV";
    static const char* MIN_FILTER = "filterMin";
    static const char* MAG_FILTER = "filterMag";
    static const char* MIP_FILTER = "filterMip";
    static const char* ANISO_NUM  = "numAniso";

    static const char* FACE       = "face";

    static const char* TEX_TYPE   = "type";

    static const char* TEX_IDX    = "texIdx";
}   // namespace attr_type

void CImageBuilder::startElement(
    const XMLCh* const uri, 
    const XMLCh* const localname, 
    const XMLCh* const qname, 
    const xercesc::Attributes& attrs)
{
    izanagi::tool::CString name(XN(qname));

    if (name == attr_type::ROOT) {
        m_IsStartRoot = IZ_TRUE;
        m_State = StateNormal;
    }
    else {
        // TODO
        IZ_ASSERT(m_IsStartRoot);

        if (name == attr_type::CUBE) {
            m_State = StateCube;
            SetCubeAttrs(attrs);
        }
        else if (name == attr_type::VOLUME) {
            // TODO
        }
        else if (name == attr_type::PLANE) {
            SetPlaneAttrs(attrs);
        }
        else if (name == attr_type::ELEMENT) {
            // NOTE
            // CubeMapのような複数のテクスチャを明示的に指定するときの要素
            if (m_State == StateCube) {
                if (m_ImageInfoList.rbegin() != m_ImageInfoList.rend()) {
                    SImageInfo& imageInfo = *m_ImageInfoList.rbegin();
                    SetElementAttr(&imageInfo, attrs);
                }
            }
        }
    }
}

void CImageBuilder::endElement(
    const XMLCh* const uri,
    const XMLCh* const localname,
    const XMLCh* const qname)
{
    izanagi::tool::CString name(XN(qname));

    if (name == attr_type::CUBE
        || name == attr_type::VOLUME)
    {
        m_State = StateNormal;
    }
}

// 属性セット
void CImageBuilder::SetPlaneAttrs(const xercesc::Attributes& attrs)
{
    SImageInfo sImageInfo;
    sImageInfo.info.type = izanagi::graph::E_GRAPH_TEX_TYPE_PLANE;

    SetCommonAttrs(&sImageInfo, attrs);
    SetElementAttr(&sImageInfo, attrs);

    // TODO
    // IsValid

    m_ImageInfoList.push_back(sImageInfo);
}

// 属性セット
void CImageBuilder::SetCubeAttrs(const xercesc::Attributes& attrs)
{
    SImageInfo sImageInfo;
    sImageInfo.info.type = izanagi::graph::E_GRAPH_TEX_TYPE_CUBE;

    SetCommonAttrs(&sImageInfo, attrs);

    m_ImageInfoList.push_back(sImageInfo);
}

namespace {
    // 比較方法
    enum CMP_TYPE {
        CMP_TYPE_LOWER = 0,
        CMP_TYPE_UPPER,
        CMP_TYPE_NORMAL,

        CMP_TYPE_NUM,
    };

    inline void _ConvString(izanagi::tool::CString& str, CMP_TYPE typeCmp)
    {
        switch (typeCmp) {
        case CMP_TYPE_LOWER:
            str.make_lower();
            break;
        case CMP_TYPE_UPPER:
            str.make_upper();
            break;
        case CMP_TYPE_NORMAL:
            break;
        }
    }

    template <typename _T>
    _T _ConvString(
        LPCSTR lpszStr,
        LPCSTR lpszTable[],
        UINT nTableNum,
        CMP_TYPE typeCmp)
    {
        _T ret = (_T)0;

        izanagi::tool::CString str(lpszStr);
        _ConvString(str, typeCmp);

        size_t nStrLen = strlen(lpszStr);

        for (UINT i = 0; i < nTableNum; i++) {
            izanagi::tool::CString tmp(lpszTable[i]);
            _ConvString(tmp, typeCmp);

            if (str == tmp) {
                ret = static_cast<_T>(i);
                break;
            }
        }

        return ret;
    }

    // 文字列 -> PixelFormat
    inline izanagi::graph::E_GRAPH_PIXEL_FMT _ConvTextToPixelFormat(LPCSTR lpszStr)
    {
        static const char* str[] = {
            "rgba8",    // graph::E_GRAPH_PIXEL_FMT_RGBA8
            "bgra8",    // graph::E_GRAPH_PIXEL_FMT_BGRA8

            "rgba4",    // graph::E_GRAPH_PIXEL_FMT_RGBA4
            "rgb10A2",  // graph::E_GRAPH_PIXEL_FMT_RGB10A2
            "a8",       // graph::E_GRAPH_PIXEL_FMT_A8
            
            "r32f",     // graph::E_GRAPH_PIXEL_FMT_R32F
            "rgba16f",  // graph::E_GRAPH_PIXEL_FMT_RGBA16F
            "rgba32f",  // graph::E_GRAPH_PIXEL_FMT_RGBA32F

            "dxt1",     // graph::E_GRAPH_PIXEL_FMT_DXT1
            "dxt3",     // graph::E_GRAPH_PIXEL_FMT_DXT3
            "dxt5",     // graph::E_GRAPH_PIXEL_FMT_DXT5

            "rgbx8",    // graph::E_GRAPH_PIXEL_FMT_RGBX8
            "d24s8",    // graph::E_GRAPH_PIXEL_FMT_D24S8

            // Only for OpenGL...
            "rgba32ui", // E_GRAPH_PIXEL_FMT_RGBA32UI,
            "rgb32ui",  // E_GRAPH_PIXEL_FMT_RGB32UI,
        };
        C_ASSERT(COUNTOF(str) == izanagi::graph::E_GRAPH_PIXEL_FMT_NUM);

        izanagi::graph::E_GRAPH_PIXEL_FMT ret = _ConvString<izanagi::graph::E_GRAPH_PIXEL_FMT>(
                                            lpszStr,
                                            str,
                                            COUNTOF(str),
                                            CMP_TYPE_LOWER);
        return ret;
    }

    // 文字列 -> TexAddress
    inline izanagi::graph::E_GRAPH_TEX_ADDRESS _ConvTextToTexAddr(LPCSTR lpszStr)
    {
        static const char* str[] = {
            "wrap",         // graph::E_GRAPH_TEX_ADDRESS_WRAP
            "mirror",       // graph::E_GRAPH_TEX_ADDRESS_MIRROR
            "clamp",        // graph::E_GRAPH_TEX_ADDRESS_CLAMP
#if 0
            "border",       // graph::E_GRAPH_TEX_ADDRESS_BORDER
            "mirroronce",   // graph::E_GRAPH_TEX_ADDRESS_MIRRORONCE
#endif
        };
        C_ASSERT(COUNTOF(str) == izanagi::graph::E_GRAPH_TEX_ADDRESS_NUM);

        izanagi::graph::E_GRAPH_TEX_ADDRESS ret = _ConvString<izanagi::graph::E_GRAPH_TEX_ADDRESS>(
                                            lpszStr,
                                            str,
                                            COUNTOF(str),
                                            CMP_TYPE_LOWER);
        return ret;
    }

    // 文字列 -> TexFilter
    inline izanagi::graph::E_GRAPH_TEX_FILTER _ConvTextToTexFilter(LPCSTR lpszStr)
    {
        static const char* str[] = {
            "none",         // graph::E_GRAPH_TEX_FILTER_NONE
            "point",        // graph::E_GRAPH_TEX_FILTER_POINT
            "linear",       // graph::E_GRAPH_TEX_FILTER_LINEAR
            "aniso",        // graph::E_GRAPH_TEX_FILTER_ANISOTROPIC
#if 0
            "pyramidquad",  // graph::E_GRAPH_TEX_FILTER_PYRAMIDALQUAD
            "gaussquad",    // graph::E_GRAPH_TEX_FILTER_GAUSSIANQUAD
#endif
        };
        C_ASSERT(COUNTOF(str) == izanagi::graph::E_GRAPH_TEX_FILTER_NUM);

        izanagi::graph::E_GRAPH_TEX_FILTER ret = _ConvString<izanagi::graph::E_GRAPH_TEX_FILTER>(
                                            lpszStr,
                                            str,
                                            COUNTOF(str),
                                            CMP_TYPE_LOWER);
        return ret;
    }

    // 文字列 -> TexType
    inline izanagi::graph::E_GRAPH_TEX_TYPE _ConvTextToTexType(LPCSTR lpszStr)
    {
        static const char* str[] = {
            "plane",
            "cube",
            "volume",
        };
        C_ASSERT(COUNTOF(str) == izanagi::graph::E_GRAPH_TEX_TYPE_NUM);

        izanagi::graph::E_GRAPH_TEX_TYPE ret = _ConvString<izanagi::graph::E_GRAPH_TEX_TYPE>(
                                            lpszStr,
                                            str,
                                            COUNTOF(str),
                                            CMP_TYPE_LOWER);
        return ret;
    }
}   // namespace

// 共通属性セット
void CImageBuilder::SetCommonAttrs(
    SImageInfo* pImageInfo,
    const xercesc::Attributes& attrs)
{
    UINT nAttrNum = (UINT)attrs.getLength();

    for (UINT i = 0; i < nAttrNum; i++) {
        izanagi::tool::CString strAttrName(XN(attrs.getQName(i)));

        izanagi::tool::CString val(XN(attrs.getValue(i)));

        // 小文字にする
        val = val.make_lower();

        if (strAttrName == attr_type::FMT) {
            // fmt
            pImageInfo->info.fmt = _ConvTextToPixelFormat(val);
        }
        else if (strAttrName == attr_type::MIPMAP) {
            // mipmap
            pImageInfo->info.level = atoi(val);
            if (pImageInfo->info.level == 0) {
                // ０はダメ
                pImageInfo->info.level = 1;
            }
        }
        else if (strAttrName == attr_type::ADDR_U) {
            // addrU
            pImageInfo->info.addressU = _ConvTextToTexAddr(val);
        }
        else if (strAttrName == attr_type::ADDR_V) {
            // addrV
            pImageInfo->info.addressV = _ConvTextToTexAddr(val);
        }
        else if (strAttrName == attr_type::MIN_FILTER) {
            // filterMin
            pImageInfo->info.minFilter = _ConvTextToTexFilter(val);
        }
        else if (strAttrName == attr_type::MAG_FILTER) {
            // filterMag
            pImageInfo->info.magFilter = _ConvTextToTexFilter(val);
        }
        else if (strAttrName == attr_type::MIP_FILTER) {
            // filterMip
            pImageInfo->info.mipFilter = _ConvTextToTexFilter(val);
        }
        else if (strAttrName == attr_type::ANISO_NUM) {
            // numAniso
            pImageInfo->info.anisoNum = atoi(val);
        }
        else if (strAttrName == attr_type::TEX_TYPE) {
            // type
            pImageInfo->info.type = _ConvTextToTexType(val);
        }
    }
}

namespace {
    // 文字列 -> CubeFace
    inline izanagi::graph::E_GRAPH_CUBE_TEX_FACE _ConvTextToCubeFace(LPCSTR lpszStr)
    {
        static const char* str[] = {
            "xp", "xn",
            "yp", "yn",
            "zp", "zn",
        };
        C_ASSERT(COUNTOF(str) == izanagi::graph::E_GRAPH_CUBE_TEX_FACE_NUM);

        izanagi::graph::E_GRAPH_CUBE_TEX_FACE ret = _ConvString<izanagi::graph::E_GRAPH_CUBE_TEX_FACE>(
                                                lpszStr,
                                                str,
                                                COUNTOF(str),
                                                CMP_TYPE_LOWER);
        return ret;
    }
}   // namespace

void CImageBuilder::SetElementAttr(
    SImageInfo* imageInfo,
    const xercesc::Attributes& attrs)
{
    UINT nAttrNum = (UINT)attrs.getLength();

    SImageElement imgElement;

    for (UINT i = 0; i < nAttrNum; i++) {
        izanagi::tool::CString strAttrName(XN(attrs.getQName(i)));

        izanagi::tool::CString val(XN(attrs.getValue(i)));


        if (strAttrName == attr_type::PATH) {
            // path
            if (m_BasePath.empty()) {
                imgElement.path = val;
            }
            else {
                imgElement.path.format(
                    "%s\\%s",
                    m_BasePath.c_str(),
                    val.c_str());
            }
        }
        else {
            // 小文字にする
            val.make_lower();

            if (strAttrName == attr_type::TEX_IDX) {
                // texIdx
                imgElement.texIdx = atoi(val);
            }
            else if (strAttrName == attr_type::FACE) {
                // face
                imgElement.face = _ConvTextToCubeFace(val);
            }
        }
    }

    imageInfo->elements.push_back(imgElement);
}

namespace {
    // テクスチャ出力
    BOOL _ExportTex(
        izanagi::IOutputStream* pOut,
        izanagi::E_PLATFORM type,
        izanagi::S_IMG_HEADER* pHeader,
        const SImageInfo& sImageInfo,
        izanagi::tool::CIMGTexture* pTex)
    {
        IZ_ASSERT(pOut != NULL);
        IZ_ASSERT(pHeader != NULL);

        BOOL result = TRUE;

        // フォーマット変換
        result = pTex->ConvertPixelFormat(
            type,
            sImageInfo.info.fmt);
        THROW_EXCEPTION(
            result,
            CErrorLog,
            "Failed : ConvertPixelFormat(%d)",
            sImageInfo.info.fmt);

        // MIPMAP作成
        result = pTex->CreateMipMap(sImageInfo.info.level);
        THROW_EXCEPTION(
            result,
            CErrorLog,
            "Failed : CreateMipMap(%d)",
            sImageInfo.info.level);

        pTex->SetTexAddress(
            sImageInfo.info.addressU, 
            sImageInfo.info.addressV);
        pTex->SetTexFilter(
            sImageInfo.info.minFilter,
            sImageInfo.info.magFilter,
            sImageInfo.info.mipFilter);

        // 最大サイズ
        IZ_UINT nSize = pTex->GetExportSize();
        pHeader->sizeMax = IZ_MAX(nSize, pHeader->sizeMax);

        // 最大ピッチサイズ
        IZ_UINT pitch = 0;
        if (izanagi::graph::CGraphUtil::IsCompressedPixelFormat(pTex->GetTexInfo().fmt))
        {
            IZ_UINT width = pTex->GetWidth();
            pitch = width * 4;
        }
        else
        {
            pitch = pTex->GetPitchSize();
        }
        pHeader->maxPitch = IZ_MAX(pitch, pHeader->maxPitch);

        // 出力
        result = pTex->Write(pOut);
        THROW_EXCEPTION(
            result,
            CErrorLog,
            "Failed : Export",
            sImageInfo.info.level);

        return result;
    }

    // テクスチャ出力
    IZ_BOOL _ExportTex(
        izanagi::CFileOutputStream* out,
        izanagi::E_PLATFORM type,
        izanagi::S_IMG_HEADER* header,
        const SImageInfo& imageInfo)
    {
        VRETURN(imageInfo.elements.size() > 0);

        IZ_BOOL ret = IZ_TRUE;

        const SImageElement& elem = imageInfo.elements[0];

        // テクスチャ読み込み
        izanagi::tool::CIMGMaster* imgMaster = NULL;
        imgMaster = izanagi::tool::CImageReader::GetInstance().Read(
                    elem.path,
                    izanagi::graph::E_GRAPH_TEX_TYPE_PLANE);
        VRETURN(imgMaster != IZ_NULL);

        UINT nNum = imgMaster->GetTexNum();
        
        if (elem.texIdx >= 0) {
            INT nTexIdx = IZ_MAX(nNum - 1, (UINT)elem.texIdx);
            izanagi::tool::CIMGTexture* tex = imgMaster->GetTexture(nTexIdx);

            if (tex != IZ_NULL) {
                // テクスチャ出力
                ret = _ExportTex(
                        out,
                        type,
                        header,
                        imageInfo,
                        tex);
                IZ_ASSERT(ret);
            }
        }
        else {
            // 全テクスチャ

            for (UINT n = 0; n < nNum; n++) {
                izanagi::tool::CIMGTexture* tex = imgMaster->GetTexture(n);

                if (tex != IZ_NULL) {
                    // テクスチャ出力
                    ret = _ExportTex(
                            out,
                            type,
                            header,
                            imageInfo,
                            tex);
                    IZ_ASSERT(ret);
                    break;
                }
            }
        }

        // 削除
        izanagi::tool::CImageReader::GetInstance().Delete(imgMaster);
        return ret;
    }

    // キューブテクスチャ出力
    IZ_BOOL _ExportCubeTex(
        izanagi::CFileOutputStream* out,
        izanagi::E_PLATFORM type,
        izanagi::S_IMG_HEADER* header,
        const SImageInfo& imageInfo)
    {
        VRETURN(imageInfo.elements.size() == izanagi::graph::E_GRAPH_CUBE_TEX_FACE_NUM);

        IZ_BOOL ret = IZ_TRUE;

        // 出力用テクスチャ作成
        izanagi::tool::CIMGTexture* exportTex = izanagi::tool::CIMGTexture::CreateEmptyTexture(izanagi::graph::E_GRAPH_TEX_TYPE_CUBE);
        VRETURN(exportTex != IZ_NULL);

        // 基準となる幅・高さ
        // キューブテクスチャは全ての面が同じ幅と高さにならないといけない
        // そのチェックに利用する
        IZ_UINT baseWidth = 0;
        IZ_UINT baseHeight = 0;

        for (IZ_UINT i = 0; i < izanagi::graph::E_GRAPH_CUBE_TEX_FACE_NUM; i++) {
            const SImageElement& elem = imageInfo.elements[i];

            // テクスチャ読み込み
            izanagi::tool::CIMGMaster* imgMaster = NULL;
            imgMaster = izanagi::tool::CImageReader::GetInstance().Read(
                        elem.path,
                        izanagi::graph::E_GRAPH_TEX_TYPE_PLANE);
            VRETURN(imgMaster != IZ_NULL);

            IZ_UINT nNum = imgMaster->GetTexNum();
            
            IZ_UINT texIdx = (elem.texIdx < 0 ? 0 : elem.texIdx);
            texIdx = IZ_MAX(nNum - 1, texIdx);

            // 読み込んだテクスチャを取得
            // Planeテクスチャでないといけない
            izanagi::tool::CIMGTexture* tex = imgMaster->GetTexture(texIdx);

            // 幅チェック
            if (baseWidth == 0) {
                // 基準値取得
                baseWidth = tex->GetWidth();
            }
            else {
                ret = (baseWidth == tex->GetWidth());
                VGOTO(ret, __EXIT__);
            }

            // 高さチェック
            if (baseHeight == 0) {
                // 基準値取得
                baseHeight = tex->GetHeight();
            }
            else {
                ret = (baseHeight == tex->GetHeight());
                VGOTO(ret, __EXIT__);
            }

            if (tex->GetTexInfo().type == izanagi::graph::E_GRAPH_TEX_TYPE_PLANE) {
                // 読み込んだイメージデータを出力用テクスチャにセット
                exportTex->AddImageAsCubeFace(
                    (izanagi::graph::E_GRAPH_CUBE_TEX_FACE)i,
                    tex->GetImage(0));

                // 所有者をexportTexにする
                tex->GetImage(0).erase(tex->GetImage(0).begin());
            }

            // 削除
            izanagi::tool::CImageReader::GetInstance().Delete(imgMaster);
        }

        // テクスチャ出力
        if (ret) {
            ret = _ExportTex(
                    out,
                    type,
                    header,
                    imageInfo,
                    exportTex);
            IZ_ASSERT(ret);
        }

__EXIT__:
        // 削除
        izanagi::tool::CIMGTexture::Delete(exportTex);

        return ret;
    }
}   // namespace

// IMGデータ作成
BOOL CImageBuilder::BuildIMG(
    izanagi::E_PLATFORM type,
    LPCSTR lpszExport)
{
    izanagi::CFileOutputStream cOut;
    THROW_EXCEPTION(
        cOut.Open(lpszExport),
        CErrorLog,
        "Failed : Open File(%s)",
        lpszExport);

    // DirectXテクスチャ読み込み処理をセット
    izanagi::tool::CImageReader::GetInstance().SetReadFunc(
        CImageReaderImpl::Read);

    BOOL ret = FALSE;

    // テクスチャ数
    IZ_UINT nTexNum = static_cast<IZ_UINT>(m_ImageInfoList.size());

    // ヘッダ
    izanagi::S_IMG_HEADER sHeader;
    izanagi::tool::CImageUtil::SetImageHeader(
        &sHeader,
        nTexNum);

    // TODO
    // プラットフォーム
    sHeader.platform = izanagi::E_PLATFORM_DX9;

    // ジャンプテーブル
    std::vector<UINT> tJumpTable;
    tJumpTable.reserve(nTexNum);

    // ヘッダ + ジャンプテーブル分空ける
    ret = cOut.Seek(
            sizeof(izanagi::S_IMG_HEADER) + sizeof(IZ_UINT) * nTexNum, 
            izanagi::E_IO_STREAM_SEEK_POS_START);
    VGOTO(ret, __EXIT__);

    for (UINT i = 0; i < m_ImageInfoList.size(); i++) {
        const SImageInfo& sImageInfo = m_ImageInfoList[i];

        // ジャンプテーブル
        // 出力先の現在位置がイメージデータの位置（ジャンプ先）になる
        tJumpTable.push_back(cOut.GetSize());

        if (sImageInfo.info.type == izanagi::graph::E_GRAPH_TEX_TYPE_CUBE) {
            ret = _ExportCubeTex(&cOut, type, &sHeader, sImageInfo);
            VGOTO(ret, __EXIT__);
        }
        else if (sImageInfo.info.type == izanagi::graph::E_GRAPH_TEX_TYPE_VOLUME) {
        }
        else {
            ret = _ExportTex(&cOut, type, &sHeader, sImageInfo);
            VGOTO(ret, __EXIT__);
        }
    }

    // ファイルサイズ取得
    sHeader.sizeFile = cOut.GetCurPos();

    // 先頭に戻る
    ret = cOut.Seek(0, izanagi::E_IO_STREAM_SEEK_POS_START);
    VGOTO(ret, __EXIT__);

    // ヘッダ出力
    ret = IZ_OUTPUT_WRITE(&cOut, &sHeader, 0, sizeof(sHeader));
    VGOTO(ret, __EXIT__);

    // ジャンプテーブル出力
    if (tJumpTable.size() > 0) {
        ret = IZ_OUTPUT_WRITE(&cOut, &tJumpTable[0], 0, sizeof(IZ_UINT) * tJumpTable.size());
        VGOTO(ret, __EXIT__);
    }

__EXIT__:
    cOut.Finalize();

    return ret;
}

void CImageBuilder::SetBasePath(const char* path)
{
    m_BasePath.format("%s", path);
}
