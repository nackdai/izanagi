#if !defined(__IZANAGI_GRAPH_RENDER_TARGET_H__)
#define __IZANAGI_GRAPH_RENDER_TARGET_H__

#include "izDefs.h"
#include "izStd.h"
#include "graph/Texture.h"
#include "graph/Surface.h"

namespace izanagi
{
namespace graph
{
    /**
     */
    class CRenderTarget : public CTexture
    {
    protected:
        CRenderTarget() {}
        virtual ~CRenderTarget() {}

        NO_COPIABLE(CRenderTarget);
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_RENDER_TARGET_H__)
