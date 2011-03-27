#if !defined(__IZANAGI_GRAPH_2D_SHADER_H__)
#define __IZANAGI_GRAPH_2D_SHADER_H__

#include "std/StdObject.h"
#include "graph/GraphDefs.h"

namespace izanagi {
	class CVertexShader;
	class CPixelShader;
	class CGraphicsDevice;

	// 2D描画用シェーダ
	class C2DShader : public CObject {
		friend class C2DRenderer;

	private:
		// インスタンス作成
		static C2DShader* Create2DShader(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice);

	private:
		C2DShader();
		~C2DShader();

		C2DShader(const C2DShader& rhs);
		const C2DShader& operator=(const C2DShader& rhs);

	private:
		// 開放
		inline void InternalRelease();

		// シェーダ作成
		IZ_BOOL CreateShader(CGraphicsDevice* pDevice);

		// シェーダセット
		IZ_BOOL SetShader(CGraphicsDevice* pDevice);

		// シェーダパラメータセット
		IZ_BOOL SetShaderParams(CGraphicsDevice* pDevice);

	private:
		// 描画設定をセット
		inline void SetRenderOp(E_GRAPH_2D_RENDER_OP nOp);

		// 描画設定を取得
		inline E_GRAPH_2D_RENDER_OP GetRenderOp() const;

	private:
		// シェーダパラメータ
		enum {
			VTX_PARAM_INV_SCREEN,		// スクリーンサイズの逆数

			VTX_PARAM_NUM,
		};

	private:
		IMemoryAllocator* m_pAllocator;

		CVertexShader* m_pVS;
		CPixelShader* m_pPS[E_GRAPH_2D_RENDER_OP_NUM];

		// 描画設定
		E_GRAPH_2D_RENDER_OP m_nOp;

		// シェーダパラメータハンドル
		SHADER_PARAM_HANDLE m_hVtxParam[VTX_PARAM_NUM];
	};

	// 開放
	void C2DShader::InternalRelease()
	{
		delete this;
		if (m_pAllocator != IZ_NULL) {
			m_pAllocator->Free(this);
		}
	}

	// 描画設定をセット
	void C2DShader::SetRenderOp(E_GRAPH_2D_RENDER_OP nOp)
	{
		m_nOp = nOp;
	}

	// 描画設定を取得
	E_GRAPH_2D_RENDER_OP C2DShader::GetRenderOp() const
	{
		return m_nOp;
	}

}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_GRAPH_2D_SHADER_H__)
