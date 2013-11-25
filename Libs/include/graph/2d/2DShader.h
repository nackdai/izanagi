#if !defined(__IZANAGI_GRAPH_2D_SHADER_H__)
#define __IZANAGI_GRAPH_2D_SHADER_H__

#include "izDefs.h"
#include "izStd.h"
#include "graph/GraphDefs.h"

namespace izanagi
{
namespace graph
{
    class CVertexShader;
    class CPixelShader;
    class CShaderProgram;
    class CGraphicsDevice;

    // 2D描画用シェーダ
    class C2DShader : public CObject {
        friend class C2DRenderer;

    private:
        // インスタンス作成
        static C2DShader* Create2DShader(
            IMemoryAllocator* allocator,
            CGraphicsDevice* device);

    protected:
        C2DShader();
        virtual ~C2DShader();

        NO_COPIABLE(C2DShader);

        IZ_DEFINE_INTERNAL_RELEASE();

    protected:
        // シェーダ作成
        PURE_VIRTUAL(IZ_BOOL CreateShader(CGraphicsDevice* device));

        // シェーダパラメータセット
        PURE_VIRTUAL(IZ_BOOL SetShaderParams(CGraphicsDevice* device));

        // シェーダセット
        IZ_BOOL SetShader(CGraphicsDevice* device);

    protected:
        // 描画設定をセット
        virtual void SetRenderOp(
            CGraphicsDevice* device,
            E_GRAPH_2D_RENDER_OP nOp)
        {
            m_nOp = nOp;
        }

        // 描画設定を取得
        E_GRAPH_2D_RENDER_OP GetRenderOp() const
        {
            return m_nOp;
        }

        CShaderProgram* GetShaderProgram()
        {
            return m_ShaderProgram[m_nOp];
        }

        // ユーザー定義のシェーダをセット
        void SetUserDefsShader(
            CVertexShader* vs,
            CPixelShader* ps);

    protected:
        // シェーダパラメータ
        enum {
            VTX_PARAM_INV_SCREEN,       // スクリーンサイズの逆数

            VTX_PARAM_NUM,
        };

    protected:
        IMemoryAllocator* m_Allocator;

        CShaderProgram* m_ShaderProgram[E_GRAPH_2D_RENDER_OP_NUM];

        CVertexShader* m_pVS;
        CVertexShader* m_UserDefsVS;

        CPixelShader* m_pPS[E_GRAPH_2D_RENDER_OP_NUM];

        // 描画設定
        E_GRAPH_2D_RENDER_OP m_nOp;
    };    
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_2D_SHADER_H__)
