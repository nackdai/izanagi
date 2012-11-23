#if !defined(__IZANAGI_GRAPH_BUFFER_BASE_H__)
#define __IZANAGI_GRAPH_BUFFER_BASE_H__

#include "izDefs.h"
#include "izStd.h"

namespace izanagi
{
namespace graph
{
	/**
     */
	class CBufferBase : public CObject
    {
	protected:
		CBufferBase()
        {
            m_Allocator = IZ_NULL;
            m_LockedBuffer = IZ_NULL;
        }

        virtual ~CBufferBase() {};

		NO_COPIABLE(CBufferBase);

	    IZ_DEFINE_INTERNAL_RELEASE();

	public:
		// ロック
        PURE_VIRTUAL(
		    IZ_BOOL Lock(
			    IZ_UINT offset,
			    IZ_UINT size,
			    void** data,
			    IZ_BOOL isReadOnly,
			    IZ_BOOL isDiscard = IZ_FALSE));

		// アンロック
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
		IMemoryAllocator* m_Allocator;

        IZ_UINT8* m_LockedBuffer;
	};
}   // namespace graph
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_GRAPH_BUFFER_BASE_H__)
