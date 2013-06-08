#include <algorithm>
#include "ShaderConverterImpl.h"
#include "ShaderConfig.h"

#include "StringChunk.h"
#include "DataBuffer.h"
#include "TextureUtil.h"
#include "SamplerUtil.h"
#include "ParamUtil.h"
#include "PassUtil.h"
#include "ShaderConverterUtil.h"
#include "CompileCmdCreator.h"

#include "izToolkit.h"

CShaderConverter CShaderConverter::s_cInstance;

/**
* コンストラクタ
*/
CShaderConverter::CShaderConverter()
{
    m_pCgContext = NULL;
    m_pCgEffect = NULL;
}

#define DESTROY(p, func)    if (p != NULL) { func(p); }

/**
* デストラクタ
*/
CShaderConverter::~CShaderConverter()
{
    DESTROY(m_pCgContext, ::cgDestroyContext);
    DESTROY(m_pCgEffect, ::cgDestroyEffect);
}

BOOL CShaderConverter::Begin(LPCSTR lpszShaderFile)
{
    // Cgコンテキスト作成
    m_pCgContext = cgCreateContext();

    // stateの保持のために必要
    // ただし、D3D9関係のヘッダ、ライブラリ、DLLが必要になる
    cgD3D9RegisterStates(m_pCgContext);

    // エフェクト作成
    m_pCgEffect = cgCreateEffectFromFile(
                    m_pCgContext,
                    lpszShaderFile,
                    NULL);

    CGerror error = ::cgGetError();

    if (error != CG_NO_ERROR) {
        // エラー出力表示
        const char* pErrorStr = cgGetLastListing(m_pCgContext);
        if (pErrorStr != NULL) {
            printf("%s\n", pErrorStr);

#if 0
            // NOTE
            // 何故かcgCreateEffectFromFileでコンパイルが失敗してもNULLが返ってこないことがある・・・。
            // なので、エラー文字列を調べて "error" があったら失敗とみなす・・・。
            izanagi::tool::CString str(pErrorStr);
            if (str.Find(_T(" error ")) >= 0) {
                return FALSE;
            }
#endif
        }
    }

    if (m_pCgEffect == NULL) {
        // エフェクト作成失敗したので、コンバート失敗
        return FALSE;
    }

    return TRUE;
}

namespace {
    inline IZ_BOOL _ExportChunkHeader(
        izanagi::CFileOutputStream& cOut,
        IZ_UINT nMagicNumber)
    {
        izanagi::S_SHD_CHUNK_HEADER sChunkHeader;
        sChunkHeader.magicChunk = nMagicNumber;

        IZ_BOOL ret = IZ_OUTPUT_WRITE(&cOut, &sChunkHeader, 0, sizeof(sChunkHeader));
        IZ_ASSERT(ret);

        return ret;
    }
}   // namespace

// NOTE
// フォーマット
// +----------------+
// | ファイルヘッダ |
// +----------------+
// |   パラメータ   |
// |    テーブル    |
// +----------------+
// | アノテーション |
// |  (パラメータ)  |
// |    テーブル    |
// +----------------+
// |パラメータ初期値|
// |    バッファ    |
// +----------------+
// |   テクスチャ   |
// |    テーブル    |
// +----------------+
// |サンプラテーブル|
// +----------------+
// |   テクニック   |
// |    テーブル    |
// +----------------+
// |  パステーブル  |
// +----------------+
// |パスで利用される|
// |  パラメータと  |
// |   サンプラの   |
// |  インデックス  |
// +----------------+
// | 文字列バッファ |
// +----------------+
// |   プログラム   |
// +----------------+-+
// | 頂点プログラム | |
// |     ヘッダ     | |
// +----------------+ +-- (1)
// | 頂点プログラム | |
// +----------------+-+
// |     (1)を      |
// | numVtxProgram  |
// |    繰り返す    |
// +----------------+

