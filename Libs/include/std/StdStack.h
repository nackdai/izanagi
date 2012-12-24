#if !defined(__IZANAGI_STD_STACK_H__)
#define __IZANAGI_STD_STACK_H__

#include "izDefs.h"
#include "allocator/MemoryAllocator.h"

namespace izanagi {
    /** スタック（LIFO）.
     */
    template<
        class _T,
        IZ_UINT _NUM
    >
    class CStack {
    public:
        CStack()
        {
            m_nPos = 0;
            m_nNum = 0;
        }

        ~CStack() {}
        
        NO_COPIABLE(CStack);
        
    public:
        // スタックが一杯か
        IZ_BOOL IsFull() const
        {
            return (m_nNum == _NUM);
        }

        // スタックが空か
        IZ_BOOL IsEmpty() const
        {
            return (m_nNum == 0);
        }
        
        // アイテムをスタックから取得
        _T& Get()
        {
            IZ_ASSERT(!IsFull());
            return m_Items[m_nPos];
        }
        
        // スタックをプッシュする
        IZ_BOOL Push()
        {
            if (!IsFull()) {
                ++m_nPos;
                ++m_nNum;
                return IZ_TRUE;
            }
            
            return IZ_FALSE;
        }
        
        // スタックをポップする
        IZ_BOOL Pop()
        {
            if (!IsEmpty()) {
                m_nPos--;
                m_nNum--;
                return IZ_TRUE;
            }

            return IZ_FALSE;
        }

        // 最大登録可能アイテム数取得
        IZ_UINT GetMaxNum() const
        {
            return _NUM;
        }

        // 登録アイテム数取得
        IZ_UINT GetNum() const
        {
            return m_nNum;
        }
                    
    private:
        _T m_Items[_NUM];

        IZ_INT m_nPos;
        IZ_INT m_nNum;
    };

    /**
     */
    template <typename _T>
    class CAllocatedStack {
    public:
        CAllocatedStack()
        {
            m_Allocator = IZ_NULL;
            m_nPos = 0;
            m_nNum = 0;
            m_Max = 0;
        }

        ~CAllocatedStack()
        {
            FREE(m_Allocator, m_Items);
        }

        NO_COPIABLE(CAllocatedStack);

    public:
        // 初期化
        IZ_BOOL Init(
            IMemoryAllocator* allocator,
            IZ_UINT maxItems)
        {
            IZ_ASSERT(allocator != IZ_NULL);

            VRETURN(m_Items == IZ_NULL);
            m_Items = (_T*)ALLOC_ZERO(allocator, maxItems);
            VRETURN(m_Items != IZ_NULL);

            m_Allocator = allocator;
            m_Max = maxItems;

            return IZ_TRUE;
        }

        void Reset()
        {
            m_nPos = 0;
            m_nNum = 0;
        }

        // スタックが一杯か
        IZ_BOOL IsFull() const
        {
            return (m_nNum == m_Max);
        }

        // スタックが空か
        IZ_BOOL IsEmpty() const
        {
            return (m_nNum == 0);
        }
        
        // アイテムをスタックから取得
        _T& Get()
        {
            IZ_ASSERT(!IsFull());
            return m_Items[m_nPos];
        }
        
        // スタックをプッシュする
        IZ_BOOL Push()
        {
            if (!IsFull()) {
                ++m_nPos;
                ++m_nNum;
                return IZ_TRUE;
            }
            
            return IZ_FALSE;
        }
        
        // スタックをポップする
        IZ_BOOL Pop()
        {
            if (!IsEmpty()) {
                m_nPos--;
                m_nNum--;
                return IZ_TRUE;
            }

            return IZ_FALSE;
        }

        // 最大登録可能アイテム数取得
        IZ_UINT GetMaxNum() const
        {
            return m_Max;
        }

        // 登録アイテム数取得
        IZ_UINT GetNum() const
        {
            return m_nNum;
        }

    private:
        IMemoryAllocator* m_Allocator;

        _T* m_Items;

        IZ_INT m_nPos;
        IZ_INT m_nNum;
        IZ_UINT m_Max;
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_STD_STACK_H__)
