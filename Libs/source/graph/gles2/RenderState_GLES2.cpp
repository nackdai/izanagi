#include "graph/RenderState.h"
#include "graph/ParamValueConverter.h"
#include "graph/GraphDefs.h"

namespace izanagi
{
namespace graph
{
    // レンダーステートをセット
    void CRenderState::SetRenderState(
        CGraphicsDevice* device,
        E_GRAPH_RENDER_STATE nState,
        IZ_DWORD val)
    {
        IZ_ASSERT(nState < E_GRAPH_RS_NUM);

        typedef void (CRenderState::*FUNC)(CGraphicsDevice*, IZ_DWORD);
        static FUNC func[] = {
            &CRenderState::EnableZWrite,            // E_GRAPH_RS_ZWRITEENABLE
            &CRenderState::EnableZTest,             // E_GRAPH_RS_ZENABLE
            &CRenderState::SetZTestFunc,            // E_GRAPH_RS_ZFUNC

            &CRenderState::EnableAlphaTest,         // E_GRAPH_RS_ALPHATESTENABLE
            &CRenderState::SetAlphaTestRef,         // E_GRAPH_RS_ALPHAREF
            &CRenderState::SetAlphaTestFunc,        // E_GRAPH_RS_ALPHAFUNC
            &CRenderState::EnableAlphaBlend,        // E_GRAPH_RS_ALPHABLENDENABLE
            &CRenderState::SetAlphaBlendMethod,     // E_GRAPH_RS_BLENDMETHOD

            &CRenderState::SetFillMode,             // E_GRAPH_RS_FILLMODE
            &CRenderState::SetCullMode,             // E_GRAPH_RS_CULLMODE

            &CRenderState::EnableRenderColorRGB,    // E_GRAPH_RS_COLORWRITEENABLE_RGB
            &CRenderState::EnableRenderColorA,      // E_GRAPH_RS_COLORWRITEENABLE_A
        
            &CRenderState::EnableScissorTest,       // E_GRAPH_RS_SCISSORTESTENABLE

            &CRenderState::EnableStencilTest,       // E_GRAPH_RS_STENCIL_ENABLE
        };
        IZ_C_ASSERT(COUNTOF(func) == E_GRAPH_RS_NUM);

        (this->*func[nState])(device, val);
    }

    // グラフィックスデバイスから現在設定されている値を取得する
    void CRenderState::GetParamsFromGraphicsDevice(CGraphicsDevice* device)
    {
        GLboolean flag = GL_FALSE;
        GLint data = 0;

        // Depth.
        {
            CALL_GL_API(::glGetBooleanv(GL_DEPTH_WRITEMASK, &flag));
            this->isZWriteEnable = flag;

            CALL_GL_API(flag = ::glIsEnabled(GL_DEPTH_TEST));
            this->isZTestEnable = flag;

            CALL_GL_API(::glGetIntegerv(GL_DEPTH_FUNC, &data));
            this->cmpZFunc = CParamValueConverterGLES2::ConvTargetToAbstract_Cmp(data);
        }

        // TODO
        // Alpha test.

        // Alpha blend.
        {
            CALL_GL_API(flag = ::glIsEnabled(GL_BLEND));
            this->isAlphaBlendEnable = flag;

            CALL_GL_API(::glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &data));
            auto op = CParamValueConverterGLES2::ConvTargetToAbstract_BlendOp(data);

            CALL_GL_API(::glGetIntegerv(GL_BLEND_SRC_ALPHA, &data));
            auto src = CParamValueConverterGLES2::ConvTargetToAbstract_Blend(data);

            CALL_GL_API(::glGetIntegerv(GL_BLEND_DST_ALPHA, &data));
            auto dst = CParamValueConverterGLES2::ConvTargetToAbstract_Blend(data);

            this->methodAlphaBlend = IZ_GRAPH_ALPHA_BLEND_VAL(op, src, dst);
        }

        // TODO
        // Fill mode.