/**
* シェーダ解析
*/
BOOL CShaderConverter::Export(const SShaderConfig& sConfig)
{
    IZ_ASSERT(m_pCgEffect != NULL);

    VRETURN(m_Out.Open(sConfig.out));

    // ファイルヘッダ
    {
        FILL_ZERO(&m_ShdHeader, sizeof(m_ShdHeader));

        // TODO
        // magic
        // version

        m_ShdHeader.name.SetString(sConfig.name);
        m_ShdHeader.nameKey = m_ShdHeader.name.GetKeyValue();

        m_ShdHeader.sizeHeader = sizeof(m_ShdHeader);
    }

    // ファイルヘッダ分空ける
    VRETURN(m_Out.Seek(sizeof(m_ShdHeader), izanagi::E_IO_STREAM_SEEK_POS_START));

    ExportParameter();
    ExportTexture();
    ExportSampler();
    ExportTechnique();
    
    ExportPass();

    ExportStringBuffer();

    // Export program chunk's header.
    _ExportChunkHeader(m_Out, izanagi::SHD_CHUNK_MAGIC_NUMBER_PROG);

    // プログラム開始位置
    m_ShdHeader.posProgram = m_Out.GetCurPos();

    ExportProgram();

    // Export chunk's terminater.
    //_ExportChunkHeader(m_Out, izanagi::SHD_PROGRAM_CHUNK_TERMINATER);

    // ファイルサイズ取得
    m_ShdHeader.sizeFile = m_Out.GetCurPos();

    // 先頭に戻る
    VRETURN(m_Out.Seek(0, izanagi::E_IO_STREAM_SEEK_POS_START));

    // ファイルヘッダ出力
    m_Out.Write(&m_ShdHeader, 0, sizeof(m_ShdHeader));

    m_Out.Finalize();
    
    return TRUE;
}

template <typename _T>
BOOL CShaderConverter::ExportData(const _T& data)
{
    IZ_OUTPUT_WRITE_VRETURN(
        &m_Out,
        &data,
        0,
        sizeof(_T));

    return TRUE;
}

namespace {
    template <typename _T>
    inline IZ_BOOL _BeginExportChunk(
        const _T& sHeader,
        izanagi::tool::CIoStreamSeekHelper& cSeekHelper)
    {
        return cSeekHelper.Skip(sizeof(_T));
    }

    template <typename _T>
    inline IZ_BOOL _EndExportChunk(
        const _T& sHeader,
        izanagi::tool::CIoStreamSeekHelper& cSeekHelper)
    {
        izanagi::IOutputStream* pOut = cSeekHelper.GetOutputStream();
        IZ_ASSERT(pOut != IZ_NULL);

        // Return to paremter's header position with anchor.
        VRETURN(cSeekHelper.ReturnWithAnchor());

        // Export paramter's header.
        IZ_OUTPUT_WRITE_VRETURN(pOut, &sHeader, 0, sizeof(sHeader));

        // Return to anchored position.
        VRETURN(cSeekHelper.ReturnToAnchor());

        return IZ_TRUE;
    }

    // テクニック内のパス数取得
    inline IZ_UINT _GetPassNum(CGtechnique tech)
    {
        IZ_UINT ret = 0;

        CGpass pass = ::cgGetFirstPass(tech);
        while (pass != NULL) {
            ret++;
            pass = ::cgGetNextPass(pass);
        }

        return ret;
    }
}   // namespace

/**
* テクニック解析
*/
BOOL CShaderConverter::ExportTechnique()
{
    VRETURN(_ExportChunkHeader(m_Out, izanagi::SHD_CHUNK_MAGIC_NUMBER_TECH));

    izanagi::S_SHD_TECH_HEADER sTechHeader;
    FILL_ZERO(&sTechHeader, sizeof(sTechHeader));

    // Blank for technique's header.
    izanagi::tool::CIoStreamSeekHelper cSeekHelper(&m_Out);
    VRETURN(_BeginExportChunk(sTechHeader, cSeekHelper));

    UINT nPassPos = 0;

    // テクニック取得
    CGtechnique tech = ::cgGetFirstTechnique(m_pCgEffect);

    if (tech != NULL) {
        while (tech != NULL) {
            izanagi::S_SHD_TECHNIQUE sTech;
            {
                FILL_ZERO(&sTech, sizeof(sTech));

                IZ_PCSTR name = ::cgGetTechniqueName(tech);
                IZ_UINT pos = CStringChunk::GetInstance().Register(name);

                //sTech.name = *(IZ_PCSTR*)(&pos);
                sTech.posName = pos;
                sTech.keyName = izanagi::CKey::GenerateValue(name);
                sTech.numPass = _GetPassNum(tech);
                sTech.posPass = nPassPos;
            }

            nPassPos += sTech.numPass;

            // 出力
            VRETURN(ExportData(sTech));

            tech = ::cgGetNextTechnique(tech);

            sTechHeader.numTechnique++;
        }
    }
    else {
        // テクニックは必ず一つはないといけない
        IZ_ASSERT(FALSE);

        // TODO

        return FALSE;
    }

    VRETURN(_EndExportChunk(sTechHeader, cSeekHelper));

    return TRUE;
}

