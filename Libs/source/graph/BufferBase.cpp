#include "graph/BufferBase.h"

namespace izanagi
{
namespace graph
{
    IZ_BOOL CBufferBase::LockBuffer(
        CGraphicsDevice* device,
        IZ_UINT offset,
        IZ_UINT size,
        IZ_BOOL isReadOnly,
        IZ_BOOL isDiscard/*= IZ_FALSE*/)
    {
        IZ_ASSERT(m_LockedBuffer == IZ_NULL);

        IZ_BOOL ret = Lock(
            device,
            offset,
            size,
            (void**)&m_LockedBuffer,
            isReadOnly,
            isDiscard);

        return ret;
    }

    IZ_BOOL CBufferBase::UnlockBuffer(CGraphicsDevice* device)
    {
        return Unlock(device);
    }

    IZ_BOOL CBufferBase::WriteBuffer(
        CGraphicsDevice* device,
        const void* data, size_t size)
    {
        IZ_ASSERT(m_LockedBuffer != IZ_NULL);
        memcpy(m_LockedBuffer, data, size);
        return IZ_TRUE;
    }

    IZ_BOOL CBufferBase::ReadBuffer(
        CGraphicsDevice* device,
        void** data, size_t size)
    {
        IZ_ASSERT(m_LockedBuffer != IZ_NULL);
        memcpy(*data, m_LockedBuffer, size);
        return IZ_TRUE;
    }
}   // namespace graph
}   // namespace izanagi