        // Cull mode.
        {
            CALL_GL_API(flag = ::glIsEnabled(GL_CULL_FACE));

            if (flag) {
                CALL_GL_API(::glGetIntegerv(GL_CULL_FACE_MODE, &data));
                this->cullMode = CParamValueConverterGLES2::ConvTargetToAbstract_Cull(data);
            }
            else {
                this->cullMode = E_GRAPH_CULL_NONE;
            }
        }

        // Color mask.
        {
            GLboolean masks[4];
            CALL_GL_API(::glGetBooleanv(GL_COLOR_WRITEMASK, masks));
            this->isEnableRenderRGB = masks[0];
            this->isEnableRenderA = masks[3];
        }

        // Scissor.
        CALL_GL_API(flag = ::glIsEnabled(GL_SCISSOR_TEST));
        this->isScissorEnable = flag;

        // Stencil.
        {
            CALL_GL_API(flag = ::glIsEnabled(GL_STENCIL_TEST));
            this->isStencilEnable = flag;

            CALL_GL_API(::glGetIntegerv(GL_STENCIL_FUNC, &data));
            this->stencilParams.func = CParamValueConverterGLES2::ConvTargetToAbstract_Cmp(data);

            CALL_GL_API(::glGetIntegerv(GL_STENCIL_VALUE_MASK, &data));
            this->stencilParams.mask = data;

            CALL_GL_API(::glGetIntegerv(GL_STENCIL_REF, &data));
            this->stencilParams.ref = data;

            CALL_GL_API(::glGetIntegerv(GL_STENCIL_FAIL, &data));
            this->stencilParams.op[0].fail = CParamValueConverterGLES2::ConvTargetToAbstract_Stencil(data);

            CALL_GL_API(::glGetIntegerv(GL_STENCIL_PASS_DEPTH_PASS, &data));
            this->stencilParams.op[0].pass = CParamValueConverterGLES2::ConvTargetToAbstract_Stencil(data);

            CALL_GL_API(::glGetIntegerv(GL_STENCIL_PASS_DEPTH_FAIL, &data));
            this->stencilParams.op[0].zfail = CParamValueConverterGLES2::ConvTargetToAbstract_Stencil(data);

            this->stencilParams.op[1].fail = this->stencilParams.op[0].fail;
            this->stencilParams.op[1].pass = this->stencilParams.op[0].pass;
            this->stencilParams.op[1].zfail = this->stencilParams.op[0].zfail;
        }
    }

    // ビューポート.
    IZ_BOOL CRenderState::SetViewport(CGraphicsDevice* device, const SViewport& _vp)
    {
        if ((vp.width != _vp.width)
            || (vp.height != _vp.height)
            || (vp.x != _vp.x)
            || (vp.y != _vp.y)
            || (vp.minZ != _vp.minZ)
            || (vp.maxZ != _vp.maxZ))
        {
            CALL_GL_API(::glViewport(
                _vp.x,
                _vp.y,
                _vp.width,
                _vp.height));

            CALL_GL_API(::glDepthRangef(
                _vp.minZ,
                _vp.maxZ));

            vp = _vp;
        }

        return IZ_TRUE;
    }

    // 深度値描き込み有効・無効
    void CRenderState::EnableZWrite(CGraphicsDevice* device, IZ_DWORD flag)
    {
        if (isZWriteEnable != flag) {
            CALL_GL_API(::glDepthMask(flag));
            isZWriteEnable = flag;
        }
    }

    // 深度テスト有効・無効
    void CRenderState::EnableZTest(CGraphicsDevice* device, IZ_DWORD flag)
    {
        if (isZTestEnable != flag) {
            if (flag) {
                CALL_GL_API(::glEnable(GL_DEPTH_TEST));
            }
            else {
                CALL_GL_API(::glDisable(GL_DEPTH_TEST));
            }
            isZTestEnable = flag;
        }
    }