BOOL CShaderConverter::ExportTexture()
{
    VRETURN(_ExportChunkHeader(m_Out, izanagi::SHD_CHUNK_MAGIC_NUMBER_TEX));

    izanagi::S_SHD_TEXTRUE_HEADER sTexHeader;
    FILL_ZERO(&sTexHeader, sizeof(sTexHeader));

    // Blank for texture's header.
    izanagi::tool::CIoStreamSeekHelper cSeekHelper(&m_Out);
    VRETURN(_BeginExportChunk(sTexHeader, cSeekHelper));

    m_TexList.clear();

    CGparameter param = ::cgGetFirstEffectParameter(m_pCgEffect);

    while (param != NULL) {
        if (CParamUtil::IsTexture(param)) {
            izanagi::S_SHD_TEXTURE sTex;
            {
                FILL_ZERO(&sTex, sizeof(sTex));

                CParamUtil::SetNameAndSemantic(sTex, param);

#if 0
                sTex.type = _GetTexTypeFromSemantic(param);

                sTex.ann.isRenderTarget = IZ_TRUE;
                sTex.ann.isDynamic = IZ_FALSE;
                sTex.ann.typeRsc = izanagi::graph::E_GRAPH_RSC_USAGE_STATIC;

                VRETURN(
                    CTextureUtil::SetAnnValue(
                        sTex.ann,
                        param));
#endif
            }

            m_TexList.push_back(param);

            // 出力
            VRETURN(ExportData(sTex));

            sTexHeader.numTexture++;
        }

        param = ::cgGetNextParameter(param);
    }

    VRETURN(_EndExportChunk(sTexHeader, cSeekHelper));

    return TRUE;
}

BOOL CShaderConverter::ExportSampler()
{
    VRETURN(_ExportChunkHeader(m_Out, izanagi::SHD_CHUNK_MAGIC_NUMBER_SMPL));

    izanagi::S_SHD_SAMPLER_HEADER sSmplHeader;
    FILL_ZERO(&sSmplHeader, sizeof(sSmplHeader));

    // Blank for texture's header.
    izanagi::tool::CIoStreamSeekHelper cSeekHelper(&m_Out);
    VRETURN(_BeginExportChunk(sSmplHeader, cSeekHelper));

    CGparameter param = ::cgGetFirstEffectParameter(m_pCgEffect);

    // パスを取得
    std::vector<CGpass> passList;
    {
        CGtechnique tech = ::cgGetFirstTechnique(m_pCgEffect);
        while (tech != NULL) {
            CGpass pass = ::cgGetFirstPass(tech);
            while (pass != NULL) {
                passList.push_back(pass);
                pass = ::cgGetNextPass(pass);
            }
            tech = ::cgGetNextTechnique(tech);
        }
    }

    while (param != NULL) {
        if (::cgIsParameterUsed(param, m_pCgEffect)) {
            VRETURN(CParamUtil::IsValidParameter(param));

            if (CParamUtil::IsSampler(param)) {
                // 対象となるパスのインデックスを取得
                IZ_INT passIdx = -1;
                for (IZ_UINT i = 0; i < passList.size(); i++) {
                    if (::cgIsParameterUsed(param, passList[i])) {
                        passIdx = i;
                        break;
                    }
                }

                izanagi::S_SHD_SAMPLER sSampler;
                {
                    FILL_ZERO(&sSampler, sizeof(sSampler));

                    sSampler.state.minFilter = izanagi::graph::E_GRAPH_TEX_FILTER_LINEAR;
                    sSampler.state.magFilter = izanagi::graph::E_GRAPH_TEX_FILTER_LINEAR;
                    sSampler.state.mipFilter = izanagi::graph::E_GRAPH_TEX_FILTER_LINEAR;
                    sSampler.state.addressU = izanagi::graph::E_GRAPH_TEX_ADDRESS_CLAMP;
                    sSampler.state.addressV = izanagi::graph::E_GRAPH_TEX_ADDRESS_CLAMP;

                    CParamUtil::SetNameAndSemantic(sSampler, param);

                    CGtype type = CParamUtil::GetCgType(param);
                    sSampler.Type = CShaderConvUtil::CgParamTypeToIzanagiShaderParamType(type);
                    
                    VRETURN(
                        CSamplerUtil::SetStateValue(
                            sSampler,
                            param));

                    // 対象となるテクスチャとのバインド情報を設定
                    VRETURN(
                        CSamplerUtil::BindTexture(
                            sSampler,
                            param,
                            m_TexList));

                    if (passIdx >= 0) {
                        // シェーダ定数テーブルを作成
                        IZ_ASSERT(passIdx < m_CompiledPSList.size());

                        izanagi::tool::CSimpleMemoryAllocator allocator;

                        izanagi::tool::CShaderConstTableLite* constTbl = izanagi::tool::CShaderConstTableLite::CreateShaderConstTableLite(
                            &allocator,
                            m_CompiledPSList[passIdx]);

                        const char* paramName = ::cgGetParameterName(param);

                        sSampler.resource_id = constTbl->GetSamplerIndex(paramName);

                        SAFE_RELEASE(constTbl);
                    }
                    else {
                        // ある？
                        sSampler.resource_id = -1;
                    }
                }

                m_SamplerList.push_back(param);

                // 出力
                VRETURN(ExportData(sSampler));

                sSmplHeader.numSampler++;
            }
        }

        param = ::cgGetNextParameter(param);
    }

    //m_ShdHeader.numSmpl = sSmplHeader.numSampler;

    VRETURN(_EndExportChunk(sSmplHeader, cSeekHelper));

    return TRUE;
}

