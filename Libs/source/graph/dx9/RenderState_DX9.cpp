#include "graph/RenderState.h"
#include "graph/ParamValueConverter.h"
#include "graph/dx9/GraphicsDevice_DX9.h"

namespace izanagi
{
namespace graph
{
    namespace {
        // レンダーステート設定
        template <typename _RS, typename _T, typename _U, typename _V>
        void _SetRenderState(
            CGraphicsDevice* device,
            _RS nRSType,
            _T& old_val, _T new_val,
            _U (*funcConv)(_V))
        {
            IZ_ASSERT(device != IZ_NULL);

            CGraphicsDeviceDX9* dx9Device = reinterpret_cast<CGraphicsDeviceDX9*>(device);
            D3D_DEVICE* d3dDev = dx9Device->GetRawInterface();

            if (old_val != new_val) {
                _U val = (*funcConv)((_V)new_val);
                d3dDev->SetRenderState(nRSType, val);
                old_val = new_val;
            }
        }

        template <typename _RS, typename _T>
        void _SetRenderState(
            CGraphicsDevice* device,
            _RS nRSType,
            _T& old_val, _T new_val)
        {
            IZ_ASSERT(device != IZ_NULL);

            CGraphicsDeviceDX9* dx9Device = reinterpret_cast<CGraphicsDeviceDX9*>(device);
            D3D_DEVICE* d3dDev = dx9Device->GetRawInterface();

            if (old_val != new_val) {
                HRESULT hr = d3dDev->SetRenderState(nRSType, new_val);
                IZ_ASSERT(SUCCEEDED(hr));
                old_val = new_val;
            }
        }
    }   // namespace

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

    #define _CHECK(f)   { HRESULT hr = f; IZ_ASSERT(SUCCEEDED(hr)); }

