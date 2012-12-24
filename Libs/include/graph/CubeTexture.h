#if !defined(__IZANAGI_GRAPH_CUBE_TEXTURE_H__)
#define __IZANAGI_GRAPH_CUBE_TEXTURE_H__

#include "BaseTexture.h"

namespace izanagi
{
namespace graph
{
    class CGraphicsDevice;

    class CCubeTexture : public CBaseTexture
    {
    protected:
        CCubeTexture() {}
        virtual ~CCubeTexture() {}

        NO_COPIABLE(CCubeTexture);

    public:
        // ロック
        PURE_VIRTUAL(
            IZ_UINT Lock(
                E_GRAPH_CUBE_TEX_FACE nFace,
                UINT level,
                void** data,
                IZ_BOOL isReadOnly,
                IZ_BOOL isDiscard = IZ_FALSE));

        // アンロック
        PURE_VIRTUAL(
            IZ_BOOL Unlock(
                E_GRAPH_CUBE_TEX_FACE nFace,
                IZ_UINT level));
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_CUBE_TEXTURE_H__)