/**
* パラメータ解析
*/
BOOL CShaderConverter::ExportParameter()
{
    _ExportChunkHeader(m_Out, izanagi::SHD_CHUNK_MAGIC_NUMBER_PARAM);

    izanagi::S_SHD_PARAM_HEADER sParamHeader;
    FILL_ZERO(&sParamHeader, sizeof(sParamHeader));

    // Blank for parameter's header.
    izanagi::tool::CIoStreamSeekHelper cSeekHelper(&m_Out);
    VRETURN(_BeginExportChunk(sParamHeader, cSeekHelper));

    IZ_UINT nAnnIdx = 0;

    // For description.
    CGparameter param = ::cgGetFirstEffectParameter(m_pCgEffect);

    while (param != NULL) {
        if (CParamUtil::IsParameter(param))
        {
            // For Debug...
            IZ_PCSTR name = ::cgGetParameterName(param);

            VRETURN(CParamUtil::IsValidParameter(param));

            izanagi::S_SHD_PARAMETER sParam;
            FILL_ZERO(&sParam, sizeof(sParam));
            
            sParam.DoNotStrip = CParamUtil::DoNotStrip(param);
            BOOL bIsUsedInEffect = ::cgIsParameterUsed(param, m_pCgEffect);

#if 0
            if (sParam.DoNotStrip
                || DoNotRemoveParam(param))
#else
            if (bIsUsedInEffect || sParam.DoNotStrip)
#endif
            {
                CParamUtil::SetNameAndSemantic(sParam, param);

                VRETURN(
                    CParamUtil::SetDescValue(
                        sParam,
                        param));

                if (sParam.hasAnn) {
                    sParam.AnnotationIdx = nAnnIdx++;
                }

                // Register initial value.
                VRETURN(
                    CParamUtil::GetInitValue(
                        sParam,
                        param));

                m_ParamList.push_back(param);

                // 出力
                VRETURN(ExportData(sParam));

                sParamHeader.numParameter++;
            }
        }

        param = ::cgGetNextParameter(param);
    }

    VRETURN(ExportParamAnn(sParamHeader, nAnnIdx));

    // Export initial value.
    sParamHeader.sizeValueBuffer = CDataBuffer::GetInstance().GetBufferSize();
    if (sParamHeader.sizeValueBuffer > 0) {
        const void* pBuf = CDataBuffer::GetInstance().GetBuffer();

        IZ_OUTPUT_WRITE_VRETURN(
            &m_Out,
            pBuf,
            0,
            sParamHeader.sizeValueBuffer);
    }

    //m_ShdHeader.numParam = sParamHeader.numParameter;

    VRETURN(_EndExportChunk(sParamHeader, cSeekHelper));

    return TRUE;
}

