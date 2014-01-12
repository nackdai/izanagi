#if !defined(__IZANAGI_GRAPH_PIXEL_SHADER_GLES2_H__)
#define __IZANAGI_GRAPH_PIXEL_SHADER_GLES2_H__

#include "graph/GraphDefs.h"
#include "graph/PixelShader.h"
#include "ShaderProxy_GLES2.h"

namespace izanagi
{
namespace graph
{
    class CGraphicsDevice;

    /** ピクセルシェーダ
     */
    class CPixelShaderGLES2 : public CShaderProxy<CPixelShader>
    {
        friend class CGraphicsDeviceGLES2;

    private:
        // インスタンス作成
        static CPixelShader* CreatePixelShader(
            CGraphicsDevice* device,
            IMemoryAllocator* allocator,
            const void* program);

    protected:
        inline CPixelShaderGLES2();
        virtual inline ~CPixelShaderGLES2();
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_PIXEL_SHADER_GLES2_H__)
