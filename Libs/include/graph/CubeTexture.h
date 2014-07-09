#if !defined(__IZANAGI_GRAPH_CUBE_TEXTURE_H__)
#define __IZANAGI_GRAPH_CUBE_TEXTURE_H__

#include "BaseTexture.h"

namespace izanagi
{
namespace graph
{
    class CGraphicsDevice;

    /** Cube texture.
     */
    class CCubeTexture : public CBaseTexture
    {
    protected:
        CCubeTexture() {}
        virtual ~CCubeTexture() {}

        NO_COPIABLE(CCubeTexture);

    public:
        /** Get texture type.
         *
         * @return Return only E_GRAPH_TEX_TYPE_CUBE.
         */
        virtual E_GRAPH_TEX_TYPE GetTexType()
        {
            return E_GRAPH_TEX_TYPE_CUBE;
        }

        /** Lock a range of data and obtains a pointer to the buffer memory.
         *
         * @param [in] nFace Face of cube texture.
         * @param [in] level Mipmap level.
         * @param [out] data VOID* pointer to a memory buffer containing the returned data.
         * @param [in] isReadOnly Read only the data to lock.
         * @param [in] isDiscard 
         * @return If the method succeeds, the return value is true. If the method fails, the return value is false.
         */
        PURE_VIRTUAL(
            IZ_UINT Lock(
                E_GRAPH_CUBE_TEX_FACE nFace,
                IZ_UINT level,
                void** data,
                IZ_BOOL isReadOnly,
                IZ_BOOL isDiscard = IZ_FALSE));

        /** Unlock data.
         *
         * @param [in] nFace Locked face of cube texture.
         * @param [in] level Locked mipmap level.
         * @return If the method succeeds, the return value is true. If the method fails, the return value is false.
         */
        PURE_VIRTUAL(
            IZ_BOOL Unlock(
                E_GRAPH_CUBE_TEX_FACE nFace,
                IZ_UINT level));
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_CUBE_TEXTURE_H__)
