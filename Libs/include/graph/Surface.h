#if !defined(__IZANAGI_GRAPH_SURFACE_H__)
#define __IZANAGI_GRAPH_SURFACE_H__

#include "izDefs.h"
#include "izStd.h"
#include "graph/GraphDefs.h"

namespace izanagi
{
namespace graph
{
    class CTexture;

    /**
     */
    class CSurface : public CObject
    {
    protected:
        CSurface()
        {
            m_Allocator = IZ_NULL;
            m_Fmt = E_GRAPH_PIXEL_FMT_FORCE_INT32;
        }

        virtual ~CSurface() {};

        NO_COPIABLE(CSurface);

        IZ_DEFINE_INTERNAL_RELEASE();

    public:
        PURE_VIRTUAL(IZ_UINT GetWidth() const);

        PURE_VIRTUAL(IZ_UINT GetHeight() const);

        E_GRAPH_PIXEL_FMT GetPixelFormat() const
        {
            return m_Fmt;
        }

        PURE_VIRTUAL(void* GetPlatformInterface());

    protected:
        PURE_VIRTUAL(
            IZ_BOOL Reset(
                CTexture* texture,
                IZ_UINT level));

    protected:
        IMemoryAllocator* m_Allocator;
        E_GRAPH_PIXEL_FMT m_Fmt;
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_SURFACE_H__)
