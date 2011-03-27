#if !defined(__IZANAGI_GRAPH_INTERNAL_2D_RENDERER_H__)
#define __IZANAGI_GRAPH_INTERNAL_2D_RENDERER_H__

#include "izD3DDefs.h"
#include "std/StdObject.h"
#include "std/2DRect.h"
#include "graph/GraphDefs.h"
#include "2DShader.h"

namespace izanagi {
	class IMemoryAllocator;
	class CGraphicsDevice;
	class CVertexBuffer;
	class CIndexBuffer;
	class CVertexDeclaration;

	/**
	*/
	class C2DRenderer : public CObject {
		friend class CGraphicsDevice;

	protected:
		// インスタンス作成
		static C2DRenderer* Create2DRenderer(
			CGraphicsDevice* pDevice,
			IMemoryAllocator* pAllocator);

	protected:
		C2DRenderer();
		~C2DRenderer();

		C2DRenderer(const C2DRenderer& rhs);
		const C2DRenderer& operator=(const C2DRenderer& rhs);

	protected:
		struct CUSTOM_VERTEX {
			float x, y, z, w;
			IZ_COLOR color;
			float u, v;
		};

		//static const DWORD FVF_CUSTOMVERTEX = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

		enum PRIM_TYPE {
			PRIM_TYPE_SPRITE = 0,
			PRIM_TYPE_RECT,
			PRIM_TYPE_LINE,

			PRIM_TYPE_NUM,
			PRIM_TYPE_FORCE_INT32 = 0x7fffffff,
		};

	protected:
		// 解放
		void InternalRelease();

		// 初期化
		IZ_BOOL Init(CGraphicsDevice* pDevice);

	protected:
		// フレーム開始時に呼ぶこと
		void BeginFrame();

		// 描画開始
		IZ_BOOL BeginDraw();

		// 描画終了
		IZ_BOOL EndDraw(CGraphicsDevice* pDevice);

		// 描画コマンドをフラッシュ
		IZ_BOOL Flush(CGraphicsDevice* pDevice);

		// スプライト描画
		IZ_BOOL DrawSprite(
			CGraphicsDevice* pDevice,
			const CFloatRect& rcSrc,
			const CIntRect& rcDst,
			const IZ_COLOR color = IZ_COLOR_RGBA(255, 255, 255, 255));
		IZ_BOOL DrawSpriteEx(
			CGraphicsDevice* pDevice,
			const CIntRect& rcSrc,
			const CIntRect& rcDst,
			const IZ_COLOR color = IZ_COLOR_RGBA(255, 255, 255, 255));

		// 矩形描画
		IZ_BOOL DrawRect(
			CGraphicsDevice* pDevice,
			const CIntRect& rcDst,
			const IZ_COLOR color);

		// 線描画
		IZ_BOOL DrawLine(
			CGraphicsDevice* pDevice,
			const CIntPoint& ptStart,
			const CIntPoint& ptGoal,
			const IZ_COLOR color);

		// 描画設定
		IZ_BOOL SetRenderOp(
			CGraphicsDevice* pDevice,
			E_GRAPH_2D_RENDER_OP nOp);

	protected:
		// 描画準備
		IZ_BOOL PrepareDraw(
			CGraphicsDevice* pDevice,
			PRIM_TYPE nPrimType);

		// ロック
		IZ_BOOL Lock();

		// アンロック
		void Unlock();

		// 頂点データセット
		void SetVtx(
			const CFloatRect& rcSrc,
			const CIntRect& rcDst,
			const IZ_COLOR color);

		// インデックスデータセット
		void SetIdx();

		// リソースリセット
		void ResetResource();

		// リセット
		void Reset();

	protected:
		// ロックフラグ制御
		IZ_BOOL IsLock() const { return m_bIsLock; }
		void ToggleIsLock() { m_bIsLock = !m_bIsLock; }

		// 描画設定取得
		inline E_GRAPH_2D_RENDER_OP GetRenderOp() const;

	protected:
		enum {
			MAX_RECT_NUM = 1000,

			// NOTE
			// x4 は１矩形 = ４頂点 なので
			MAX_VERTEX_NUM = MAX_RECT_NUM * 4,

			// NOTE
			// １矩形 = ４頂点
			// 矩形と矩形の間をつなぐ見えない三角形用のインデックス数が２
			// ex) ２矩形 -> 0123 34 4567 -> 4 * 2 + 2
			//               012-123-[233-334-344-445]-456-567
			MAX_INDEX_NUM = MAX_RECT_NUM * 4 + (MAX_RECT_NUM - 1) * 2,

			// １頂点のサイズ
			VTX_STRIDE = sizeof(CUSTOM_VERTEX),

			// FVF
			//VTX_FVF = FVF_CUSTOMVERTEX,
		};

	protected:
		// バッファ情報
		struct SBufferInfo {
			IZ_UINT offset;	// オフセット
			IZ_UINT num;	// データ数
			void* buf_ptr;	// ロックしたときのポインタ

			// 次のロックはDISCARDかどうか
			IZ_BOOL next_lock_discard;

			SBufferInfo()
			{
				Clear();
				next_lock_discard = IZ_FALSE;
			}

			void Clear()
			{
				offset = 0;
				num = 0;
				buf_ptr = IZ_NULL;
			}
		};

	protected:
		IMemoryAllocator* m_pAllocator;

		CVertexBuffer* m_pVB;		// 頂点バッファ
		CIndexBuffer* m_pIB;		// インデックスバッファ
		CVertexDeclaration* m_pVD;	// 頂点宣言

		// バッファ状態管理
		SBufferInfo m_sVBInfo;	// 頂点バッファ用
		SBufferInfo m_sIBInfo;	// インデックスバッファ用

		// シェーダ
		C2DShader* m_pShader;

		// 描画プリミティブタイプ
		PRIM_TYPE m_nPrimType;

		// 現在設定されているプリミティブ数
		IZ_UINT m_nCurPrimNum;

		// 現在セットされているインデックス
		IZ_UINT16 m_nCurIdx;

		// ロックフラグ
		IZ_BOOL m_bIsLock;
	};

	// inline ********************************

	// 描画設定取得
	E_GRAPH_2D_RENDER_OP C2DRenderer::GetRenderOp() const
	{
		IZ_ASSERT(m_pShader != IZ_NULL);
		return m_pShader->GetRenderOp();
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_GRAPH_INTERNAL_2D_RENDERER_H__)
