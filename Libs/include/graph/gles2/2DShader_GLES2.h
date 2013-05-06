#if !defined(__IZANAGI_GRAPH_2D_SHADER_GLES2_H__)
#define __IZANAGI_GRAPH_2D_SHADER_GLES2_H__

#include "graph/2d/2DShader.h"

namespace izanagi
{
namespace graph
{
    class CVertexShader;
    class CPixelShader;
    class CGraphicsDevice;
    class CShaderProgram;

    // 2D描画用シェーダ
    class C2DShaderGLES2 : public C2DShader {
        friend class C2DShader;

    private:
        C2DShaderGLES2();
        virtual ~C2DShaderGLES2();

    private:
        // シェーダ作成
        virtual IZ_BOOL CreateShader(CGraphicsDevice* device);

        // シェーダパラメータセット
        virtual IZ_BOOL SetShaderParams(CGraphicsDevice* device);

    private:
        // シェーダパラメータハンドル
        SHADER_PARAM_HANDLE m_hVtxParam[E_GRAPH_2D_RENDER_OP_NUM][VTX_PARAM_NUM];
    };    
}   // namespace graph
}   // namespace izanagi

#endif  // #if !__IZANAGI_GRAPH_2D_SHADER_GLES2_H__(__IZANAGI_GRAPH_2D_SHADER_DX9_H__)
