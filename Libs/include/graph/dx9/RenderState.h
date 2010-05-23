#if !defined(__URANUS_GRAPH_RENDER_STATE_H__)
#define __URANUS_GRAPH_RENDER_STATE_H__

#include "graph/GraphDefs.h"

namespace uranus {
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
				UN_DWORD isZWriteEnable;		// D3DRS_ZWRITEENABLE
				UN_DWORD isZTestEnable;			// D3DRS_ZENABLE
				UN_DWORD cmpZFunc;				// D3DRS_ZFUNC

				UN_DWORD isAlphaTestEnable;		// D3DRS_ALPHATESTENABLE
				UN_DWORD alphaRef;				// D3DRS_ALPHAREF
				UN_DWORD cmpAlphaFunc;			// D3DRS_ALPHAFUNC

				UN_DWORD isAlphaBlendEnable;	// D3DRS_ALPHABLENDENABLE

				// アルファブレンド処理
				UN_DWORD methodAlphaBlend;

				UN_DWORD fillMode;				// D3DRS_FILLMODE
				UN_DWORD cullMode;				// D3DRS_CULLMODE

				// D3DRS_COLORWRITEENABLE
				UN_DWORD isEnableRenderRGB;	// RGB
				UN_DWORD isEnableRenderA;	// A

				UN_DWORD isScissorEnable;		// D3DRS_SCISSORTESTENABLE
			};
			UN_DWORD dwRS[E_GRAPH_RS_NUM];
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

		UN_DWORD curFVF;			// FVF

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
			UN_DWORD val);

		// 現在のレンダーステートを保存
		UN_BOOL Save();

		// 保存したレンダーステートを元に戻す
		UN_BOOL Load(CGraphicsDevice* pDevice);

	private:
		// グラフィックスデバイスから現在設定されている値を取得する
		void GetParamsFromGraphicsDevice(CGraphicsDevice* pDevice);

		// 深度
		void EnableZWrite(CGraphicsDevice* pDevice, UN_DWORD flag);			// 深度値描き込み有効・無効
		void EnableZTest(CGraphicsDevice* pDevice, UN_DWORD flag);			// 深度テスト有効・無効
		void SetZTestFunc(CGraphicsDevice* pDevice, UN_DWORD func);			// 深度テスト判定方法

		// アルファ
		void EnableAlphaTest(CGraphicsDevice* pDevice, UN_DWORD flag);			// アルファテスト有効・無効
		void SetAlphaTestRef(CGraphicsDevice* pDevice, UN_DWORD ref);			// アルファテスト基準値
		void SetAlphaTestFunc(CGraphicsDevice* pDevice, UN_DWORD func);			// アルファテスト判定方法
		void EnableAlphaBlend(CGraphicsDevice* pDevice, UN_DWORD flag);			// アルファブレンド有効・無効
		void SetAlphaBlendMethod(CGraphicsDevice* pDevice, UN_DWORD method);	// アルファブレンド方法

		// 描画モード
		void SetFillMode(CGraphicsDevice* pDevice, UN_DWORD fill);			// フィルモード
		void SetCullMode(CGraphicsDevice* pDevice, UN_DWORD cull);			// カリングモード

		// レンダーターゲットのカラーバッファの描き込み設定
		void EnableRenderColorRGB(CGraphicsDevice* pDevice, UN_DWORD enableRGB);
		void EnableRenderColorA(CGraphicsDevice* pDevice, UN_DWORD enableA);

		// シザーテスト
		void EnableScissorTest(CGraphicsDevice* pDevice, UN_DWORD flag);

	private:
		S_RENDER_STATE m_SaveRS;
		UN_BOOL m_bIsSavedRS;
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_GRAPH_RENDER_STATE_H__)
