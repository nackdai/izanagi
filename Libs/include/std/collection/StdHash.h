#if !defined(__IZANAGI_STD_HASH_H__)
#define __IZANAGI_STD_HASH_H__

#include "StdHashBase.h"

namespace izanagi {
    /**
    */
    template <typename _KEY, typename _T, IZ_UINT _NUM>
    class CStdHash : public CStdHashBase<_KEY, _T, CStdHash<_KEY, _T, _NUM> > {
    public:
        // コンストラクタ
        CStdHash()
        {
            m_GetHashListFunc = &CStdHash<_KEY, _T, _NUM>::GetHashListInternal;
        }

        // デストラクタ
        ~CStdHash()
        {
            Clear();
        }

        // ハッシュサイズを取得
        IZ_UINT GetHashSize() const
        {
            return _NUM;
        }
        
        // クリア
        void Clear()
        {
            // ハッシュリストクリア
            for (IZ_UINT i = 0; i < _NUM; ++i) {
                m_HashList[i].Clear();
            }

            m_OrderList.Clear();
        }

    protected:
        CStdList<Item>* GetHashListInternal(const _KEY& tKey)
        {
            IZ_UINT64 idx = tKey % _NUM;
            return &m_HashList[idx];
        }

    private:
        // ハッシュリスト
        CStdList<Item> m_HashList[_NUM];

        IZ_C_ASSERT(_NUM > 0);
    };

    /**
    */
    template <typename _KEY, typename _T>
    class CStdHashEx : public CStdHashBase<_KEY, _T, CStdHashEx<_KEY, _T> > {
    public:
        // コンストラクタ
        CStdHashEx()
        {
            m_Allocator != NULL;

            m_nSize = 0;
            m_pHashList = NULL;

            m_GetHashListFunc = GetHashListInternal;
        }

        // デストラクタ
        ~CStdHashEx()
        {
            Clear();
            FREE(m_Allocator, m_pHashList);
        }

        // 初期化
        IZ_BOOL Init(IMemoryAllocator* pAllocator, IZ_UINT nHashSize)
        {
            IZ_ASSERT(pAllocator != IZ_NULL);
            IZ_ASSERT(nSize > 0);

            if (m_Allocator != NULL) {
                Clear();
            }

            size_t nSize = ComputeBufferSize(nHashSize);

            // メモリ確保
            IZ_UINT8* pBuf = (IZ_UINT8*)ALLOC_ZERO(pAllocator, nSize);
            VRETURN(pBuf != NULL);

            IZ_BOOL ret = InitEx(pBuf, nHashSize);

            return ret;
        }

        // 初期化
        IZ_BOOL InitEx(void* pBuf, IZ_UINT nHashSize)
        {
            IZ_ASSERT(m_pHashList == IZ_NULL);
            IZ_ASSERT(pBuf != NULL);
            IZ_ASSERT(nSize > 0);

            CStdList<Item>* p = reinterpret_cast<CStdList<Item>*>(pBuf);
            
            CStdList<Item>* pList = m_pHashList;

            // リスト初期化
            for (IZ_UINT i = 0; i < nHashSize; ++i) {
                pList = new(p) CStdList<Item>;
                ++pList;

                ++p;
            }

            m_nSize = nHashSize;

            return IZ_TRUE;
        }

        void Clear()
        {
            // ハッシュリストクリア
            for (IZ_UINT i = 0; i < _NUM; ++i) {
                m_HashList[i].Clear();
            }

            m_OrderList.Clear();
        }

        // ハッシュサイズを取得
        IZ_UINT GetHashSize() const
        {
            return m_nSize;
        }

    protected:
        CStdList<Item>* GetHashListInternal(const _KEY& tKey)
        {
            IZ_ASSERT(m_pHashList != IZ_NULL);
            IZ_UINT idx = tKey % m_nSize;
            return &m_pHashList[idx];
        }

    private:
        IMemoryAllocator* m_Allocator;

        // ハッシュリスト
        IZ_UINT m_nSize;
        CStdList<Item>* m_pHashList;
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_STD_HASH_H__)