    // グラフィックスデバイスから現在設定されている値を取得する
    void CRenderState::GetParamsFromGraphicsDevice(CGraphicsDevice* device)
    {
        CGraphicsDeviceDX9* dx9Device = reinterpret_cast<CGraphicsDeviceDX9*>(device);
        D3D_DEVICE* d3dDev = dx9Device->GetRawInterface();

        // 深度
        {
            _CHECK(d3dDev->GetRenderState(D3DRS_ZWRITEENABLE, (DWORD*)&isZWriteEnable));
            _CHECK(d3dDev->GetRenderState(D3DRS_ZENABLE, (DWORD*)&isZTestEnable));
            _CHECK(d3dDev->GetRenderState(D3DRS_ZFUNC, (DWORD*)&cmpZFunc));

            cmpZFunc = IZ_GET_ABST_CMP((D3DCMPFUNC)cmpZFunc);
        }

        // アルファテスト
        {
            _CHECK(d3dDev->GetRenderState(D3DRS_ALPHATESTENABLE, (DWORD*)&isAlphaTestEnable));
            _CHECK(d3dDev->GetRenderState(D3DRS_ALPHAREF, (DWORD*)&alphaRef));
            _CHECK(d3dDev->GetRenderState(D3DRS_ALPHAFUNC, (DWORD*)&cmpAlphaFunc));

            cmpAlphaFunc = IZ_GET_ABST_CMP((D3DCMPFUNC)cmpAlphaFunc);
        }

        // アルファブレンド
        _CHECK(d3dDev->GetRenderState(D3DRS_ALPHABLENDENABLE, (DWORD*)&isAlphaBlendEnable));

        // ブレンド方法
        {
            IZ_DWORD nOp, nSrc, nDst;
            _CHECK(d3dDev->GetRenderState(D3DRS_BLENDOP, (DWORD*)&nOp));
            _CHECK(d3dDev->GetRenderState(D3DRS_SRCBLEND, (DWORD*)&nSrc));
            _CHECK(d3dDev->GetRenderState(D3DRS_DESTBLEND, (DWORD*)&nDst));

            nOp = IZ_GET_ABST_BLEND_OP((D3DBLENDOP)nOp);
            nSrc = IZ_GET_ABST_BLEND((D3DBLEND)nSrc);
            nDst = IZ_GET_ABST_BLEND((D3DBLEND)nDst);

            methodAlphaBlend = IZ_GRAPH_ALPHA_BLEND_VAL(nOp, nSrc, nDst);
        }

        // 描画モード
        {
            _CHECK(d3dDev->GetRenderState(D3DRS_FILLMODE, (DWORD*)&fillMode));
            _CHECK(d3dDev->GetRenderState(D3DRS_CULLMODE, (DWORD*)&cullMode));

            fillMode = IZ_GET_ABST_FILL_MODE((D3DFILLMODE)fillMode);
            cullMode = IZ_GET_ABST_CULL((D3DCULL)cullMode);
        }

        // シザー矩形
        {
            _CHECK(d3dDev->GetRenderState(D3DRS_SCISSORTESTENABLE, (DWORD*)&isScissorEnable));
            _CHECK(d3dDev->GetScissorRect((RECT*)&rcScissor));
        }

        // カラー描画有効・無効
        {
            IZ_DWORD nColorWriteFlag;
            _CHECK(d3dDev->GetRenderState(D3DRS_COLORWRITEENABLE, (DWORD*)&nColorWriteFlag));
            isEnableRenderRGB = (nColorWriteFlag & (D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_RED));
            isEnableRenderA = (nColorWriteFlag & D3DCOLORWRITEENABLE_ALPHA);
        }

        // ステンシル
        {
            _CHECK(d3dDev->GetRenderState(D3DRS_STENCILENABLE, (DWORD*)&isStencilEnable));

            IZ_DWORD stencilFunc;
            IZ_DWORD stencilRef;
            IZ_DWORD stencilMask;
            IZ_DWORD stencilOpPass;
            IZ_DWORD stencilOpZFail;
            IZ_DWORD stencilOpFail;
            _CHECK(d3dDev->GetRenderState(D3DRS_STENCILFUNC, (DWORD*)&stencilFunc));
            _CHECK(d3dDev->GetRenderState(D3DRS_STENCILREF, (DWORD*)&stencilRef));
            _CHECK(d3dDev->GetRenderState(D3DRS_STENCILMASK, (DWORD*)&stencilMask));
            _CHECK(d3dDev->GetRenderState(D3DRS_STENCILPASS, (DWORD*)&stencilOpPass));
            _CHECK(d3dDev->GetRenderState(D3DRS_STENCILZFAIL, (DWORD*)&stencilOpZFail));
            _CHECK(d3dDev->GetRenderState(D3DRS_STENCILFAIL, (DWORD*)&stencilOpFail));

            stencilParams.func = IZ_GET_ABST_CMP((D3DCMPFUNC)stencilFunc);
            stencilParams.ref = stencilRef;
            stencilParams.mask = stencilMask;
            stencilParams.op[0].pass = IZ_GET_ABST_STENCIL_OP((D3DSTENCILOP)stencilOpPass);
            stencilParams.op[0].zfail = IZ_GET_ABST_STENCIL_OP((D3DSTENCILOP)stencilOpZFail);
            stencilParams.op[0].fail = IZ_GET_ABST_STENCIL_OP((D3DSTENCILOP)stencilOpFail);

            stencilParams.op[1].pass = stencilParams.op[0].pass;
            stencilParams.op[1].zfail = stencilParams.op[0].zfail;
            stencilParams.op[1].fail = stencilParams.op[0].fail;
        }
    }

    // ビューポート.
    IZ_BOOL CRenderState::SetViewport(CGraphicsDevice* device, const SViewport& _vp)
    {
        IZ_BOOL ret = IZ_TRUE;

        if ((vp.width != _vp.width)
            || (vp.height != _vp.height)
            || (vp.x != _vp.x)
            || (vp.y != _vp.y)
            || (vp.minZ != _vp.minZ)
            || (vp.maxZ != _vp.maxZ))
        {
            D3D_VIEWPORT sD3DViewport;
            {
                sD3DViewport.X = _vp.x;
                sD3DViewport.Y = _vp.y;
                sD3DViewport.Width = _vp.width;
                sD3DViewport.Height = _vp.height;
                sD3DViewport.MinZ = _vp.minZ;
                sD3DViewport.MaxZ = _vp.maxZ;
            }

            CGraphicsDeviceDX9* dx9Device = reinterpret_cast<CGraphicsDeviceDX9*>(device);
            D3D_DEVICE* d3dDev = dx9Device->GetRawInterface();

            auto hr = d3dDev->SetViewport(&sD3DViewport);

            ret = SUCCEEDED(hr);
            if (ret) {
                vp = _vp;
            }
        }

        return ret;
    }

    // 深度値描き込み有効・無効
    void CRenderState::EnableZWrite(CGraphicsDevice* device, IZ_DWORD flag)
    {
        _SetRenderState(
            device,
            D3DRS_ZWRITEENABLE,
            isZWriteEnable, flag);
    }

    // 深度テスト有効・無効
    void CRenderState::EnableZTest(CGraphicsDevice* device, IZ_DWORD flag)
    {
        _SetRenderState(
            device,
            D3DRS_ZENABLE,
            isZTestEnable, flag);
    }

