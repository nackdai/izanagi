#if !defined(__IZANAGI_GRAPH_RENDER_STATE_SETTER_H__)
#define __IZANAGI_GRAPH_RENDER_STATE_SETTER_H__

#include "GraphicsDeviceProxy.h"

namespace izanagi {
	/**
	*/
	class CRenderStateSetter {
	private:
		CRenderStateSetter();
		~CRenderStateSetter();

		NO_COPIABLE(CRenderStateSetter);

	public:
		// レンダーステート

		// 深度
		static inline void EnableZWrite(CGraphicsDevice* pDevice, IZ_DWORD flag);			// 深度値描き込み有効・無効
		static inline void EnableZTest(CGraphicsDevice* pDevice, IZ_DWORD flag);			// 深度テスト有効・無効
		static inline void SetZTestFunc(CGraphicsDevice* pDevice, IZ_DWORD func);			// 深度テスト判定方法

		// アルファ
		static inline void EnableAlphaTest(CGraphicsDevice* pDevice, IZ_DWORD flag);			// アルファテスト有効・無効
		static inline void SetAlphaTestRef(CGraphicsDevice* pDevice, IZ_DWORD ref);			// アルファテスト基準値
		static inline void SetAlphaTestFunc(CGraphicsDevice* pDevice, IZ_DWORD func);			// アルファテスト判定方法
		static inline void EnableAlphaBlend(CGraphicsDevice* pDevice, IZ_DWORD flag);			// アルファブレンド有効・無効
		static inline void SetAlphaBlendMethod(CGraphicsDevice* pDevice, IZ_DWORD method);	// アルファブレンド方法

		// 描画モード
		static inline void SetFillMode(CGraphicsDevice* pDevice, IZ_DWORD fill);			// フィルモード
		static inline void SetCullMode(CGraphicsDevice* pDevice, IZ_DWORD cull);			// カリングモード

		// レンダーターゲットのカラーバッファの描き込み設定
		static inline void EnableRenderColorRGB(CGraphicsDevice* pDevice, IZ_DWORD enableRGB);
		static inline void EnableRenderColorA(CGraphicsDevice* pDevice, IZ_DWORD enableA);

		// シザーテスト
		static inline void EnableScissorTest(CGraphicsDevice* pDevice, IZ_DWORD flag);
	};

	/**
	* 深度値描き込み有効・無効
	*/
	void CRenderStateSetter::EnableZWrite(CGraphicsDevice* pDevice, IZ_DWORD flag)
	{
		pDevice->SetRenderState(E_GRAPH_RS_ZWRITEENABLE, flag);
	}

	/**
	* 深度テスト有効・無効
	*/
	void CRenderStateSetter::EnableZTest(CGraphicsDevice* pDevice, IZ_DWORD flag)
	{
		pDevice->SetRenderState(E_GRAPH_RS_ZENABLE, flag);
	}

	/**
	* 深度テスト判定方法
	*/
	void CRenderStateSetter::SetZTestFunc(CGraphicsDevice* pDevice, IZ_DWORD func)
	{
		pDevice->SetRenderState(E_GRAPH_RS_ZFUNC, func);
	}
	
	/**
	* アルファテスト有効・無効
	*/
	void CRenderStateSetter::EnableAlphaTest(CGraphicsDevice* pDevice, IZ_DWORD flag)
	{
		pDevice->SetRenderState(E_GRAPH_RS_ALPHATESTENABLE, flag);
	}

	/**
	* アルファテスト基準値
	*/
	void CRenderStateSetter::SetAlphaTestRef(CGraphicsDevice* pDevice, IZ_DWORD ref)
	{
		pDevice->SetRenderState(E_GRAPH_RS_ALPHAREF, ref);
	}

	/**
	* アルファテスト判定方法
	*/
	void CRenderStateSetter::SetAlphaTestFunc(CGraphicsDevice* pDevice, IZ_DWORD func)
	{
		pDevice->SetRenderState(E_GRAPH_RS_ALPHAFUNC, func);
	}

	/**
	* アルファブレンド有効・無効
	*/
	void CRenderStateSetter::EnableAlphaBlend(CGraphicsDevice* pDevice, IZ_DWORD flag)
	{
		pDevice->SetRenderState(E_GRAPH_RS_ALPHABLENDENABLE, flag);
	}

	/**
	* アルファブレンド方法
	*/
	void CRenderStateSetter::SetAlphaBlendMethod(CGraphicsDevice* pDevice, IZ_DWORD method)
	{
		pDevice->SetRenderState(E_GRAPH_RS_BLENDMETHOD, method);
	}

	/**
	* フィルモード
	*/
	void CRenderStateSetter::SetFillMode(CGraphicsDevice* pDevice, IZ_DWORD fill)
	{
		pDevice->SetRenderState(E_GRAPH_RS_FILLMODE, fill);
	}

	/**
	* カリングモード
	*/
	void CRenderStateSetter::SetCullMode(CGraphicsDevice* pDevice, IZ_DWORD cull)
	{
		pDevice->SetRenderState(E_GRAPH_RS_CULLMODE, cull);
	}

	/**
	* レンダーターゲットのカラーバッファの描き込み設定
	*/
	void CRenderStateSetter::EnableRenderColorRGB(CGraphicsDevice* pDevice, IZ_DWORD enableRGB)
	{
		pDevice->SetRenderState(E_GRAPH_RS_COLORWRITEENABLE_RGB, enableRGB);
	}

	/**
	* レンダーターゲットのカラーバッファの描き込み設定
	*/
	void CRenderStateSetter::EnableRenderColorA(CGraphicsDevice* pDevice, IZ_DWORD enableA)
	{
		pDevice->SetRenderState(E_GRAPH_RS_COLORWRITEENABLE_A, enableA);
	}

	/**
	* シザーテスト
	*/
	void CRenderStateSetter::EnableScissorTest(CGraphicsDevice* pDevice, IZ_DWORD flag)
	{
		pDevice->SetRenderState(E_GRAPH_RS_SCISSORTESTENABLE, flag);
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_GRAPH_RENDER_STATE_SETTER_H__)
