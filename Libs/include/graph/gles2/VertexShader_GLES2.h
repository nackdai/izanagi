#if !defined(__IZANAGI_GRAPH_VERTEX_SHADER_GLES2_H__)
#define __IZANAGI_GRAPH_VERTEX_SHADER_GLES2_H__

#include "izGLES2Defs.h"
#include "graph/VertexShader.h"
#include "ShaderProxy_GLES2.h"

namespace izanagi
{
namespace graph
{
    class CGraphicsDevice;

    // 頂点シェーダ
    class CVertexShaderGLES2 : public CShaderProxy<CVertexShader>
    {
        friend class CGraphicsDeviceGLES2;

    private:
        // インスタンス作成
        static CVertexShader* CreateVertexShader(
            CGraphicsDevice* device,
            IMemoryAllocator* allocator,
            const void* program);

    protected:
        inline CVertexShaderGLES2();
        virtual inline ~CVertexShaderGLES2();
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_VERTEX_SHADER_GLES2_H__)
