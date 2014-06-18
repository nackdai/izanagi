#if !defined(__IZANAGI_GRAPH_BUFFER_BASE_H__)
#define __IZANAGI_GRAPH_BUFFER_BASE_H__

#include "izDefs.h"
#include "izStd.h"
#include "graph/GraphResource.h"

namespace izanagi
{
namespace graph
{
    /** This class is base of buffer classes which have vertex/index data.
     */
    class CBufferBase : public CGraphResource
    {
    protected:
        CBufferBase()
        {
            m_Allocator = IZ_NULL;
            m_LockedBuffer = IZ_NULL;
        }

        virtual ~CBufferBase() {};

    public:
        /** Lock a range of data and obtains a pointer to the buffer memory.
         *
         * @param [in] offset offset in the data to lock, in bytes. To lock entire the data, specify 0 to offset and size.
         * @param [in] size size of the data to lock, in bytes. To lock entire the data, specify 0 to offset and size.
         * @param [out] data VOID* pointer to a memory buffer containing the returned data.
         * @param [in] isReadOnly read only the data to lock.
         * @param [in] isDiscard 
         * @return If the method succeeds, the return value is true. If the method fails, the return value is false.
         */
        PURE_VIRTUAL(
            IZ_BOOL Lock(
                IZ_UINT offset,
                IZ_UINT size,
                void** data,
                IZ_BOOL isReadOnly,
                IZ_BOOL isDiscard = IZ_FALSE));

        /** Unlock data.
         *
         * @return If the method succeeds, the return value is true. If the method fails, the return value is false.
         */
        PURE_VIRTUAL(IZ_BOOL Unlock());

    public:
        IZ_BOOL LockBuffer(
            IZ_UINT offset,
            IZ_UINT size,
            IZ_BOOL isReadOnly,
            IZ_BOOL isDiscard = IZ_FALSE);

        IZ_BOOL UnlockBuffer();

        IZ_BOOL WriteBuffer(const void* data, size_t size);

        IZ_BOOL ReadBuffer(void** data, size_t size);

    protected:
        IZ_UINT8* m_LockedBuffer;
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_BUFFER_BASE_H__)
