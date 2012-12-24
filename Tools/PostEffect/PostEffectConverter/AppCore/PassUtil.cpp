#include "PassUtil.h"
#include "StringChunk.h"
#include "PostEffectConverterUtil.h"
#include "izToolKit.h"

namespace {
    // FunctorName
    BOOL _SetValueFunctorName(izanagi::S_PES_PASS_ANN& sAnn, CGannotation ann)
    {
        const char* name = ::cgGetStringAnnotationValue(ann);

        if (name != NULL) {
            IZ_UINT pos = CStringChunk::GetInstance().Register(name);

            //sAnn.nameFunctor = *(IZ_PCSTR*)(&pos);
            sAnn.posNameFunctor = pos;
            sAnn.keyFunctor = izanagi::CKey::GenerateValue(name);
        }
        
        return (name != NULL);
    }

    // FunctorArgsF
    BOOL _SetValueFunctorArgsF(izanagi::S_PES_PASS_ANN& sAnn, CGannotation ann)
    {
        INT num;
        const float* p = ::cgGetFloatAnnotationValues(ann, &num);
        
        BOOL ret = ((p != NULL) && (num <= izanagi::PES_FUNCTOR_ARGS_NUM));
        VRETURN(ret);

        if (ret) {
            for (INT i = 0; i < num; i++) {
                sAnn.FunctorArgs[i] = *(p + i);
            }
        }

        return ret;
    }

    // ScissorRectInflate
    BOOL _SetValueScissorRectInflate(izanagi::S_PES_PASS_ANN& sAnn, CGannotation ann)
    {
        INT num;
        const int* p = ::cgGetIntAnnotationValues(ann, &num);

        BOOL ret = ((p != NULL) && (num == 2));
        VRETURN(ret);

        if (ret) {
            sAnn.ScissorRectInflate[0] = *(p + 0);
            sAnn.ScissorRectInflate[1] = *(p + 1);
        }

        return ret;
    }

    // SrcTexRectInflate
    BOOL _SetValueSrcTexRectInflate(izanagi::S_PES_PASS_ANN& sAnn, CGannotation ann)
    {
        INT num;
        const int* p = ::cgGetIntAnnotationValues(ann, &num);

        BOOL ret = ((p != NULL) && (num == 2));
        VRETURN(ret);

        if (ret) {
            sAnn.SrcTexRectInflate[0] = *(p + 0);
            sAnn.SrcTexRectInflate[1] = *(p + 1);
        }

        return ret;
    }

    // ClearColor
    BOOL _SetValueClearColor(izanagi::S_PES_PASS_ANN& sAnn, CGannotation ann)
    {
        INT num;
        const int* p = ::cgGetIntAnnotationValues(ann, &num);

        BOOL ret = (p != NULL);
        VRETURN(ret);

        if (ret) {
            sAnn.ClearColor = *p;
            sAnn.isClearColor = TRUE;
        }

        return ret;
    }

    // TextureOffset
    BOOL _SetValueTextureOffset(izanagi::S_PES_PASS_ANN& sAnn, CGannotation ann)
    {
        // Nothing is done.
        // See _GetTexOffsetParamIdx in PostEffectConverterImpl.cpp...
        return IZ_TRUE;
    }

    // 各種アノテーションをセット
    BOOL _SetAnnValue(izanagi::S_PES_PASS_ANN& sAnn, CGannotation ann)
    {
        static struct {
            const char* Name;
            BOOL (*Func)(izanagi::S_PES_PASS_ANN&, CGannotation);
        } AnnTable[] = {
            {"FunctorName", _SetValueFunctorName},
            {"FunctorArgsF", _SetValueFunctorArgsF},                // ファンクタ引数（Float)
            {"ScissorRectInflate", _SetValueScissorRectInflate},    // シザー矩形Inflate設定値
            {"SrcTexRectInflate", _SetValueSrcTexRectInflate},      // 入力テクスチャ矩形Inflate設定値
            {"ClearColor", _SetValueClearColor},
            {"TextureOffset", _SetValueTextureOffset},              // テクスチャオフセットパラメータ
        };

        BOOL ret = TRUE;

        izanagi::tool::CString str(::cgGetAnnotationName(ann));

        for (IZ_UINT i = 0; i < COUNTOF(AnnTable); i++) {
            if (str == AnnTable[i].Name) {
                ret = (*AnnTable[i].Func)(sAnn, ann);
                break;
            }
        }

        return ret;
    }
}   // namepsace

