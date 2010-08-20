#include "graph/ogl/RenderState.h"
#include "graph/ogl/GraphicsDevice.h"
#include "graph/ogl/glext.h"

using namespace uranus;

static PFNGLBLENDEQUATIONSEPARATEPROC _glBlendEquationSeparate = UN_NULL;

// コンストラクタ
CRenderState::CRenderState()
{
	m_bIsSavedRS = UN_FALSE;
}

// デストラクタ
CRenderState::~CRenderState()
{
}

// レンダーステートをセット
void CRenderState::SetRenderState(
	CGraphicsDevice* pDevice,
	E_GRAPH_RENDER_STATE nState,
	UN_DWORD val)
{
	UN_ASSERT(nState < E_GRAPH_RS_NUM);

	typedef void (CRenderState::*FUNC)(CGraphicsDevice*, UN_DWORD);
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
	UN_C_ASSERT(COUNTOF(func) == E_GRAPH_RS_NUM);

	(this->*func[nState])(pDevice, val);
}

// 現在のレンダーステートを保存
UN_BOOL CRenderState::Save()
{
	// 保持中でないこと
	UN_ASSERT(!m_bIsSavedRS);

	UN_BOOL ret = UN_FALSE;

	if (!m_bIsSavedRS) {
		memcpy(&m_SaveRS.dwRS, this->dwRS, sizeof(m_SaveRS.dwRS));

		m_bIsSavedRS = UN_TRUE;
		ret = UN_TRUE;
	}

	return ret;
}

// 保存したレンダーステートを元に戻す
UN_BOOL CRenderState::Load(CGraphicsDevice* pDevice)
{
	// 保持中であること
	UN_ASSERT(m_bIsSavedRS);

	UN_BOOL ret = UN_FALSE;

	if (m_bIsSavedRS) {
		for (UN_UINT i = 0; i < E_GRAPH_RS_NUM; i++) {
			SetRenderState(
				pDevice,
				static_cast<E_GRAPH_RENDER_STATE>(i),
				m_SaveRS.dwRS[i]);
		}

		m_bIsSavedRS = UN_FALSE;
		ret = UN_TRUE;
	}

	return ret;
}

// グラフィックスデバイスから現在設定されている値を取得する
void CRenderState::GetParamsFromGraphicsDevice(CGraphicsDevice* pDevice)
{
	// TODO
}

namespace {
	template <typename _T>
	void _SetRenderStateBoolean(
		GLenum param,
		_T& old_val, _T new_val)
	{
		if (old_val != new_val) {
			if (new_val) {
				glEnable(param);
			}
			else {
				glDisable(param);
			}

			old_val = new_val;
		}
	}
}	// namespace

#define _SET_RENDER_STATE(func, old_val, new_val) \
	if (old_val != new_val) {\
		func(new_val);\
		old_val = new_val;\
	}

// 深度値描き込み有効・無効
void CRenderState::EnableZWrite(CGraphicsDevice* pDevice, UN_DWORD flag)
{
	_SET_RENDER_STATE(glDepthMask, isZWriteEnable, flag);
}

// 深度テスト有効・無効
void CRenderState::EnableZTest(CGraphicsDevice* pDevice, UN_DWORD flag)
{
	_SetRenderStateBoolean(
		GL_DEPTH_TEST,
		isZTestEnable, flag);
}

// 深度テスト判定方法
void CRenderState::SetZTestFunc(CGraphicsDevice* pDevice, UN_DWORD func)
{
	_SET_RENDER_STATE(glDepthFunc, cmpZFunc, func);
}

// アルファテスト有効・無効
void CRenderState::EnableAlphaTest(CGraphicsDevice* pDevice, UN_DWORD flag)
{
	_SetRenderStateBoolean(
		GL_ALPHA_TEST,
		isAlphaTestEnable, flag);
}

// アルファテスト基準値
void CRenderState::SetAlphaTestRef(CGraphicsDevice* pDevice, UN_DWORD ref)
{
	if (alphaRef != ref) {
		alphaRef = ref;
		UN_FLOAT fRef = CLAMP(alphaRef / 255.0f, 0.0f, 1.0f);
		glAlphaFunc(cmpAlphaFunc, fRef);
	}
}

