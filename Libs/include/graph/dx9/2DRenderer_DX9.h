#if !defined(__IZANAGI_GRAPH_INTERNAL_2D_RENDERER_DX9_H__)
#define __IZANAGI_GRAPH_INTERNAL_2D_RENDERER_DX9_H__

#include "graph/2d/2DRenderer.h"
#include "graph/dx9/Surface_DX9.h"

namespace izanagi
{
namespace graph
{
    class C2DRendererDX9 : public C2DRenderer
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

#endif  // #if !defined(__IZANAGI_GRAPH_INTERNAL_2D_RENDERER_DX9_H__)