BOOL CPassUtil::SetAnnValue(
    izanagi::S_PES_PASS_ANN& sAnn,
    CGpass pass)
{
    CGannotation ann = ::cgGetFirstPassAnnotation(pass);

    while (ann != NULL) {
        VRETURN(
            _SetAnnValue(
                sAnn,
                ann));

        ann = ::cgGetNextAnnotation(ann);
    }

    // ファンクタ名が設定されていない場合は、デフォルトファンクタになるようにする
    if (sAnn.keyFunctor == 0) {
        static const char* pszDefaultFunctorName = "PostEffectFunctorDefault";

        IZ_UINT pos = CStringChunk::GetInstance().Register(pszDefaultFunctorName);

        sAnn.posNameFunctor = pos;
        sAnn.keyFunctor = izanagi::CKey::GenerateValue(pszDefaultFunctorName);
    }

    return TRUE;
}

namespace {
    // AlphaBlendEnable
    BOOL _SetValueAlphaBlendEnable(izanagi::S_PES_PASS_STATE& sState, CGstateassignment sa)
    {
        INT num;
        const CGbool* p = ::cgGetBoolStateAssignmentValues(sa, &num);

        BOOL ret = (p != NULL);
        VRETURN(ret);

        if (ret) {
            sState.AlphaBlendEnable = *p;
        }
        return ret;
    }

    // AlphaFunc
    BOOL _SetValueAlphaFunc(izanagi::S_PES_PASS_STATE& sState, CGstateassignment sa)
    {
        INT num;
        const float* p = ::cgGetFloatStateAssignmentValues(sa, &num);

        BOOL ret = ((p != NULL) && (num == 2));
        VRETURN(ret);

        if (ret) {
            int func = (int)*p;

            sState.AlphaTestFunc = static_cast<izanagi::graph::E_GRAPH_CMP_FUNC>(func - 1);
            sState.AlphaTestRef = (IZ_UINT8)*(p + 1);
        }

        return ret;
    }

    // AlphaTestEnable
    BOOL _SetValueAlphaTestEnable(izanagi::S_PES_PASS_STATE& sState, CGstateassignment sa)
    {
        INT num;
        const CGbool* p = ::cgGetBoolStateAssignmentValues(sa, &num);

        BOOL ret = (p != NULL);
        VRETURN(ret);

        if (ret) {
            sState.AlphaTestEnable = *p;
        }
        return ret;
    }

    // 各種ステートをセット
    BOOL _SetStateValue(izanagi::S_PES_PASS_STATE& sState, CGstateassignment sa)
    {
        static struct {
            const char* Name;
            BOOL (*Func)(izanagi::S_PES_PASS_STATE&, CGstateassignment);
        } StateTable[] = {
            {"AlphaBlendEnable", _SetValueAlphaBlendEnable},
            {"AlphaFunc", _SetValueAlphaFunc},
            {"AlphaTestEnable", _SetValueAlphaTestEnable},
        };

        BOOL ret = TRUE;

        CGstate state = ::cgGetStateAssignmentState(sa);
        izanagi::tool::CString str(::cgGetStateName(state));

        for (IZ_UINT i = 0; i < COUNTOF(StateTable); i++) {
            if (str == StateTable[i].Name) {
                ret = (*StateTable[i].Func)(sState, sa);
                break;
            }
        }

        return ret;
    }

    BOOL _GetStateValue(
        std::vector<IZ_UINT>& tvRetValue,
        IZ_PCSTR pszStateName,
        CGstateassignment sa)
    {
        CGstate state = ::cgGetStateAssignmentState(sa);
        izanagi::tool::CString strName(::cgGetStateName(state));

        if (strName == pszStateName) {
            INT num;
            const int* p = ::cgGetIntStateAssignmentValues(sa, &num);
            VRETURN(p != NULL);

            for (INT i = 0; i < num; i++) {
                tvRetValue.push_back(*(p + i));
            }

            return TRUE;
        }

        return FALSE;
    }
}   // namespace

