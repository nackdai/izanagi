#if !defined(__IZANAGI_GRAPH_TEXTURE_H__)
#define __IZANAGI_GRAPH_TEXTURE_H__

#include "izDefs.h"
#include "izStd.h"
#include "graph/BaseTexture.h"
#include "graph/Surface.h"

namespace izanagi
{
namespace graph
{
    /**
     */
    class CTexture : public CBaseTexture
    {
    protected:
        CTexture() {}
        virtual ~CTexture() {}

        NO_COPIABLE(CTexture);

    public:
        // ロック
        virtual IZ_UINT Lock(
            UINT level,
            void** data,
            IZ_BOOL isReadOnly,
            IZ_BOOL isDiscard = IZ_FALSE)
        {
            IZ_ASSERT(IZ_FALSE);
            return 0;
        }

        // アンロック
        virtual IZ_BOOL Unlock(IZ_UINT level)
        {
            IZ_ASSERT(IZ_FALSE);
            return IZ_FALSE;
        }

        virtual CSurface* GetSurface()
        {
            return IZ_NULL;
        }
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_TEXTURE_H__)