BOOL CShaderConverter::ExportParamAnn(
    izanagi::S_SHD_PARAM_HEADER& sParamHeader,
    IZ_INT nAnnNum/*= -1*/)
{
    IZ_UINT nAnnCnt = 0;

    // For annotation.
    std::vector<CGparameter>::iterator it = m_ParamList.begin();

    for (; it != m_ParamList.end(); it++) {
        CGparameter param = *it;

        if (CParamUtil::HasAnn(param)) {
            izanagi::S_SHD_PARAM_ANN sAnn;
            
            FILL_ZERO(&sAnn, sizeof(sAnn));

            VRETURN(
                CParamUtil::SetAnnValue(
                    sAnn,
                    param));

            // 出力
            VRETURN(ExportData(sAnn));

            nAnnCnt++;
        }
    }

    BOOL ret = (nAnnNum >= 0
                ? (nAnnCnt == nAnnNum)
                : TRUE);

    sParamHeader.numParamAnn = nAnnCnt;

    IZ_ASSERT(ret);
    return ret;
}

#if 0
BOOL CShaderConverter::DoNotRemoveParam(CGparameter param)
{
    BOOL bIsUsedInEffect = ::cgIsParameterUsed(param, m_pCgEffect);

    izanagi::tool::CString strParamName(::cgGetParameterName(param));

    {
        // 頂点シェーダでしか利用されていない
        // -> ピクセルシェーダで利用されていたらセーフ
        CGtechnique tech = ::cgGetFirstTechnique(m_pCgEffect);
        while (tech != NULL) {
            CGpass pass = ::cgGetFirstPass(tech);

            while (pass != NULL) {
                // Check whether the parameter is use in pixel shader.
                CGprogram progPS = CPassUtil::GetPSProgram(pass);
                if (::cgIsParameterUsed(param, progPS)
                    && bIsUsedInEffect)
                {
                    return TRUE;
                }

                // Check whether the parameter is texture offset.
                IZ_PCSTR pszTexOffsetParam = CPassUtil::GetTextureOffsetParameterName(pass);
                if (pszTexOffsetParam != IZ_NULL) {
                    if (strParamName == pszTexOffsetParam) {
                        return TRUE;
                    }
                }

                pass = ::cgGetNextPass(pass);
            }

            tech = ::cgGetNextTechnique(tech);
        }
    }

    return FALSE;
}
#endif

namespace {
    inline IZ_UINT _GetUsedParamNum(
        const std::vector<CGparameter>& tvParamList,
        CGpass pass)
    {
        IZ_UINT ret = 0;

        std::vector<CGparameter>::const_iterator it = tvParamList.begin();
        for (; it != tvParamList.end(); it++) {
            CGparameter param = *it;

            if (::cgIsParameterUsed(param, pass)) {
                ret++;
            }
        }

        return ret;
    }

    inline IZ_INT _GetParamIdx(
        IZ_PCSTR pszParamName,
        const std::vector<CGparameter>& tvParamList,
        CGpass pass)
    {
        izanagi::tool::CString strName(pszParamName);

        std::vector<CGparameter>::const_iterator it = tvParamList.begin();
        for (IZ_INT i = 0; it != tvParamList.end(); it++, i++) {
            CGparameter param = *it;

            IZ_PCSTR name = ::cgGetParameterName(param);
            if (strName == name) {
                return i;
            }
        }

        return -1;
    }

    inline IZ_INT _GetRenderTargetIdx(
        const std::vector<CGparameter>& tvParamList,
        CGpass pass)
    {
        IZ_PCSTR pszRTName = CPassUtil::GetRenderTargetName(pass);
        if (pszRTName == NULL) {
            return -1;
        }

        IZ_INT ret = _GetParamIdx(
                        pszRTName,
                        tvParamList,
                        pass);

        return ret;
    }

    inline IZ_INT _GetTexOffsetParamIdx(
        const std::vector<CGparameter>& tvParamList,
        CGpass pass)
    {
        IZ_PCSTR pszTexOffsetName = CPassUtil::GetTextureOffsetParameterName(pass);
        if (pszTexOffsetName == NULL) {
            return -1;
        }
        
        IZ_INT ret = _GetParamIdx(
                        pszTexOffsetName,
                        tvParamList,
                        pass);

        return ret;
    }

