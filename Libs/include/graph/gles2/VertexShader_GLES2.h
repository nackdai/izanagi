#if !defined(__IZANAGI_GRAPH_VERTEX_SHADER_GLES2_H__)
#define __IZANAGI_GRAPH_VERTEX_SHADER_GLES2_H__

#include "graph/VertexShader.h"
#include "graph/dx9/ShaderProxy.h"

namespace izanagi
{
namespace graph
{
    class CGraphicsDeviceGLES2;

    // 頂点シェーダ
    class CVertexShaderGLES2 : public CVertexShader
    {
        friend class CGraphicsDeviceGLES2;

    private:
        // インスタンス作成
        static CVertexShader* CreateVertexShader(
            CGraphicsDeviceGLES2* device,
            IMemoryAllocator* allocator,
            const void* program);

    protected:
        inline CVertexShaderGLES2();
        virtual inline ~CVertexShaderGLES2();

    public:
        D3D_VS* GetRawInterface() { return m_VS; }

    protected:
        // 本体
        D3D_VS* m_VS;
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_VERTEX_SHADER_GLES2_H__)
