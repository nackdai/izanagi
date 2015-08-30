#if !defined(__IZANAGI_STD_STD_ARRAY_H__)
#define __IZANAGI_STD_STD_ARRAY_H__

#include <functional>

#include "izDefs.h"
#include "std/allocator/MemoryAllocator.h"

namespace izanagi {
    /**
    */
    template <typename _T>
    class CArray {
    public:
		CArray()
        {
            m_allocator = nullptr;

			m_num = 0;
			m_data = nullptr;
        }

		~CArray()
		{
			clear();
		}

		NO_COPIABLE(CArray);

    public:
        IZ_BOOL init(
            IMemoryAllocator* allocator,
            IZ_UINT num)
        {
			IZ_ASSERT(allocator != nullptr);
			IZ_ASSERT(num > 0);

			if (isValid()) {
				return IZ_TRUE;
			}

			IZ_UINT size = num * sizeof(_T*);
			size += num * sizeof(_T);

			IZ_CHAR* buf = (IZ_CHAR*)ALLOC(allocator, size);
			VRETURN(buf != nullptr);

			m_data = (_T**)buf;
			buf += num * sizeof(_T*);

			for (IZ_UINT i = 0; i < num; i++) {
				_T* p = new(buf)_T;
				m_data[i] = p;

				buf += sizeof(_T);
			}

			m_allocator = allocator;
			m_num = num;

            return IZ_TRUE;
        }

        void clear()
        {
			for (IZ_UINT i = 0; i < m_num; i++) {
                m_data[i]->~_T();
			}

			FREE(m_allocator, m_data);

			m_data = nullptr;
			m_num = 0;
        }

        IZ_BOOL isValid()
        {
			return (m_data != nullptr);
        }

		_T& at(IZ_UINT idx)
		{
			IZ_ASSERT(idx < m_num);

			_T* ret = m_data[idx];

			return *ret;
		}

		const _T& at(IZ_UINT idx) const
		{
			IZ_ASSERT(idx < m_num);

			_T* ret = m_data[idx];

			return *ret;
		}

		IZ_UINT getNum() const
		{
			return m_num;
		}

		void traverse(std::function<void(_T&)> func)
		{
			for (IZ_UINT i = 0; i < m_num; i++) {
				func(*m_data[i]);
			}
		}

    protected:
        IMemoryAllocator* m_allocator;
        
		IZ_UINT m_num;
		_T** m_data;
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_STD_STD_RECYCLE_ARRAY_H__)
