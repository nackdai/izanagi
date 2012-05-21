#if !defined(__IZANAGI_STD_STL_MEMORY_ALLOCATOR_H__)
#define __IZANAGI_STD_STL_MEMORY_ALLOCATOR_H__

#include "izDefs.h"
#include "MemoryAllocator.h"

namespace izanagi {

	/**
	 */
	class STLMemoryAllocatorProxy
	{
	protected:
		static IMemoryAllocator* s_Allocator;

	public:
		static void SetAllocator(IMemoryAllocator* allocator)
		{
			s_Allocator = allocator;
		}
	};

	/** Memory allocator for STL.
	 */
	template <class T>
	class STLMemoryAllocator : STLMemoryAllocatorProxy
	{
	public:
		// 型定義
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef T value_type;
		
		// アロケータをU型にバインドする
		template <class U>
		struct rebind
		{
			typedef STLMemoryAllocator<U> other;
		};
		
		// コンストラクタ
		STLMemoryAllocator() throw()
		{
		}

		STLMemoryAllocator(const STLMemoryAllocator& allocater) throw()
		{
		}

		template <class U>
		STLMemoryAllocator(const STLMemoryAllocator<U>& allocater) throw()
		{
		}

		// デストラクタ
		~STLMemoryAllocator() throw(){}
		
		// メモリを割り当てる
		pointer allocate(
			size_type num,
			//STLMemoryAllocator<void>::const_pointer hint = 0)
			const void* hint = 0)
		{
			IZ_ASSERT(s_Allocator != IZ_NULL);
			void* p = ALLOC(s_Allocator, num * sizeof(T));
			return (pointer)(p);
		}

		// 割当て済みの領域を初期化する
		void construct(pointer p, const T& value)
		{
			new( (void*)p ) T(value);
		}
		
		// メモリを解放する
		void deallocate(pointer p, size_type num)
		{
			FREE(s_Allocator, p);
		}

		// 初期化済みの領域を削除する
		void destroy(pointer p)
		{
			p->~T();
		}
		
		// アドレスを返す
		pointer address(reference value) const
		{
			return &value;
		}

		const_pointer address(const_reference value) const
		{
			return &value;
		}
		
		// 割当てることができる最大の要素数を返す
		size_type max_size() const throw()
		{
			IZ_ASSERT(s_Allocator != IZ_NULL);
			return s_Allocator->GetSize() / sizeof(T);
		}

		template <class T1, class T2>
		static friend bool operator==(const STLMemoryAllocator<T1>&, const STLMemoryAllocator<T2>&) throw()
		{
			return true;
		}

		template <class T1, class T2>
		static friend bool operator!=(const STLMemoryAllocator<T1>&, const STLMemoryAllocator<T2>&) throw()
		{
			return false;
		}
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_STD_STL_MEMORY_ALLOCATOR_H__)
