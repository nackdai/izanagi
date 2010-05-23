#if !defined(__URANUS_GRAPH_GRAPHICS_DEVICE_UTIL_H__)
#define __URANUS_GRAPH_GRAPHICS_DEVICE_UTIL_H__

#include "GraphicsDeviceProxy.h"

namespace uranus {
	/**
	*/
	class CGraphicsDeviceUtil {
	private:
		CGraphicsDeviceUtil();
		~CGraphicsDeviceUtil();

		CGraphicsDeviceUtil(const CGraphicsDeviceUtil& rhs);
		const CGraphicsDeviceUtil& operator=(const CGraphicsDeviceUtil& rhs);

	public:
		// レンダーステート

		// 深度
		static inline void EnableZWrite(CGraphicsDevice* pDevice, UN_DWORD flag);			// 深度値描き込み有効・無効
		static inline void EnableZTest(CGraphicsDevice* pDevice, UN_DWORD flag);			// 深度テスト有効・無効
		static inline void SetZTestFunc(CGraphicsDevice* pDevice, UN_DWORD func);			// 深度テスト判定方法

		// アルファ
		static inline void EnableAlphaTest(CGraphicsDevice* pDevice, UN_DWORD flag);			// アルファテスト有効・無効
		static inline void SetAlphaTestRef(CGraphicsDevice* pDevice, UN_DWORD ref);			// アルファテスト基準値
		static inline void SetAlphaTestFunc(CGraphicsDevice* pDevice, UN_DWORD func);			// アルファテスト判定方法
		static inline void EnableAlphaBlend(CGraphicsDevice* pDevice, UN_DWORD flag);			// アルファブレンド有効・無効
		static inline void SetAlphaBlendMethod(CGraphicsDevice* pDevice, UN_DWORD method);	// アルファブレンド方法

		// 描画モード
		static inline void SetFillMode(CGraphicsDevice* pDevice, UN_DWORD fill);			// フィルモード
		static inline void SetCullMode(CGraphicsDevice* pDevice, UN_DWORD cull);			// カリングモード

		// レンダーターゲットのカラーバッファの描き込み設定
		static inline void EnableRenderColorRGB(CGraphicsDevice* pDevice, UN_DWORD enableRGB);
		static inline void EnableRenderColorA(CGraphicsDevice* pDevice, UN_DWORD enableA);

		// シザーテスト
		static inline void EnableScissorTest(CGraphicsDevice* pDevice, UN_DWORD flag);
	};

	/**
	* 深度値描き込み有効・無効
	*/
	void CGraphicsDeviceUtil::EnableZWrite(CGraphicsDevice* pDevice, UN_DWORD flag)
	{
		pDevice->SetRenderState(E_GRAPH_RS_ZWRITEENABLE, flag);
	}

	/**
	* 深度テスト有効・無効
	*/
	void CGraphicsDeviceUtil::EnableZTest(CGraphicsDevice* pDevice, UN_DWORD flag)
	{
		pDevice->SetRenderState(E_GRAPH_RS_ZENABLE, flag);
	}

	/**
	* 深度テスト判定方法
	*/
	void CGraphicsDeviceUtil::SetZTestFunc(CGraphicsDevice* pDevice, UN_DWORD func)
	{
		pDevice->SetRenderState(E_GRAPH_RS_ZFUNC, func);
	}
	
	/**
	* アルファテスト有効・無効
	*/
	void CGraphicsDeviceUtil::EnableAlphaTest(CGraphicsDevice* pDevice, UN_DWORD flag)
	{
		pDevice->SetRenderState(E_GRAPH_RS_ALPHATESTENABLE, flag);
	}

	/**
	* アルファテスト基準値
	*/
	void CGraphicsDeviceUtil::SetAlphaTestRef(CGraphicsDevice* pDevice, UN_DWORD ref)
	{
		pDevice->SetRenderState(E_GRAPH_RS_ALPHAREF, ref);
	}

	/**
	* アルファテスト判定方法
	*/
	void CGraphicsDeviceUtil::SetAlphaTestFunc(CGraphicsDevice* pDevice, UN_DWORD func)
	{
		pDevice->SetRenderState(E_GRAPH_RS_ALPHAFUNC, func);
	}

	/**
	* アルファブレンド有効・無効
	*/
	void CGraphicsDeviceUtil::EnableAlphaBlend(CGraphicsDevice* pDevice, UN_DWORD flag)
	{
		pDevice->SetRenderState(E_GRAPH_RS_ALPHABLENDENABLE, flag);
	}

	/**
	* アルファブレンド方法
	*/
	void CGraphicsDeviceUtil::SetAlphaBlendMethod(CGraphicsDevice* pDevice, UN_DWORD method)
	{
		pDevice->SetRenderState(E_GRAPH_RS_BLENDMETHOD, method);
	}

	/**
	* フィルモード
	*/
	void CGraphicsDeviceUtil::SetFillMode(CGraphicsDevice* pDevice, UN_DWORD fill)
	{
		pDevice->SetRenderState(E_GRAPH_RS_FILLMODE, fill);
	}

	/**
	* カリングモード
	*/
	void CGraphicsDeviceUtil::SetCullMode(CGraphicsDevice* pDevice, UN_DWORD cull)
	{
		pDevice->SetRenderState(E_GRAPH_RS_CULLMODE, cull);
	}

	/**
	* レンダーターゲットのカラーバッファの描き込み設定
	*/
	void CGraphicsDeviceUtil::EnableRenderColorRGB(CGraphicsDevice* pDevice, UN_DWORD enableRGB)
	{
		pDevice->SetRenderState(E_GRAPH_RS_COLORWRITEENABLE_RGB, enableRGB);
	}

	/**
	* レンダーターゲットのカラーバッファの描き込み設定
	*/
	void CGraphicsDeviceUtil::EnableRenderColorA(CGraphicsDevice* pDevice, UN_DWORD enableA)
	{
		pDevice->SetRenderState(E_GRAPH_RS_COLORWRITEENABLE_A, enableA);
	}

	/**
	* シザーテスト
	*/
	void CGraphicsDeviceUtil::EnableScissorTest(CGraphicsDevice* pDevice, UN_DWORD flag)
	{
		pDevice->SetRenderState(E_GRAPH_RS_SCISSORTESTENABLE, flag);
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_GRAPH_GRAPHICS_DEVICE_UTIL_H__)
