#ifdef __IZ_DX9__
#if !defined(__IZANAGI_GRAPH_INTERNAL_2D_RENDERER_DX9_H__)
#define __IZANAGI_GRAPH_INTERNAL_2D_RENDERER_DX9_H__

#include "izD3DDefs.h"
#include "graph/2d/2DRenderer.h"

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
#endif  // #ifdef __IZ_DX9__
