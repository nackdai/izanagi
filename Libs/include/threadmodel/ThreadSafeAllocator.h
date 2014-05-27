#if !defined(__IZANAGI_THREAD_MODEL_THREAD_SAFE_ALLOCATOR_H__)
#define __IZANAGI_THREAD_MODEL_THREAD_SAFE_ALLOCATOR_H__

#include "izStd.h"
#include "izSystem.h"

namespace izanagi
{
namespace threadmodel
{
    /**
     */
    class IThreadSafeAllocator : public IMemoryAllocator {
    protected:
        IThreadSafeAllocator() {}
        virtual ~IThreadSafeAllocator() {}
    };

    /**
     */
    template <typename _ALLOCATOR>
    class CTheadSafeAllocator : public IThreadSafeAllocator {
    public:
        CTheadSafeAllocator()
        {
            m_Mutex.Open();
        }

        virtual ~CTheadSafeAllocator()
        {
            m_Mutex.Lock();
            Dump();
            m_Mutex.Unlock();

            m_Mutex.Close();
        }

    public:
        // TODO
        void Init(void* buf, IZ_UINT size)
        {
            m_Allocator.Init(buf, size);
        }

        /** メモリ確保.
         */
        void* Alloc(size_t size, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = 0)
        {
            sys::CGuarder guard(m_Mutex);
            return m_Allocator.Alloc(size, file, line);
        }

        /** メモリを確保して、ゼロで埋める.
         */
        void* AllocZero(size_t size, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = 0)
        {
            sys::CGuarder guard(m_Mutex);
            return m_Allocator.AllocZero(size, file, line);
        }

        /** メモリ再確保.
         */
        void* Realloc(void* ptr, size_t size, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = 0)
        {
            sys::CGuarder guard(m_Mutex);
            return m_Allocator.Realloc(ptr, size, file, line);
        }

        /** メモリ解放.
         */
        IZ_BOOL Free(void* data)
        {
            sys::CGuarder guard(m_Mutex);
            return m_Allocator.Free(data);
        }

        /** 確保可能な総メモリ量を取得.
         */
        IZ_UINT GetSize()
        {
            sys::CGuarder guard(m_Mutex);
            return m_Allocator.GetSize();
        }

        /** 確保中メモリ量を取得.
         */
        IZ_UINT GetAllocatedSize()
        {
            sys::CGuarder guard(m_Mutex);
            return m_Allocator.GetAllocatedSize();
        }

        /** 確保されていないメモリ量を取得.
         */
        IZ_UINT GetFreedSize()
        {
            sys::CGuarder guard(m_Mutex);
            return m_Allocator.GetFreedSize();
        }

        /** メモリ確保状況のダンプ.
         */
        IZ_BOOL Dump()
        {
            sys::CGuarder guard(m_Mutex);
            return m_Allocator.Dump();
        }

        /** メモリ確保時にアサートで停止するインデックスを指定する.
         */
        void SetAllocAssert(IZ_UINT64 idx)
        {
            sys::CGuarder guard(m_Mutex);
            m_Allocator.SetAllocAssert(idx);
        }

    private:
        sys::CMutex m_Mutex;
        _ALLOCATOR m_Allocator;
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREAD_MODEL_THREAD_SAFE_ALLOCATOR_H__)