BOOL CPassUtil::SetStateValue(
    izanagi::S_PES_PASS_STATE& sState,
    CGpass pass)
{
    {
        CGstateassignment sa = ::cgGetFirstStateAssignment(pass);

        while (sa != NULL) {
            VRETURN(
                _SetStateValue(
                    sState,
                    sa));

            sa = ::cgGetNextStateAssignment(sa);
        }
    }

    if (sState.AlphaBlendEnable) {
        std::vector<IZ_UINT> tvTmp;
        CGstateassignment sa = ::cgGetFirstStateAssignment(pass);

        izanagi::graph::E_GRAPH_BLEND src = izanagi::graph::E_GRAPH_BLEND_FORCE_INT32;
        izanagi::graph::E_GRAPH_BLEND dst = izanagi::graph::E_GRAPH_BLEND_FORCE_INT32;
        izanagi::graph::E_GRAPH_BLEND_OP op = izanagi::graph::E_GRAPH_BLEND_OP_FORCE_INT32;

        while (sa != NULL) {
            if (_GetStateValue(tvTmp, "BlendOp", sa)) {
                BOOL result = (tvTmp.size() == 2);
                VRETURN(result);

                if (result) {
                    src = static_cast<izanagi::graph::E_GRAPH_BLEND>(tvTmp[0] - 1);
                    dst = static_cast<izanagi::graph::E_GRAPH_BLEND>(tvTmp[1] - 1);
                }

                tvTmp.clear();
            }
            else if (_GetStateValue(tvTmp, "BlendFunc", sa)) {
                BOOL result = (tvTmp.size() == 1);
                VRETURN(result);

                if (result) {
                    op = static_cast<izanagi::graph::E_GRAPH_BLEND_OP>(tvTmp[0] - 1);
                }

                tvTmp.clear();
            }
            
            sa = ::cgGetNextStateAssignment(sa);
        }

        if ((src != izanagi::graph::E_GRAPH_BLEND_FORCE_INT32)
            && (dst != izanagi::graph::E_GRAPH_BLEND_FORCE_INT32)
            && (op != izanagi::graph::E_GRAPH_BLEND_OP_FORCE_INT32))
        {
            sState.AlphaBlendMethod = IZ_GRAPH_ALPHA_BLEND_VAL(op, src, dst);
        }
    }

    return TRUE;
}

BOOL CPassUtil::SetVSType(
    izanagi::S_PES_PASS& sPass,
    CGpass pass)
{
    CGprogram progVS = GetVSProgram(pass);
    VRETURN(progVS != NULL);

    if (progVS != NULL) {
        LPCSTR lpszEntryPoint = ::cgGetProgramString(progVS, CG_PROGRAM_ENTRY);
        sPass.VSType = CPostEffectConvUtil::GetVtxShaderType(lpszEntryPoint);
    }

    return TRUE;
}

namespace {
    CGprogram _GetShaderProgram(
        IZ_PCSTR pszStateName,
        CGpass pass)
    {
        CGstateassignment sa = ::cgGetFirstStateAssignment(pass);

        while (sa != NULL) {
            CGstate state = ::cgGetStateAssignmentState(sa);
            izanagi::tool::CString strName(::cgGetStateName(state));

            if (strName == pszStateName) {
                return ::cgGetProgramStateAssignmentValue(sa);
            }

            sa = ::cgGetNextStateAssignment(sa);
        }

        IZ_ASSERT(IZ_FALSE);
        return NULL;
    }
}   // namespace

CGprogram CPassUtil::GetVSProgram(CGpass pass)
{
    CGprogram ret = _GetShaderProgram(
                        "VertexShader",
                        pass);
    IZ_ASSERT(ret != NULL);
    return ret;
}

CGprogram CPassUtil::GetPSProgram(CGpass pass)
{
    CGprogram ret = _GetShaderProgram(
                        "PixelShader",
                        pass);
    IZ_ASSERT(ret != NULL);
    return ret;
}

namespace {
    inline IZ_PCSTR _GetStringAnnotationValue(
        IZ_PCSTR pszAnn,
        CGpass pass)
    {
        CGannotation ann = ::cgGetFirstPassAnnotation(pass);

        while (ann != NULL) {
            izanagi::tool::CString str(::cgGetAnnotationName(ann));

            if (str == pszAnn) {
                return ::cgGetStringAnnotationValue(ann);
            }

            ann = ::cgGetNextAnnotation(ann);
        }

        return NULL;
    }
}   // namespace

IZ_PCSTR CPassUtil::GetTextureOffsetParameterName(CGpass pass)
{
    return _GetStringAnnotationValue("TextureOffset", pass);
}

IZ_PCSTR CPassUtil::GetRenderTargetName(CGpass pass)
{
    return _GetStringAnnotationValue("RenderColorTarget", pass);
}

void CPassUtil::GetFunctorArgsString(
    CGpass pass,
    std::vector<izanagi::tool::CString>& tvRet)
{
    IZ_PCSTR pszValue = _GetStringAnnotationValue("FunctorArgsS", pass);
    if (pszValue == NULL) {
        return;
    }

    static IZ_CHAR BUF[1024];

    size_t nLen = strlen(pszValue);

    IZ_UINT nPos = 0;

    // ','で区切る
    for (size_t i = 0; i < nLen + 1; i++) {
        IZ_CHAR ch = pszValue[i];

        if ((ch == ',') || (ch == '\0')) {
            BUF[nPos] = '\0';

            izanagi::tool::CString str(BUF);
            tvRet.push_back(str);

            nPos = 0;
        }
        else {
            BUF[nPos++] = ch;
        }
    }
}

IZ_PCSTR CPassUtil::GetFunctorName(CGpass pass)
{
    IZ_PCSTR ret = _GetStringAnnotationValue("FunctorName", pass);
    return ret;
}
