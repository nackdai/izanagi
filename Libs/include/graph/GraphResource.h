#if !defined(__IZANAGI_GRAPH_GRAPH_RESOURCE_H__)
#define __IZANAGI_GRAPH_GRAPH_RESOURCE_H__

#include "izDefs.h"
#include "izStd.h"

namespace izanagi
{
namespace graph
{
    /**
     */
    class CGraphResource : public CObject
    {
    protected:
        CGraphResource()
        {
            m_Allocator = IZ_NULL;
        }

        virtual ~CGraphResource() {};

        NO_COPIABLE(CGraphResource);

        IZ_DEFINE_INTERNAL_RELEASE();

    public:
        virtual IZ_BOOL IsPrepared() const
        {
            return IZ_TRUE;
        }

        virtual IZ_BOOL Disable()
        {
            return IZ_TRUE;
        }

        virtual IZ_BOOL Restore()
        {
            return IZ_TRUE;
        }

    protected:
        IMemoryAllocator* m_Allocator;
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_GRAPH_RESOURCE_H__)