    inline IZ_UINT _GetFileSize(IZ_PCSTR pszFile)
    {
        izanagi::CFileInputStream in;
        VRETURN_VAL(in.Open(pszFile), 0);

        IZ_UINT ret = static_cast<IZ_UINT>(in.GetSize());

        return ret;
    }
}   // namespace

// パス解析
BOOL CShaderConverter::ExportPass()
{
    _ExportChunkHeader(m_Out, izanagi::SHD_CHUNK_MAGIC_NUMBER_PASS);

    izanagi::S_SHD_PASS_HEADER sPassHeader;
    FILL_ZERO(&sPassHeader, sizeof(sPassHeader));

    // Blank for pass's header.
    izanagi::tool::CIoStreamSeekHelper cSeekHelper(&m_Out);
    VRETURN(_BeginExportChunk(sPassHeader, cSeekHelper));

    IZ_UINT nTechIdx = 0;
    IZ_UINT nPassIdx = 0;

    IZ_UINT nConstNum = 0;
    IZ_UINT nSamplerNum = 0;

    CGtechnique tech = ::cgGetFirstTechnique(m_pCgEffect);

    while (tech != NULL) {
        CGpass pass = ::cgGetFirstPass(tech);
        while (pass != NULL) {
            izanagi::S_SHD_PASS sPass;
            {
                FILL_ZERO(&sPass, sizeof(sPass));

                IZ_PCSTR name = ::cgGetPassName(pass);
                IZ_UINT pos = CStringChunk::GetInstance().Register(name);

                //sPass.name = *(IZ_PCSTR*)(&pos);
                sPass.posName = pos;
                sPass.keyName = izanagi::CKey::GenerateValue(name);

                sPass.TechniqueIdx = nTechIdx;

                sPass.numConst = _GetUsedParamNum(m_ParamList, pass);
                sPass.numSampler = _GetUsedParamNum(m_SamplerList, pass);

                sPass.sizeVS = _GetFileSize(m_CompiledVSList[nPassIdx]);
                sPass.sizePS = _GetFileSize(m_CompiledPSList[nPassIdx]);

                nConstNum += sPass.numConst;
                nSamplerNum += sPass.numSampler;
            }

            VRETURN(
                CPassUtil::SetStateValue(
                    sPass.state,
                    pass));

            // 出力
            VRETURN(ExportData(sPass));

            pass = ::cgGetNextPass(pass);
            nPassIdx++;

            IZ_UINT nMax = IZ_MAX(sPass.sizeVS, sPass.sizePS);

            m_ShdHeader.maxProgamSize = IZ_MAX(m_ShdHeader.maxProgamSize, nMax);

            sPassHeader.numPass++;
        }

        tech = ::cgGetNextTechnique(tech);
        nTechIdx++;
    }

    m_ShdHeader.numPass = sPassHeader.numPass;

    // NOTE
    // 全体でのシェーダ定数、サンプラの総数を知りたい

    m_ShdHeader.numParam = nConstNum;
    m_ShdHeader.numSmpl = nSamplerNum;

    VRETURN(ExportUsedParamAndSamplerIdxByPass());

    VRETURN(_EndExportChunk(sPassHeader, cSeekHelper));

    return TRUE;
}

BOOL CShaderConverter::ExportUsedParamAndSamplerIdxByPass()
{
    CGtechnique tech = ::cgGetFirstTechnique(m_pCgEffect);

    while (tech != NULL) {
        CGpass pass = ::cgGetFirstPass(tech);
        while (pass != NULL) {
            VRETURN(ExportUsedParamAndSamplerIdxByPass(pass));

            pass = ::cgGetNextPass(pass);
        }

        tech = ::cgGetNextTechnique(tech);
    }

    return TRUE;
}