    // 深度テスト判定方法
    void CRenderState::SetZTestFunc(CGraphicsDevice* device, IZ_DWORD func)
    {
        if (cmpZFunc != func) {
            auto glFunc = CParamValueConverterGLES2::ConvAbstractToTarget_Cmp((izanagi::graph::E_GRAPH_CMP_FUNC)func);

            CALL_GL_API(::glDepthFunc(glFunc));

            cmpZFunc = func;
        }
    }

    // アルファテスト有効・無効
    void CRenderState::EnableAlphaTest(CGraphicsDevice* device, IZ_DWORD flag)
    {
        // NOTE
        // There is no alpha test in GLES2...
    }

    // アルファテスト基準値
    void CRenderState::SetAlphaTestRef(CGraphicsDevice* device, IZ_DWORD ref)
    {
        // NOTE
        // There is no alpha test in GLES2...
    }

    // アルファテスト判定方法
    void CRenderState::SetAlphaTestFunc(CGraphicsDevice* device, IZ_DWORD func)
    {
        // NOTE
        // There is no alpha test in GLES2...
    }

    // アルファブレンド有効・無効
    void CRenderState::EnableAlphaBlend(CGraphicsDevice* device, IZ_DWORD flag)
    {
        if (isAlphaBlendEnable != flag) {
            if (flag) {
                CALL_GL_API(::glEnable(GL_BLEND));
            }
            else {
                CALL_GL_API(::glDisable(GL_BLEND));
            }
            isAlphaBlendEnable = flag;
        }
    }

    // アルファブレンド方法
    void CRenderState::SetAlphaBlendMethod(CGraphicsDevice* device, IZ_DWORD method)
    {
        // 新しい設定値
        IZ_DWORD newOp = IZ_GRAPH_GET_ALPHA_BLEND_OP(method);
        IZ_DWORD newSrc = IZ_GRAPH_GET_ALPHA_BLEND_SRC(method);
        IZ_DWORD newDst = IZ_GRAPH_GET_ALPHA_BLEND_DST(method);

        // 現在の設定値
        IZ_DWORD curOp = IZ_GRAPH_GET_ALPHA_BLEND_OP(methodAlphaBlend);
        IZ_DWORD curSrc = IZ_GRAPH_GET_ALPHA_BLEND_SRC(methodAlphaBlend);
        IZ_DWORD curDst = IZ_GRAPH_GET_ALPHA_BLEND_DST(methodAlphaBlend);

        if (newOp != curOp) {
            GLenum glOp = CTargetParamValueConverter::ConvAbstractToTarget_BlendOp((E_GRAPH_BLEND_OP)newOp);

            CALL_GL_API(::glBlendEquation(glOp));
        }

        if (newSrc != curSrc
            || newDst != curDst)
        {
            GLenum glSrc = CTargetParamValueConverter::ConvAbstractToTarget_Blend((E_GRAPH_BLEND)newSrc);
            GLenum glDst = CTargetParamValueConverter::ConvAbstractToTarget_Blend((E_GRAPH_BLEND)newDst);

            CALL_GL_API(::glBlendFunc(glSrc, glDst));
        }

        // 更新
        methodAlphaBlend = IZ_GRAPH_ALPHA_BLEND_VAL(newOp, newSrc, newDst);
    }

    // フィルモード
    void CRenderState::SetFillMode(CGraphicsDevice* device, IZ_DWORD fill)
    {
        // NOTE
        // There is no fill mode in GLES2...
    }

    // カリングモード
    void CRenderState::SetCullMode(CGraphicsDevice* device, IZ_DWORD cull)
    {
        if (cull == E_GRAPH_CULL_NONE) {
            CALL_GL_API(::glDisable(GL_CULL_FACE));
        }
        else {
            CALL_GL_API(::glEnable(GL_CULL_FACE));
        }

        if (cullMode != cull) {
            GLenum raelCull = IZ_GET_TARGET_CULL((E_GRAPH_CULL)cull);

            CALL_GL_API(::glCullFace(raelCull));
            cullMode = cull;
        }
    }