    // 深度テスト判定方法
    void CRenderState::SetZTestFunc(CGraphicsDevice* device, IZ_DWORD func)
    {
        _SetRenderState(
            device,
            D3DRS_ZFUNC,
            cmpZFunc, func,
            IZ_CONV_PARAM_TO_TARGET(Cmp));
    }

    // アルファテスト有効・無効
    void CRenderState::EnableAlphaTest(CGraphicsDevice* device, IZ_DWORD flag)
    {
        _SetRenderState(
            device,
            D3DRS_ALPHATESTENABLE,
            isAlphaTestEnable, flag);
    }

    // アルファテスト基準値
    void CRenderState::SetAlphaTestRef(CGraphicsDevice* device, IZ_DWORD ref)
    {
        _SetRenderState(
            device,
            D3DRS_ALPHAREF,
            alphaRef, ref);
    }

    // アルファテスト判定方法
    void CRenderState::SetAlphaTestFunc(CGraphicsDevice* device, IZ_DWORD func)
    {
        _SetRenderState(
            device,
            D3DRS_ALPHAFUNC,
            cmpAlphaFunc, func,
            IZ_CONV_PARAM_TO_TARGET(Cmp));
    }

    // アルファブレンド有効・無効
    void CRenderState::EnableAlphaBlend(CGraphicsDevice* device, IZ_DWORD flag)
    {
        _SetRenderState(
            device,
            D3DRS_ALPHABLENDENABLE ,
            isAlphaBlendEnable, flag);
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

        _SetRenderState(
            device,
            D3DRS_BLENDOP,
            nCurOp, nNewOp,
            IZ_CONV_PARAM_TO_TARGET(BlendOp));
        _SetRenderState(
            device,
            D3DRS_SRCBLEND,
            nCurSrc, nNewSrc,
            IZ_CONV_PARAM_TO_TARGET(Blend));
        _SetRenderState(
            device,
            D3DRS_DESTBLEND,
            nCurDst, nNewDst,
            IZ_CONV_PARAM_TO_TARGET(Blend));

        // 更新
        methodAlphaBlend = IZ_GRAPH_ALPHA_BLEND_VAL(nNewOp, nNewSrc, nNewDst);
    }

    // フィルモード
    void CRenderState::SetFillMode(CGraphicsDevice* device, IZ_DWORD fill)
    {
        _SetRenderState(
            device,
            D3DRS_FILLMODE,
            fillMode, fill,
            IZ_CONV_PARAM_TO_TARGET(FillMode));
    }

    // カリングモード
    void CRenderState::SetCullMode(CGraphicsDevice* device, IZ_DWORD cull)
    {
        _SetRenderState(
            device,
            D3DRS_CULLMODE,
            cullMode, cull,
            IZ_CONV_PARAM_TO_TARGET(Cull));
    }

    #define GET_RGB_FLAG(b) (b ? (D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_RED) : 0)
    #define GET_A_FLAG(b)   (b ? D3DCOLORWRITEENABLE_ALPHA : 0)

    // レンダーターゲットのカラーバッファの描き込み設定
    void CRenderState::EnableRenderColorRGB(CGraphicsDevice* device, IZ_DWORD enableRGB)
    {
        // 新しい設定値
        IZ_DWORD nNewFlag = GET_RGB_FLAG(enableRGB);
        nNewFlag |= GET_A_FLAG(isEnableRenderA);

        // 現在の設定値
        IZ_DWORD nCurFlag = GET_RGB_FLAG(isEnableRenderRGB);
        nCurFlag |= GET_A_FLAG(isEnableRenderA);

        _SetRenderState(
            device,
            D3DRS_COLORWRITEENABLE,
            nCurFlag, nNewFlag);

        // 更新
        isEnableRenderRGB = enableRGB;
    }

    void CRenderState::EnableRenderColorA(CGraphicsDevice* device, IZ_DWORD enableA)
    {
        // 新しい設定値
        IZ_DWORD nNewFlag = GET_RGB_FLAG(isEnableRenderRGB);
        nNewFlag |= GET_A_FLAG(enableA);

        // 現在の設定値
        IZ_DWORD nCurFlag = GET_RGB_FLAG(isEnableRenderRGB);
        nCurFlag |= GET_A_FLAG(isEnableRenderA);

        _SetRenderState(
            device,
            D3DRS_COLORWRITEENABLE,
            nCurFlag, nNewFlag);

        // 更新
        isEnableRenderA = enableA;
    }

