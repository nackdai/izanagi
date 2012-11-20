#include "graph/RenderState.h"
#include "graph/internal/ParamValueConverter.h"
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
			    d3dDev->SetRenderState(nRSType, new_val);
			    old_val = new_val;
		    }
	    }
    }	// namespace

    // レンダーステートをセット
    void CRenderState::SetRenderState(
	    CGraphicsDevice* device,
	    E_GRAPH_RENDER_STATE nState,
	    IZ_DWORD val)
    {
	    IZ_ASSERT(nState < E_GRAPH_RS_NUM);

	    typedef void (CRenderState::*FUNC)(CGraphicsDevice*, IZ_DWORD);
	    static FUNC func[] = {
		    &CRenderState::EnableZWrite,			// E_GRAPH_RS_ZWRITEENABLE
		    &CRenderState::EnableZTest,				// E_GRAPH_RS_ZENABLE
		    &CRenderState::SetZTestFunc,			// E_GRAPH_RS_ZFUNC

		    &CRenderState::EnableAlphaTest,			// E_GRAPH_RS_ALPHATESTENABLE
		    &CRenderState::SetAlphaTestRef,			// E_GRAPH_RS_ALPHAREF
		    &CRenderState::SetAlphaTestFunc,		// E_GRAPH_RS_ALPHAFUNC
		    &CRenderState::EnableAlphaBlend,		// E_GRAPH_RS_ALPHABLENDENABLE
		    &CRenderState::SetAlphaBlendMethod,		// E_GRAPH_RS_BLENDMETHOD

		    &CRenderState::SetFillMode,				// E_GRAPH_RS_FILLMODE
		    &CRenderState::SetCullMode,				// E_GRAPH_RS_CULLMODE

		    &CRenderState::EnableRenderColorRGB,	// E_GRAPH_RS_COLORWRITEENABLE_RGB
		    &CRenderState::EnableRenderColorA,		// E_GRAPH_RS_COLORWRITEENABLE_A
		
		    &CRenderState::EnableScissorTest,		// E_GRAPH_RS_SCISSORTESTENABLE
	    };
	    IZ_C_ASSERT(COUNTOF(func) == E_GRAPH_RS_NUM);

	    (this->*func[nState])(device, val);
    }

    #define _CHECK(f)	{ HRESULT hr = f; IZ_ASSERT(SUCCEEDED(hr)); }

    // グラフィックスデバイスから現在設定されている値を取得する
    void CRenderState::GetParamsFromGraphicsDevice(CGraphicsDevice* device)
    {
        CGraphicsDeviceDX9* dx9Device = reinterpret_cast<CGraphicsDeviceDX9*>(device);
	    D3D_DEVICE* d3dDev = dx9Device->GetRawInterface();

	    // 深度
	    _CHECK(d3dDev->GetRenderState(D3DRS_ZWRITEENABLE, (DWORD*)&isZWriteEnable));
	    _CHECK(d3dDev->GetRenderState(D3DRS_ZENABLE,      (DWORD*)&isZTestEnable));
	    _CHECK(d3dDev->GetRenderState(D3DRS_ZFUNC,        (DWORD*)&cmpZFunc));

	    cmpZFunc = IZ_GET_ABST_CMP((D3DCMPFUNC)cmpZFunc);
	
	    // アルファテスト
	    _CHECK(d3dDev->GetRenderState(D3DRS_ALPHATESTENABLE,  (DWORD*)&isAlphaTestEnable));
	    _CHECK(d3dDev->GetRenderState(D3DRS_ALPHAREF,         (DWORD*)&alphaRef));
	    _CHECK(d3dDev->GetRenderState(D3DRS_ALPHAFUNC,        (DWORD*)&cmpAlphaFunc));

	    cmpAlphaFunc = IZ_GET_ABST_CMP((D3DCMPFUNC)cmpAlphaFunc);

	    // アルファブレンド
	    _CHECK(d3dDev->GetRenderState(D3DRS_ALPHABLENDENABLE, (DWORD*)&isAlphaBlendEnable));

	    // ブレンド方法
	    IZ_DWORD nOp, nSrc, nDst;
	    _CHECK(d3dDev->GetRenderState(D3DRS_BLENDOP,   (DWORD*)&nOp));
	    _CHECK(d3dDev->GetRenderState(D3DRS_SRCBLEND,  (DWORD*)&nSrc));
	    _CHECK(d3dDev->GetRenderState(D3DRS_DESTBLEND, (DWORD*)&nDst));

	    nOp = IZ_GET_ABST_BLEND_OP((D3DBLENDOP)nOp);
	    nSrc = IZ_GET_ABST_BLEND((D3DBLEND)nSrc);
	    nDst = IZ_GET_ABST_BLEND((D3DBLEND)nDst);

	    methodAlphaBlend = IZ_GRAPH_ALPHA_BLEND_VAL(nOp, nSrc, nDst);

	    // 描画モード
	    _CHECK(d3dDev->GetRenderState(D3DRS_FILLMODE, (DWORD*)&fillMode));
	    _CHECK(d3dDev->GetRenderState(D3DRS_CULLMODE, (DWORD*)&cullMode));

	    fillMode = IZ_GET_ABST_FILL_MODE((D3DFILLMODE)fillMode);
	    cullMode = IZ_GET_ABST_CULL((D3DCULL)cullMode);

	    // シザー矩形
	    _CHECK(d3dDev->GetRenderState(D3DRS_SCISSORTESTENABLE, (DWORD*)&isScissorEnable));
	    _CHECK(d3dDev->GetScissorRect((RECT*)&rcScissor));

	    // カラー描画有効・無効
	    IZ_DWORD nColorWriteFlag;
	    _CHECK(d3dDev->GetRenderState(D3DRS_COLORWRITEENABLE, (DWORD*)&nColorWriteFlag));
	    isEnableRenderRGB = (nColorWriteFlag & (D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_RED));
	    isEnableRenderA = (nColorWriteFlag & D3DCOLORWRITEENABLE_ALPHA);
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

    #define GET_RGB_FLAG(b)	(b ? (D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_RED) : 0)
    #define GET_A_FLAG(b)	(b ? D3DCOLORWRITEENABLE_ALPHA : 0)

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
	    nCurFlag |= GET_A_FLAG(enableA);

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
			d3DDev->SetScissorRect(reinterpret_cast<RECT*>(&rcScissor));
		}
    }
}   // namespace graph
}   // namespace izanagi