// アルファテスト判定方法
void CRenderState::SetAlphaTestFunc(CGraphicsDevice* pDevice, UN_DWORD func)
{
	if (cmpAlphaFunc != func) {
		UN_FLOAT fRef = CLAMP(alphaRef / 255.0f, 0.0f, 1.0f);
		glAlphaFunc(func, fRef);
		cmpAlphaFunc = func;
	}
}

// アルファブレンド有効・無効
void CRenderState::EnableAlphaBlend(CGraphicsDevice* pDevice, UN_DWORD flag)
{
	_SetRenderStateBoolean(
		GL_BLEND,
		isAlphaBlendEnable, flag);
}

// アルファブレンド方法
void CRenderState::SetAlphaBlendMethod(CGraphicsDevice* pDevice, UN_DWORD method)
{
	// 新しい設定値
	UN_DWORD nNewOp = UN_GRAPH_GET_ALPHA_BLEND_OP(method);
	UN_DWORD nNewSrc = UN_GRAPH_GET_ALPHA_BLEND_SRC(method);
	UN_DWORD nNewDst = UN_GRAPH_GET_ALPHA_BLEND_DST(method);

	// 現在の設定値
	UN_DWORD nCurOp = UN_GRAPH_GET_ALPHA_BLEND_OP(methodAlphaBlend);
	UN_DWORD nCurSrc = UN_GRAPH_GET_ALPHA_BLEND_SRC(methodAlphaBlend);
	UN_DWORD nCurDst = UN_GRAPH_GET_ALPHA_BLEND_DST(methodAlphaBlend);

	if (_glBlendEquationSeparate == UN_NULL) {
		_glBlendEquationSeparate = (PFNGLBLENDEQUATIONSEPARATEPROC)wglGetProcAddress("glBlendEquationSeparate");
	}
	UN_ASSERT(_glBlendEquationSeparate != UN_NULL);

	if (nCurOp != nNewOp) {
		// TODO
		(*_glBlendEquationSeparate)(nNewOp, GL_FUNC_ADD);
		nCurOp = nNewOp;
	}

	if ((nCurSrc != nNewSrc)
		|| (nCurDst != nNewDst))
	{
		glBlendFunc(nNewSrc, nNewDst);
	}

	// 更新
	methodAlphaBlend = UN_GRAPH_ALPHA_BLEND_VAL(nNewOp, nNewSrc, nNewDst);
}

// フィルモード
void CRenderState::SetFillMode(CGraphicsDevice* pDevice, UN_DWORD fill)
{
	if (fillMode != fill) {
		glPolygonMode(
			GL_FRONT_AND_BACK,	// TODO
			fill);
		fillMode = fill;
	}
}

// カリングモード
void CRenderState::SetCullMode(CGraphicsDevice* pDevice, UN_DWORD cull)
{
	_SET_RENDER_STATE(
		glCullFace,
		cullMode, cull);
}

#define GET_RGB_FLAG(b)	(b ? (D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_RED) : 0)
#define GET_A_FLAG(b)	(b ? D3DCOLORWRITEENABLE_ALPHA : 0)

// レンダーターゲットのカラーバッファの描き込み設定
void CRenderState::EnableRenderColorRGB(CGraphicsDevice* pDevice, UN_DWORD enableRGB)
{
	// 新しい設定値
	UN_DWORD nNewFlag = GET_RGB_FLAG(enableRGB);
	nNewFlag |= GET_A_FLAG(isEnableRenderA);

	// 現在の設定値
	UN_DWORD nCurFlag = GET_RGB_FLAG(isEnableRenderRGB);
	nCurFlag |= GET_A_FLAG(isEnableRenderA);

	// TODO

	// 更新
	isEnableRenderRGB = enableRGB;
}

void CRenderState::EnableRenderColorA(CGraphicsDevice* pDevice, UN_DWORD enableA)
{
	// 新しい設定値
	UN_DWORD nNewFlag = GET_RGB_FLAG(isEnableRenderRGB);
	nNewFlag |= GET_A_FLAG(enableA);

	// 現在の設定値
	UN_DWORD nCurFlag = GET_RGB_FLAG(isEnableRenderRGB);
	nCurFlag |= GET_A_FLAG(enableA);

	// TODO

	// 更新
	isEnableRenderA = enableA;
}

// シザーテスト
void CRenderState::EnableScissorTest(CGraphicsDevice* pDevice, UN_DWORD flag)
{
	_SetRenderStateBoolean(
		GL_SCISSOR_TEST,
		isScissorEnable, flag);
}
