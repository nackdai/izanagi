#include "graph/RenderState.h"
#include "graph/internal/ParamValueConverter.h"
#include "graph/gles2/izGLES2Defs.h"

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
        };
        IZ_C_ASSERT(COUNTOF(func) == E_GRAPH_RS_NUM);

        (this->*func[nState])(device, val);
    }

    // グラフィックスデバイスから現在設定されている値を取得する
    void CRenderState::GetParamsFromGraphicsDevice(CGraphicsDevice* device)
    {
        // TODO
    }

    // 深度値描き込み有効・無効
    void CRenderState::EnableZWrite(CGraphicsDevice* device, IZ_DWORD flag)
    {
        if (isZWriteEnable != flag) {
            ::glDepthMask(flag);
            isZWriteEnable = flag;
        }
    }

    // 深度テスト有効・無効
    void CRenderState::EnableZTest(CGraphicsDevice* device, IZ_DWORD flag)
    {
        if (isZTestEnable != flag) {
            if (flag) {
                ::glEnable(GL_DEPTH_TEST);
            }
            else {
                ::glDisable(GL_DEPTH_TEST);
            }
            isZTestEnable = flag;
        }
    }

    // 深度テスト判定方法
    void CRenderState::SetZTestFunc(CGraphicsDevice* device, IZ_DWORD func)
    {
        if (cmpZFunc != func) {
            // TODO

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
                ::glEnable(GL_BLEND);
            }
            else {
                ::glDisable(GL_BLEND);
            }
        }
    }

    // アルファブレンド方法
    void CRenderState::SetAlphaBlendMethod(CGraphicsDevice* device, IZ_DWORD method)
    {
        // 新しい設定値
        IZ_DWORD nNewOp = IZ_GRAPH_GET_ALPHA_BLEND_OP(method);
        IZ_DWORD nNewSrc = IZ_GRAPH_GET_ALPHA_BLEND_SRC(method);
        IZ_DWORD nNewDst = IZ_GRAPH_GET_ALPHA_BLEND_DST(method);

        // 現在の設定値
        IZ_DWORD nCurOp = IZ_GRAPH_GET_ALPHA_BLEND_OP(methodAlphaBlend);
        IZ_DWORD nCurSrc = IZ_GRAPH_GET_ALPHA_BLEND_SRC(methodAlphaBlend);
        IZ_DWORD nCurDst = IZ_GRAPH_GET_ALPHA_BLEND_DST(methodAlphaBlend);

        // TODO

        // 更新
        methodAlphaBlend = IZ_GRAPH_ALPHA_BLEND_VAL(nNewOp, nNewSrc, nNewDst);
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
        if (cullMode != cull) {
            GLenum raelCull = IZ_GET_TARGET_CULL((E_GRAPH_CULL)cull);

            ::glCullFace(raelCull);
            cullMode = cull;
        }
    }

    // レンダーターゲットのカラーバッファの描き込み設定
    void CRenderState::EnableRenderColorRGB(CGraphicsDevice* device, IZ_DWORD enableRGB)
    {
        if (isEnableRenderRGB != enableRGB) {
            ::glColorMask(
                enableRGB,
                enableRGB,
                enableRGB,
                isEnableRenderA);

            isEnableRenderRGB = enableRGB;
        }
    }

    void CRenderState::EnableRenderColorA(CGraphicsDevice* device, IZ_DWORD enableA)
    {
        if (isEnableRenderA != enableA) {
            ::glColorMask(
                isEnableRenderRGB,
                isEnableRenderRGB,
                isEnableRenderRGB,
                enableA);

            isEnableRenderA = enableA;
        }
    }

    // シザーテスト
    void CRenderState::EnableScissorTest(CGraphicsDevice* device, IZ_DWORD flag)
    {
        if (isScissorEnable != flag) {
            if (flag) {
                ::glEnable(GL_SCISSOR_TEST);
            }
            else {
                ::glDisable(GL_SCISSOR_TEST);
            }
            isScissorEnable = flag;
        }
    }

    void CRenderState::SetScissorRect(CGraphicsDevice* device, const SIntRect& rc)
    {
        // 念のためこれくらいはチェックするか
        IZ_C_ASSERT(sizeof(RECT) == sizeof(CIntRect));

        if (rcScissor != rc) {
            ::glScissor(
                rc.left,
                rc.top,
                rc.right - rc.left,
                rc.bottom - rc.top);

            rcScissor = rc;
        }
    }
}   // namespace graph
}   // namespace izanagi