namespace {
    // 指定されたパス内で利用されているパラメータのインデックスを出力
    inline IZ_BOOL _ExportUsedParamIdxByPass(
        izanagi::IOutputStream* pOut,
        const std::vector<CGparameter>& tvParamList,
        CGpass pass)
    {
        std::vector<CGparameter>::const_iterator it = tvParamList.begin();

        for (IZ_UINT nIdx = 0; it != tvParamList.end(); it++, nIdx++) {
            CGparameter param = *it;

            // For Debug
            //const char* name = ::cgGetParameterName(param);

            if (::cgIsParameterUsed(param, pass)) {
                CGprogram progVS = CPassUtil::GetVSProgram(pass);
                CGprogram progPS = CPassUtil::GetPSProgram(pass);

                // シェーダ内で利用されているかどうか
                IZ_BOOL bIsUsedInVS = ::cgIsParameterUsed(param, progVS);
                IZ_BOOL bIsUsedInPS = ::cgIsParameterUsed(param, progPS);

                izanagi::S_SHD_PARAM_IDX sParamIdx;
                {
                    sParamIdx.isVS = IZ_FALSE;
                    sParamIdx.isPS = IZ_FALSE;
                }

                if (bIsUsedInVS) {
                    // 頂点シェーダで利用されている
                    sParamIdx.isVS = IZ_TRUE;
                    sParamIdx.idx = nIdx;
                }
                if (bIsUsedInPS) {
                    // ピクセルシェーダで利用されている
                    sParamIdx.isPS = IZ_TRUE;
                    sParamIdx.idx = nIdx;
                }

                if (bIsUsedInVS || bIsUsedInPS) {
                    // シェーダ内で利用されているので出力
                    IZ_OUTPUT_WRITE_VRETURN(pOut, &sParamIdx, 0, sizeof(sParamIdx));
                }
            }
        }

        return IZ_TRUE;
    }
}   // namespace

BOOL CShaderConverter::ExportUsedParamAndSamplerIdxByPass(CGpass pass)
{
    // 指定されたパス内で利用されているパラメータのインデックスを出力
    VRETURN(
        _ExportUsedParamIdxByPass(
            &m_Out,
            m_ParamList,
            pass));

    // / 指定されたパス内で利用されているサンプラのインデックスを出力
    VRETURN(
        _ExportUsedParamIdxByPass(
            &m_Out,
            m_SamplerList,
            pass));

    return TRUE;
}

namespace {
    BOOL _ExportFile(
        const izanagi::tool::CString& strIn,
        izanagi::IOutputStream* pOut)
    {
        static const IZ_UINT BUF_SIZE = 1024;
        static IZ_UINT8 BUF[BUF_SIZE];

        izanagi::CFileInputStream cIn;
        VRETURN(cIn.Open(strIn));

        for (;;) {
            IZ_UINT nReadSize = cIn.Read(BUF, 0, BUF_SIZE);

            IZ_OUTPUT_WRITE_VRETURN(pOut, BUF, 0, nReadSize);

            if (nReadSize != BUF_SIZE) {
                break;
            }
        }

        return TRUE;
    }
}   // namespace

BOOL CShaderConverter::ExportProgram()
{
    VRETURN(m_CompiledPSList.size() == m_CompiledVSList.size());

    for (size_t i = 0; i < m_CompiledVSList.size(); i++) {
        const izanagi::tool::CString strVS = m_CompiledVSList[i];
        const izanagi::tool::CString strPS = m_CompiledPSList[i];

        VRETURN(_ExportFile(strVS, &m_Out));
        VRETURN(_ExportFile(strPS, &m_Out));
    }

    return TRUE;
}

BOOL CShaderConverter::ExportStringBuffer()
{
    _ExportChunkHeader(m_Out, izanagi::SHD_CHUNK_MAGIC_NUMBER_STRING);

    m_ShdHeader.sizeStringBuffer = CStringChunk::GetInstance().GetBufferSize();

    if (m_ShdHeader.sizeStringBuffer > 0) {
        IZ_PCSTR pszStrBuf = CStringChunk::GetInstance().GetBuffer();

        IZ_OUTPUT_WRITE_VRETURN(
            &m_Out,
            pszStrBuf,
            0,
            m_ShdHeader.sizeStringBuffer);
    }

    return TRUE;
}

//////////////////////////////////////////////////////////////////////

