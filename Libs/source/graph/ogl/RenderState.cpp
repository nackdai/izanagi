#include "graph/dx9/RenderState.h"
#include "graph/dx9/GraphicsDevice.h"

using namespace izanagi;

namespace {
	// レンダーステート設定
	template <typename _RS, typename _T>
	void _SetRenderState(
		CGraphicsDevice* pDevice,
		_RS nRSType,
		_T& old_val, _T new_val)
	{
		IZ_ASSERT(pDevice != IZ_NULL);

		D3D_DEVICE* pD3DDev = pDevice->GetRawInterface();

		if (old_val != new_val) {
			pD3DDev->SetRenderState(nRSType, new_val);
			old_val = new_val;
		}
	}
}	// namespace

// コンストラクタ
CRenderState::CRenderState()
{
	m_bIsSavedRS = IZ_FALSE;
}

// デストラクタ
CRenderState::~CRenderState()
{
}

// レンダーステートをセット
void CRenderState::SetRenderState(
	CGraphicsDevice* pDevice,
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

	(this->*func[nState])(pDevice, val);
}

// 現在のレンダーステートを保存
IZ_BOOL CRenderState::Save()
{
	// 保持中でないこと
	IZ_ASSERT(!m_bIsSavedRS);

	IZ_BOOL ret = IZ_FALSE;

	if (!m_bIsSavedRS) {
		memcpy(&m_SaveRS.dwRS, this->dwRS, sizeof(m_SaveRS.dwRS));

		m_bIsSavedRS = IZ_TRUE;
		ret = IZ_TRUE;
	}

	return ret;
}

// 保存したレンダーステートを元に戻す
IZ_BOOL CRenderState::Load(CGraphicsDevice* pDevice)
{
	// 保持中であること
	IZ_ASSERT(m_bIsSavedRS);

	IZ_BOOL ret = IZ_FALSE;

	if (m_bIsSavedRS) {
		for (IZ_UINT i = 0; i < E_GRAPH_RS_NUM; ++i) {
			SetRenderState(
				pDevice,
				static_cast<E_GRAPH_RENDER_STATE>(i),
				m_SaveRS.dwRS[i]);
		}

		m_bIsSavedRS = IZ_FALSE;
		ret = IZ_TRUE;
	}

	return ret;
}

#define _CHECK(f)	{ HRESULT hr = f; IZ_ASSERT(SUCCEEDED(hr)); }

// グラフィックスデバイスから現在設定されている値を取得する
void CRenderState::GetParamsFromGraphicsDevice(CGraphicsDevice* pDevice)
{
	D3D_DEVICE* pD3DDev = pDevice->GetRawInterface();

	// 深度
	_CHECK(pD3DDev->GetRenderState(D3DRS_ZWRITEENABLE, (DWORD*)&isZWriteEnable));
	_CHECK(pD3DDev->GetRenderState(D3DRS_ZENABLE,      (DWORD*)&isZTestEnable));
	_CHECK(pD3DDev->GetRenderState(D3DRS_ZFUNC,        (DWORD*)&cmpZFunc));
	
	// アルファテスト
	_CHECK(pD3DDev->GetRenderState(D3DRS_ALPHATESTENABLE,  (DWORD*)&isAlphaTestEnable));
	_CHECK(pD3DDev->GetRenderState(D3DRS_ALPHAREF,         (DWORD*)&alphaRef));
	_CHECK(pD3DDev->GetRenderState(D3DRS_ALPHAFUNC,        (DWORD*)&cmpAlphaFunc));

	// アルファブレンド
	_CHECK(pD3DDev->GetRenderState(D3DRS_ALPHABLENDENABLE, (DWORD*)&isAlphaBlendEnable));

	// ブレンド方法
	IZ_DWORD nOp, nSrc, nDst;
	_CHECK(pD3DDev->GetRenderState(D3DRS_BLENDOP,   (DWORD*)&nOp));
	_CHECK(pD3DDev->GetRenderState(D3DRS_SRCBLEND,  (DWORD*)&nSrc));
	_CHECK(pD3DDev->GetRenderState(D3DRS_DESTBLEND, (DWORD*)&nDst));
	methodAlphaBlend = IZ_GRAPH_ALPHA_BLEND_VAL(nOp, nSrc, nDst);

	// 描画モード
	_CHECK(pD3DDev->GetRenderState(D3DRS_FILLMODE, (DWORD*)&fillMode));
	_CHECK(pD3DDev->GetRenderState(D3DRS_CULLMODE, (DWORD*)&cullMode));

	// シザー矩形
	_CHECK(pD3DDev->GetRenderState(D3DRS_SCISSORTESTENABLE, (DWORD*)&isScissorEnable));
	_CHECK(pD3DDev->GetScissorRect((RECT*)&rcScissor));

	// カラー描画有効・無効
	IZ_DWORD nColorWriteFlag;
	_CHECK(pD3DDev->GetRenderState(D3DRS_COLORWRITEENABLE, (DWORD*)&nColorWriteFlag));
	isEnableRenderRGB = (nColorWriteFlag & (D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_RED));
	isEnableRenderA = (nColorWriteFlag & D3DCOLORWRITEENABLE_ALPHA);
}

