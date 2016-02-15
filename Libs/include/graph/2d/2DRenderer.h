#if !defined(__IZANAGI_GRAPH_INTERNAL_2D_RENDERER_H__)
#define __IZANAGI_GRAPH_INTERNAL_2D_RENDERER_H__

#include "izDefs.h"
#include "izStd.h"
#include "graph/GraphDefs.h"
#include "graph/2d/2DShader.h"

//#define __IZ_TRISTRIP_RECT__

namespace izanagi
{
namespace graph
{
    class CGraphicsDevice;
    class CVertexBuffer;
    class CIndexBuffer;
    class CVertexDeclaration;

    /**
    */
    class C2DRenderer : public CObject
    {
        friend class CGraphicsDevice;

    public:
        // インスタンス作成
        static C2DRenderer* Create2DRenderer(
            CGraphicsDevice* device,
            IMemoryAllocator* allocator);

    protected:
        C2DRenderer();
        ~C2DRenderer();

        NO_COPIABLE(C2DRenderer);

        IZ_DEFINE_INTERNAL_RELEASE();

    protected:
        struct CUSTOM_VERTEX {
            float x, y, z, w;
            IZ_COLOR color;
            float u, v;
        };

        enum PRIM_TYPE {
            PRIM_TYPE_SPRITE = 0,
            PRIM_TYPE_RECT,
            PRIM_TYPE_LINE,

            PRIM_TYPE_NUM,
            PRIM_TYPE_FORCE_INT32 = 0x7fffffff,
        };

    protected:
        // 初期化
        IZ_BOOL Init(CGraphicsDevice* device);

    public:
        // フレーム開始時に呼ぶこと
        void BeginFrame();

        // 描画開始
        IZ_BOOL BeginDraw();

        // 描画終了
        IZ_BOOL EndDraw(CGraphicsDevice* device);

        // 描画コマンドをフラッシュ
        IZ_BOOL Flush(CGraphicsDevice* device);

        // スプライト描画
        IZ_BOOL DrawSprite(
            CGraphicsDevice* device,
            const CFloatRect& rcSrc,
            const CIntRect& rcDst,
            const IZ_COLOR color = IZ_COLOR_RGBA(255, 255, 255, 255));
        IZ_BOOL DrawSpriteEx(
            CGraphicsDevice* device,
            const CIntRect& rcSrc,
            const CIntRect& rcDst,
            const IZ_COLOR color = IZ_COLOR_RGBA(255, 255, 255, 255));

        // 矩形描画
        IZ_BOOL DrawRect(
            CGraphicsDevice* device,
            const CIntRect& rcDst,
            const IZ_COLOR color);

        // 線描画
        IZ_BOOL DrawLine(
            CGraphicsDevice* device,
            const CIntPoint& ptStart,
            const CIntPoint& ptGoal,
            const IZ_COLOR color);

        // 描画設定
        IZ_BOOL SetRenderOp(
            CGraphicsDevice* device,
            E_GRAPH_2D_RENDER_OP nOp);

        // ユーザー定義のシェーダをセット
        void SetUserDefsShader(
            CVertexShader* vs,
            CPixelShader* ps);

        // 描画モードがユーザー定義の時のみ2D用のシェーダープログラムを取得
        CShaderProgram* Get2DShaderProgramIfRenderOpIsUserDefs();

    public:
        // 描画準備
        IZ_BOOL PrepareDraw(
            CGraphicsDevice* device,
            PRIM_TYPE nPrimType);

        // ロック
        IZ_BOOL Lock(CGraphicsDevice* device);

        // アンロック
        void Unlock(CGraphicsDevice* device);

        // 頂点データセット
        void SetVtx(
            const CFloatRect& rcSrc,
            const CIntRect& rcDst,
            const IZ_COLOR color);

        // インデックスデータセット
        void SetIdx();

        // リソースリセット
        void DisableResource();

        // リセット
        void RestoreResource();

    protected:
        // ロックフラグ制御
        IZ_BOOL IsLock() const { return m_bIsLock; }
        void ToggleIsLock() { m_bIsLock = !m_bIsLock; }

        // 描画設定取得
        E_GRAPH_2D_RENDER_OP GetRenderOp() const
        {
            IZ_ASSERT(m_pShader != IZ_NULL);
            return m_pShader->GetRenderOp();
        }

    protected:
        enum {
            MAX_RECT_NUM = 1000,

            // NOTE
            // x4 は１矩形 = ４頂点 なので
            MAX_VERTEX_NUM = MAX_RECT_NUM * 4,

#ifdef __IZ_TRISTRIP_RECT__
            // NOTE
            // １矩形 = ４頂点
            // 矩形と矩形の間をつなぐ見えない三角形用のインデックス数が２
            // ex) ２矩形 -> 0123 34 4567 -> 4 * 2 + 2
            //               012-123-[233-334-344-445]-456-567
            MAX_INDEX_NUM = MAX_RECT_NUM * 4 + (MAX_RECT_NUM - 1) * 2,
#else
            // NOTE
            // １矩形 = ２三角形 = ６頂点
            MAX_INDEX_NUM = MAX_RECT_NUM * 6,
#endif

            // １頂点のサイズ
            VTX_STRIDE = sizeof(CUSTOM_VERTEX),

            // FVF
            //VTX_FVF = FVF_CUSTOMVERTEX,
        };

    protected:
        // バッファ情報
        struct SBufferInfo {
            IZ_UINT offset; // オフセット
            IZ_UINT num;    // データ数
            void* buf_ptr;  // ロックしたときのポインタ

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
        IMemoryAllocator* m_Allocator;

        CVertexBuffer* m_pVB;       // 頂点バッファ
        CIndexBuffer* m_pIB;        // インデックスバッファ
        CVertexDeclaration* m_pVD;  // 頂点宣言

        // バッファ状態管理
        SBufferInfo m_sVBInfo;  // 頂点バッファ用
        SBufferInfo m_sIBInfo;  // インデックスバッファ用

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
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_INTERNAL_2D_RENDERER_H__)