namespace {
    // コンパイラ実行
    BOOL _CompileShaderInternal(
        izanagi::tool::CString& out,
        COMPILE_TYPE type,
        const SShaderConfig& config,
        IZ_PCSTR entry,
        CGprofile profile)
    {
        // コマンド作成
        izanagi::tool::CString cmd;
        CCompileCmdCreator::GetInstance(config.type).CreateCompileCommand(
            cmd, out,
            type,
            config,
            entry,
            profile);

        int result = 0;

        if (!cmd.empty()) {
            // TODO
            FILE* fp = _popen(cmd, "w");
            if (fp == NULL) {
                IZ_ASSERT(FALSE);

                // TODO

                return FALSE;
            }

            result = _pclose(fp);
            IZ_ASSERT(result == 0);
        }

        return (result == 0);
    }

    // コンパイラ実行
    BOOL _CompileShader(
        izanagi::tool::CString& out,
        const SShaderConfig& config,
        IZ_PCSTR entry,
        CGprofile profile)
    {
        izanagi::tool::CString tmp = out;

        BOOL ret = FALSE;

        // 通常
        ret = _CompileShaderInternal(
                out,
                COMPILE_TYPE_SHADER,
                config,
                entry,
                profile);

        if (ret && config.isCompileAsm) {
            // アセンブラ出力
            ret = _CompileShaderInternal(
                    tmp,
                    COMPILE_TYPE_SHADER_ASM,
                    config,
                    entry,
                    profile);
        }

        return ret;
    }

    // 出力ファイル名を作成
    inline void _MakeOutFile(
        izanagi::tool::CString& strOut,
        UINT nID,
        LPCSTR lpszShaderFile,
        LPCSTR lpszObjDir)
    {
        static CHAR BUF[1024];

        izanagi::tool::CString tmp0;
        tmp0.format("%s", lpszShaderFile);

        // ファイル名取得
        LPSTR file_name = PathFindFileName(tmp0);

        // 拡張子削除
        PathRemoveExtension(file_name);

        izanagi::tool::CString tmp1;
        tmp1.format("%s_%d", file_name, nID);

        if (strlen(lpszObjDir) > 0) {
            // 中間ディレクトリに出力する
            PathCombine(
                BUF,
                lpszObjDir,
                tmp1);

            strOut.format("%s", BUF);
        }
        else {
            strOut.format("%s", tmp1.c_str());
        }
    }
}   // namespace

/**
* シェーダコンパイル
*/
BOOL CShaderConverter::CompileShader(
    const SShaderConfig& config,
    LPCSTR lpszObjDir)
{
    // 頂点プログラムをコンパイル
    BOOL b0 = CompileProgram(
                TRUE,
                config,
                lpszObjDir);

    // ピクセルプログラムをコンパイル
    BOOL b1 = CompileProgram(
                FALSE,
                config,
                lpszObjDir);

    return (b0 && b1);
}

BOOL CShaderConverter::CompileProgram(
    BOOL bIsVS,
    const SShaderConfig& config,
    LPCSTR lpszObjDir)
{
    CCompileCmdCreator::GetInstance(config.type).SetIsVS(FALSE);

    IZ_UINT nPassCnt = 0;

    CGtechnique tech = ::cgGetFirstTechnique(m_pCgEffect);

    while (tech != NULL) {
        CGpass pass = ::cgGetFirstPass(tech);

        while (pass != NULL) {
            // 出力ファイル名を作成
            izanagi::tool::CString strOut;
            _MakeOutFile(
                strOut,
                nPassCnt,
                config.shader,
                lpszObjDir);

            CCompileCmdCreator::GetInstance(config.type).SetIsVS(bIsVS);

            CGprogram program = (bIsVS
                                    ? CPassUtil::GetVSProgram(pass)
                                    : CPassUtil::GetPSProgram(pass));

            IZ_PCSTR entry = ::cgGetProgramString(program, CG_PROGRAM_ENTRY);
            
            IZ_PCSTR pszProfile = ::cgGetProgramString(program, CG_PROGRAM_PROFILE);
            CGprofile profile = ::cgGetProfile(pszProfile);

            // コンパイル
            if (!_CompileShader(
                    strOut,
                    config,
                    entry,
                    profile))
            {
                IZ_ASSERT(FALSE);

                // TODO

                return FALSE;
            }

            if (bIsVS) {
                m_CompiledVSList.push_back(strOut);
            }
            else {
                m_CompiledPSList.push_back(strOut);
            }

            nPassCnt++;
            pass = ::cgGetNextPass(pass);
        }

        tech = ::cgGetNextTechnique(tech);
    }

    return TRUE;
}
