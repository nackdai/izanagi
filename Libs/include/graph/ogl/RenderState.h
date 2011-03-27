#if !defined(__IZANAGI_GRAPH_RENDER_STATE_H__)
#define __IZANAGI_GRAPH_RENDER_STATE_H__

#include "graph/GraphDefs.h"

namespace izanagi {
	class CGraphicsDevice;
	class CVertexBuffer;
	class CIndexBuffer;
	class CVertexShader;
	class CPixelShader;
	class CVertexDeclaration;
	class CSurface;

	enum {
		TEX_STAGE_NUM = 8,
		MAX_MRT_NUM = 2,
	};

	////////////////////////////////////////////
	/**
	* レンダーステートパラメータ
	*/
	struct S_RENDER_STATE {
		union {
			struct {
				IZ_DWORD isZWriteEnable;		// D3DRS_ZWRITEENABLE
				IZ_DWORD isZTestEnable;			// D3DRS_ZENABLE
				IZ_DWORD cmpZFunc;				// D3DRS_ZFUNC

				IZ_DWORD isAlphaTestEnable;		// D3DRS_ALPHATESTENABLE
				IZ_DWORD alphaRef;				// D3DRS_ALPHAREF
				IZ_DWORD cmpAlphaFunc;			// D3DRS_ALPHAFUNC

				IZ_DWORD isAlphaBlendEnable;	// D3DRS_ALPHABLENDENABLE

				// アルファブレンド処理
				IZ_DWORD methodAlphaBlend;

				IZ_DWORD fillMode;				// D3DRS_FILLMODE
				IZ_DWORD cullMode;				// D3DRS_CULLMODE

				// D3DRS_COLORWRITEENABLE
				IZ_DWORD isEnableRenderRGB;	// RGB
				IZ_DWORD isEnableRenderA;	// A

				IZ_DWORD isScissorEnable;		// D3DRS_SCISSORTESTENABLE
			};
			IZ_DWORD dwRS[E_GRAPH_RS_NUM];
		};

		SViewport vp;			// ビューポート
		
		CIntRect rcScissor;			// シザー矩形

		// TODO
		// ステンシル

		CVertexBuffer* curVB;		// 頂点バッファ
		CIndexBuffer* curIB;		// インデックスバッファ

		CVertexShader* curVS;		// 頂点シェーダ
		CPixelShader* curPS;		// ピクセルシェーダ

		CVertexDeclaration* curVD;	// 頂点宣言

		IZ_DWORD curFVF;			// FVF

		// レンダーターゲット
		CSurface* curRT[MAX_MRT_NUM];
		CSurface* curDepth;
	};

	////////////////////////////////////////////
	/**
	* レンダーステート
	*/
	class CRenderState : public S_RENDER_STATE {
		friend class CGraphicsDevice;

	private:
		CRenderState();
		~CRenderState();

		CRenderState(const CRenderState& rhs);
		const CRenderState& operator=(const CRenderState& rhs);

	private:
		// レンダーステートをセット
		void SetRenderState(
			CGraphicsDevice* pDevice,
			E_GRAPH_RENDER_STATE nState,
			IZ_DWORD val);

		// 現在のレンダーステートを保存
		IZ_BOOL Save();

		// 保存したレンダーステートを元に戻す
		IZ_BOOL Load(CGraphicsDevice* pDevice);

	private:
		// グラフィックスデバイスから現在設定されている値を取得する
		void GetParamsFromGraphicsDevice(CGraphicsDevice* pDevice);

		// 深度
		void EnableZWrite(CGraphicsDevice* pDevice, IZ_DWORD flag);			// 深度値描き込み有効・無効
		void EnableZTest(CGraphicsDevice* pDevice, IZ_DWORD flag);			// 深度テスト有効・無効
		void SetZTestFunc(CGraphicsDevice* pDevice, IZ_DWORD func);			// 深度テスト判定方法

		// アルファ
		void EnableAlphaTest(CGraphicsDevice* pDevice, IZ_DWORD flag);			// アルファテスト有効・無効
		void SetAlphaTestRef(CGraphicsDevice* pDevice, IZ_DWORD ref);			// アルファテスト基準値
		void SetAlphaTestFunc(CGraphicsDevice* pDevice, IZ_DWORD func);			// アルファテスト判定方法
		void EnableAlphaBlend(CGraphicsDevice* pDevice, IZ_DWORD flag);			// アルファブレンド有効・無効
		void SetAlphaBlendMethod(CGraphicsDevice* pDevice, IZ_DWORD method);	// アルファブレンド方法

		// 描画モード
		void SetFillMode(CGraphicsDevice* pDevice, IZ_DWORD fill);			// フィルモード
		void SetCullMode(CGraphicsDevice* pDevice, IZ_DWORD cull);			// カリングモード

		// レンダーターゲットのカラーバッファの描き込み設定
		void EnableRenderColorRGB(CGraphicsDevice* pDevice, IZ_DWORD enableRGB);
		void EnableRenderColorA(CGraphicsDevice* pDevice, IZ_DWORD enableA);

		// シザーテスト
		void EnableScissorTest(CGraphicsDevice* pDevice, IZ_DWORD flag);

	private:
		S_RENDER_STATE m_SaveRS;
		IZ_BOOL m_bIsSavedRS;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_GRAPH_RENDER_STATE_H__)
