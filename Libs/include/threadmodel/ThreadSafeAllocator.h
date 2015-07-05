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
        }

        virtual ~CTheadSafeAllocator()
        {
            Dump();
        }

    public:
        // TODO
        void Init(void* buf, IZ_UINT size)
        {
            m_Allocator.Init(size, buf);
        }

        /** �������m��.
         */
        void* Alloc(size_t size, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = 0)
        {
			std::unique_lock<std::mutex> lock(m_Mutex);
            return m_Allocator.Alloc(size, file, line);
        }

        /** ���������m�ۂ��āA�[���Ŗ��߂�.
         */
        void* AllocZero(size_t size, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = 0)
        {
			std::unique_lock<std::mutex> lock(m_Mutex);
            return m_Allocator.AllocZero(size, file, line);
        }

        /** �������Ċm��.
         */
        void* Realloc(void* ptr, size_t size, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = 0)
        {
			std::unique_lock<std::mutex> lock(m_Mutex);
            return m_Allocator.Realloc(ptr, size, file, line);
        }

        /** ���������.
         */
        IZ_BOOL Free(void* data)
        {
			std::unique_lock<std::mutex> lock(m_Mutex);
            return m_Allocator.Free(data);
        }

        /** �m�ۉ\�ȑ��������ʂ��擾.
         */
        IZ_UINT GetSize()
        {
			std::unique_lock<std::mutex> lock(m_Mutex);
            return m_Allocator.GetSize();
        }

        /** �m�ے��������ʂ��擾.
         */
        IZ_UINT GetAllocatedSize()
        {
			std::unique_lock<std::mutex> lock(m_Mutex);
            return m_Allocator.GetAllocatedSize();
        }

        /** �m�ۂ���Ă��Ȃ��������ʂ��擾.
         */
        IZ_UINT GetFreedSize()
        {
			std::unique_lock<std::mutex> lock(m_Mutex);
            return m_Allocator.GetFreedSize();
        }

        /** �������m�ۏ󋵂̃_���v.
         */
        IZ_BOOL Dump()
        {
            std::unique_lock<std::mutex> lock(m_Mutex);
            return m_Allocator.Dump();
        }

        /** �������m�ێ��ɃA�T�[�g�Œ�~����C���f�b�N�X���w�肷��.
         */
        void SetAllocAssert(IZ_UINT64 idx)
        {
			std::unique_lock<std::mutex> lock(m_Mutex);
            m_Allocator.SetAllocAssert(idx);
        }

    private:
        std::mutex m_Mutex;
        _ALLOCATOR m_Allocator;
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREAD_MODEL_THREAD_SAFE_ALLOCATOR_H__)
