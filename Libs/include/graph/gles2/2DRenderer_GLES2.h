#if !defined(__IZANAGI_GRAPH_INTERNAL_2D_RENDERER_GLES2_H__)
#define __IZANAGI_GRAPH_INTERNAL_2D_RENDERER_GLES2_H__

#include "graph/2d/2DRenderer.h"

namespace izanagi
{
namespace graph
{
    class C2DRendererGLES2 : public C2DRenderer
    {
    public:
        // インスタンス作成
        static C2DRenderer* Create2DRenderer(
            CGraphicsDevice* device,
            IMemoryAllocator* allocator)
        {
            return C2DRenderer::Create2DRenderer(device, allocator);
        }
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_INTERNAL_2D_RENDERER_GLES2_H__)