    // シザーテスト
    void CRenderState::EnableScissorTest(CGraphicsDevice* device, IZ_DWORD flag)
    {
        _SetRenderState(
            device,
            D3DRS_SCISSORTESTENABLE,
            isScissorEnable, flag);
    }

    void CRenderState::SetScissorRect(CGraphicsDevice* device, const SIntRect& rc)
    {
        // 念のためこれくらいはチェックするか
        IZ_C_ASSERT(sizeof(RECT) == sizeof(CIntRect));

        CGraphicsDeviceDX9* dx9Device = reinterpret_cast<CGraphicsDeviceDX9*>(device);

        if (rcScissor != rc) {
            rcScissor = rc;
            D3D_DEVICE* d3DDev = dx9Device->GetRawInterface();

            RECT rcDX9;
            rcDX9.left = rcScissor.left;
            rcDX9.top = rcScissor.top;
            rcDX9.right = rcScissor.left + rcScissor.width;
            rcDX9.bottom = rcScissor.top + rcScissor.height;

            d3DDev->SetScissorRect(&rcDX9);
        }
    }

    // ステンシル.
    void CRenderState::EnableStencilTest(CGraphicsDevice* device, IZ_DWORD flag)
    {
#if 0
        _SetRenderState(
            device,
            D3DRS_STENCILENABLE,
            isStencilEnable, flag);
#else
        CGraphicsDeviceDX9* dx9Device = reinterpret_cast<CGraphicsDeviceDX9*>(device);
        D3D_DEVICE* d3dDev = dx9Device->GetRawInterface();

        if (isStencilEnable != flag) {
            HRESULT hr = d3dDev->SetRenderState(D3DRS_STENCILENABLE, flag);
            IZ_ASSERT(SUCCEEDED(hr));

            hr = d3dDev->SetRenderState(D3DRS_TWOSIDEDSTENCILMODE, flag);
            IZ_ASSERT(SUCCEEDED(hr));

            isStencilEnable = flag;
        }
#endif
    }

    // Sets stencil function.
    void CRenderState::SetStencilFunc(
        CGraphicsDevice* device,
        E_GRAPH_CMP_FUNC cmp,
        IZ_INT ref,
        IZ_DWORD mask)
    {
        CGraphicsDeviceDX9* dx9Device = reinterpret_cast<CGraphicsDeviceDX9*>(device);
        D3D_DEVICE* d3DDev = dx9Device->GetRawInterface();

        if (stencilParams.func != cmp) {
            stencilParams.func = cmp;
            auto d3dCmp = CD3D9ParamValueConverter::ConvAbstractToTarget_Cmp(cmp);

            // 両面を設定.
            d3DDev->SetRenderState(
                D3DRS_STENCILFUNC,
                d3dCmp);
            d3DDev->SetRenderState(
                D3DRS_CCW_STENCILFUNC,
                d3dCmp);
        }

        if (stencilParams.ref != ref) {
            stencilParams.ref = ref;
            d3DDev->SetRenderState(D3DRS_STENCILREF, ref);
            d3DDev->SetRenderState(D3DRS_STENCILREF, ref);
        }

        if (stencilParams.mask != mask) {
            stencilParams.mask = mask;
            d3DDev->SetRenderState(D3DRS_STENCILMASK, mask);
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

        CGraphicsDeviceDX9* dx9Device = reinterpret_cast<CGraphicsDeviceDX9*>(device);
        D3D_DEVICE* d3DDev = dx9Device->GetRawInterface();

        if (op.pass != pass) {
            op.pass = pass;
            auto d3dPass = CD3D9ParamValueConverter::ConvAbstractToTarget_Stencil(pass);
            d3DDev->SetRenderState(
                isFront ? D3DRS_STENCILPASS : D3DRS_CCW_STENCILPASS,
                d3dPass);
        }

        if (op.zfail != zfail) {
            op.zfail = zfail;
            auto d3dZFail = CD3D9ParamValueConverter::ConvAbstractToTarget_Stencil(zfail);
            d3DDev->SetRenderState(
                isFront ? D3DRS_STENCILZFAIL : D3DRS_CCW_STENCILZFAIL,
                d3dZFail);
        }

        if (op.fail != fail) {
            op.fail = fail;
            auto d3dFail = CD3D9ParamValueConverter::ConvAbstractToTarget_Stencil(fail);
            d3DDev->SetRenderState(
                isFront ? D3DRS_STENCILFAIL : D3DRS_CCW_STENCILZFAIL,
                d3dFail);
        }
    }
}   // namespace graph
}   // namespace izanagi