    // レンダーターゲットのカラーバッファの描き込み設定
    void CRenderState::EnableRenderColorRGB(CGraphicsDevice* device, IZ_DWORD enableRGB)
    {
        if (isEnableRenderRGB != enableRGB) {
            CALL_GL_API(::glColorMask(
                enableRGB,
                enableRGB,
                enableRGB,
                isEnableRenderA));

            isEnableRenderRGB = enableRGB;
        }
    }

    void CRenderState::EnableRenderColorA(CGraphicsDevice* device, IZ_DWORD enableA)
    {
        if (isEnableRenderA != enableA) {
            CALL_GL_API(::glColorMask(
                isEnableRenderRGB,
                isEnableRenderRGB,
                isEnableRenderRGB,
                enableA));

            isEnableRenderA = enableA;
        }
    }

    // シザーテスト
    void CRenderState::EnableScissorTest(CGraphicsDevice* device, IZ_DWORD flag)
    {
        if (isScissorEnable != flag) {
            if (flag) {
                CALL_GL_API(::glEnable(GL_SCISSOR_TEST));
            }
            else {
                CALL_GL_API(::glDisable(GL_SCISSOR_TEST));
            }
            isScissorEnable = flag;
        }
    }

    void CRenderState::SetScissorRect(CGraphicsDevice* device, const SIntRect& rc)
    {
        if (rcScissor != rc) {
            CALL_GL_API(::glScissor(
                rc.left,
                rc.top,
                rc.width,
                rc.height));

            rcScissor = rc;
        }
    }

    void CRenderState::EnableStencilTest(CGraphicsDevice* device, IZ_DWORD flag)
    {
        if (isStencilEnable != flag) {
            if (flag) {
                CALL_GL_API(::glEnable(GL_STENCIL_TEST));
            }
            else {
                CALL_GL_API(::glDisable(GL_STENCIL_TEST));
            }

            isStencilEnable = flag;
        }
    }

    // Sets stencil function.
    void CRenderState::SetStencilFunc(
        CGraphicsDevice* device,
        E_GRAPH_CMP_FUNC cmp,
        IZ_INT ref,
        IZ_DWORD mask)
    {
        if (stencilParams.func != cmp
            || stencilParams.ref != ref
            || stencilParams.mask != mask)
        {
            stencilParams.func = cmp;
            stencilParams.ref = ref;
            stencilParams.mask = mask;

            auto glCmp = CParamValueConverterGLES2::ConvAbstractToTarget_Cmp(cmp);

            CALL_GL_API(::glStencilFuncSeparate(
                GL_FRONT_AND_BACK,
                glCmp, ref, mask));
        }
    }

    // Sets stencil operations.
    void CRenderState::SetStencilOp(
        CGraphicsDevice* device,
        IZ_BOOL isFront,
        E_GRAPH_STENCIL_OP pass,
        E_GRAPH_STENCIL_OP zfail,
        E_GRAPH_STENCIL_OP fail)
    {
        IZ_UINT idx = (isFront ? 0 : 1);
        auto& op = stencilParams.op[idx];

        if (op.pass != pass
            || op.zfail != zfail
            || op.fail != fail)
        {
            op.pass = pass;
            op.zfail = zfail;
            op.fail = fail;

            auto dppass = CParamValueConverterGLES2::ConvAbstractToTarget_Stencil(pass);
            auto dpfail = CParamValueConverterGLES2::ConvAbstractToTarget_Stencil(zfail);
            auto sfail = CParamValueConverterGLES2::ConvAbstractToTarget_Stencil(fail);

            // NOTE
            // glFrontFace(GL_CCW)で面の向きをOpenGLデフォルトと反対にしているので、BACKとFRONTを逆に扱う.
            CALL_GL_API(::glStencilOpSeparate(
                isFront ? GL_BACK : GL_FRONT,
                sfail, dpfail, dppass));
        }
    }
}   // namespace graph
}   // namespace izanagi