// 深度値描き込み有効・無効
void CRenderState::EnableZWrite(CGraphicsDevice* pDevice, IZ_DWORD flag)
{
	_SetRenderState(
		pDevice,
		D3DRS_ZWRITEENABLE,
		isZWriteEnable, flag);
}

// 深度テスト有効・無効
void CRenderState::EnableZTest(CGraphicsDevice* pDevice, IZ_DWORD flag)
{
	_SetRenderState(
		pDevice,
		D3DRS_ZENABLE,
		isZTestEnable, flag);
}

// 深度テスト判定方法
void CRenderState::SetZTestFunc(CGraphicsDevice* pDevice, IZ_DWORD func)
{
	_SetRenderState(
		pDevice,
		D3DRS_ZFUNC,
		cmpZFunc, func);
}

// アルファテスト有効・無効
void CRenderState::EnableAlphaTest(CGraphicsDevice* pDevice, IZ_DWORD flag)
{
	_SetRenderState(
		pDevice,
		D3DRS_ALPHATESTENABLE,
		isAlphaTestEnable, flag);
}

// アルファテスト基準値
void CRenderState::SetAlphaTestRef(CGraphicsDevice* pDevice, IZ_DWORD ref)
{
	_SetRenderState(
		pDevice,
		D3DRS_ALPHAREF,
		alphaRef, ref);
}

// アルファテスト判定方法
void CRenderState::SetAlphaTestFunc(CGraphicsDevice* pDevice, IZ_DWORD func)
{
	_SetRenderState(
		pDevice,
		D3DRS_ALPHAFUNC,
		cmpAlphaFunc, func);
}

// アルファブレンド有効・無効
void CRenderState::EnableAlphaBlend(CGraphicsDevice* pDevice, IZ_DWORD flag)
{
	_SetRenderState(
		pDevice,
		D3DRS_ALPHABLENDENABLE ,
		isAlphaBlendEnable, flag);
}

// アルファブレンド方法
void CRenderState::SetAlphaBlendMethod(CGraphicsDevice* pDevice, IZ_DWORD method)
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
		pDevice,
		D3DRS_BLENDOP,
		nCurOp, nNewOp);
	_SetRenderState(
		pDevice,
		D3DRS_SRCBLEND,
		nCurSrc, nNewSrc);
	_SetRenderState(
		pDevice,
		D3DRS_DESTBLEND,
		nCurDst, nNewDst);

	// 更新
	methodAlphaBlend = IZ_GRAPH_ALPHA_BLEND_VAL(nNewOp, nNewSrc, nNewDst);
}

// フィルモード
void CRenderState::SetFillMode(CGraphicsDevice* pDevice, IZ_DWORD fill)
{
	_SetRenderState(
		pDevice,
		D3DRS_FILLMODE,
		fillMode, fill);
}

// カリングモード
void CRenderState::SetCullMode(CGraphicsDevice* pDevice, IZ_DWORD cull)
{
	_SetRenderState(
		pDevice,
		D3DRS_CULLMODE,
		cullMode, cull);
}

#define GET_RGB_FLAG(b)	(b ? (D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_RED) : 0)
#define GET_A_FLAG(b)	(b ? D3DCOLORWRITEENABLE_ALPHA : 0)

// レンダーターゲットのカラーバッファの描き込み設定
void CRenderState::EnableRenderColorRGB(CGraphicsDevice* pDevice, IZ_DWORD enableRGB)
{
	// 新しい設定値
	IZ_DWORD nNewFlag = GET_RGB_FLAG(enableRGB);
	nNewFlag |= GET_A_FLAG(isEnableRenderA);

	// 現在の設定値
	IZ_DWORD nCurFlag = GET_RGB_FLAG(isEnableRenderRGB);
	nCurFlag |= GET_A_FLAG(isEnableRenderA);

	_SetRenderState(
		pDevice,
		D3DRS_COLORWRITEENABLE,
		nCurFlag, nNewFlag);

	// 更新
	isEnableRenderRGB = enableRGB;
}

void CRenderState::EnableRenderColorA(CGraphicsDevice* pDevice, IZ_DWORD enableA)
{
	// 新しい設定値
	IZ_DWORD nNewFlag = GET_RGB_FLAG(isEnableRenderRGB);
	nNewFlag |= GET_A_FLAG(enableA);

	// 現在の設定値
	IZ_DWORD nCurFlag = GET_RGB_FLAG(isEnableRenderRGB);
	nCurFlag |= GET_A_FLAG(enableA);

	_SetRenderState(
		pDevice,
		D3DRS_COLORWRITEENABLE,
		nCurFlag, nNewFlag);

	// 更新
	isEnableRenderA = enableA;
}

// シザーテスト
void CRenderState::EnableScissorTest(CGraphicsDevice* pDevice, IZ_DWORD flag)
{
	_SetRenderState(
		pDevice,
		D3DRS_SCISSORTESTENABLE,
		isScissorEnable, flag);
}
