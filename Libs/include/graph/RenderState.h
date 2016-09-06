#if !defined(__IZANAGI_GRAPH_RENDER_STATE_H__)
#define __IZANAGI_GRAPH_RENDER_STATE_H__

#include "izDefs.h"
#include "graph/GraphDefs.h"

namespace izanagi
{
namespace graph
{
    class CGraphicsDevice;
    class CVertexBuffer;
    class CIndexBuffer;
    class CShaderProgram;
    class CVertexDeclaration;
    class CRenderTarget;

    /** レンダーステートパラメータ
     */
    struct S_RENDER_STATE
    {
        union
        {
            struct
            {
                IZ_DWORD isZWriteEnable;
                IZ_DWORD isZTestEnable;
                IZ_DWORD cmpZFunc;

                IZ_DWORD isAlphaTestEnable;
                IZ_DWORD alphaRef;
                IZ_DWORD cmpAlphaFunc;

                IZ_DWORD isAlphaBlendEnable;

                // アルファブレンド処理
                IZ_DWORD methodAlphaBlend;

                IZ_DWORD fillMode;
                IZ_DWORD cullMode;

                IZ_DWORD isEnableRenderRGB; // RGB
                IZ_DWORD isEnableRenderA;   // A

                IZ_DWORD isScissorEnable;

                IZ_DWORD isStencilEnable;
            };
            IZ_DWORD dwRS[E_GRAPH_RS_NUM];
        };

        SViewport vp;               // ビューポート
        
        CIntRect rcScissor;         // シザー矩形

        // ステンシル
        struct {
            E_GRAPH_CMP_FUNC func;
            IZ_INT ref;
            IZ_DWORD mask;
            struct {
                E_GRAPH_STENCIL_OP pass;
                E_GRAPH_STENCIL_OP zfail;
                E_GRAPH_STENCIL_OP fail;
            } op[2];
        } stencilParams;

        CVertexBuffer* curVB[MAX_STREAM_NUM];       // 頂点バッファ
        CIndexBuffer* curIB;        // インデックスバッファ

        CShaderProgram* curShader;

        CVertexDeclaration* curVD;  // 頂点宣言

        // レンダーターゲット
        CRenderTarget* curRT[MAX_MRT_NUM];
        CRenderTarget* curDepth;
    };

    /** レンダーステート
     */
    class CRenderState : public S_RENDER_STATE
    {
        friend class CGraphicsDevice;

    protected:
        CRenderState();
        virtual ~CRenderState();

        NO_COPIABLE(CRenderState);

    public:
        // レンダーステートをセット.
        void SetRenderState(
            CGraphicsDevice* device,
            E_GRAPH_RENDER_STATE nState,
            IZ_DWORD val);

        // 現在のレンダーステートを保存.
        IZ_BOOL Save();

        // 保存したレンダーステートを元に戻す.
        IZ_BOOL Load(CGraphicsDevice* device);

        // グラフィックスデバイスから現在設定されている値を取得する.
        void GetParamsFromGraphicsDevice(CGraphicsDevice* device);

    private:
        // ビューポート.
        IZ_BOOL SetViewport(CGraphicsDevice* device, const SViewport& vp);

        // 深度.
        void EnableZWrite(CGraphicsDevice* device, IZ_DWORD flag);          // 深度値描き込み有効・無効.
        void EnableZTest(CGraphicsDevice* device, IZ_DWORD flag);           // 深度テスト有効・無効.
        void SetZTestFunc(CGraphicsDevice* device, IZ_DWORD func);          // 深度テスト判定方法.

        // アルファ.
        void EnableAlphaTest(CGraphicsDevice* device, IZ_DWORD flag);       // アルファテスト有効・無効.
        void SetAlphaTestRef(CGraphicsDevice* device, IZ_DWORD ref);        // アルファテスト基準値.
        void SetAlphaTestFunc(CGraphicsDevice* device, IZ_DWORD func);      // アルファテスト判定方法.
        void EnableAlphaBlend(CGraphicsDevice* device, IZ_DWORD flag);      // アルファブレンド有効・無効.
        void SetAlphaBlendMethod(CGraphicsDevice* device, IZ_DWORD method); // アルファブレンド方法.

        // 描画モード.
        void SetFillMode(CGraphicsDevice* device, IZ_DWORD fill);           // フィルモード.
        void SetCullMode(CGraphicsDevice* device, IZ_DWORD cull);           // カリングモード.

        // レンダーターゲットのカラーバッファの描き込み設定.
        void EnableRenderColorRGB(CGraphicsDevice* device, IZ_DWORD enableRGB);
        void EnableRenderColorA(CGraphicsDevice* device, IZ_DWORD enableA);

        // シザーテスト.
        void EnableScissorTest(CGraphicsDevice* device, IZ_DWORD flag);
        void SetScissorRect(CGraphicsDevice* device, const SIntRect& rc);

        // ステンシル.
        void EnableStencilTest(CGraphicsDevice* device, IZ_DWORD flag);
        void SetStencilFunc(
            CGraphicsDevice* device,
            E_GRAPH_CMP_FUNC cmp,
            IZ_INT ref,
            IZ_DWORD mask);
        void SetStencilOp(
            CGraphicsDevice* device,
            IZ_BOOL isFront,
            E_GRAPH_STENCIL_OP pass,
            E_GRAPH_STENCIL_OP zfail,
            E_GRAPH_STENCIL_OP fail);

    protected:
        S_RENDER_STATE m_SaveRS;
        IZ_BOOL m_IsSavedRS;
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_RENDER_STATE_H__)
