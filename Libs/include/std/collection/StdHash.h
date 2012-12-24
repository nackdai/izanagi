#if !defined(__IZANAGI_STD_HASH_H__)
#define __IZANAGI_STD_HASH_H__

#include "izDefs.h"
#include "StdList.h"
#include "std/allocator/MemoryAllocator.h"

namespace izanagi {
    //
    template <typename _KEY, typename _T, typename _SELF>
    class CStdHashBase {
    public:
        class Item {
            friend class CStdHashBase<_KEY, _T, _SELF>;

        public:
            Item() : m_Key((IZ_UINT)0)
            {
                //m_HashItem.Init(this);
                //m_OrderItem.Init(this);
            }

            ~Item() {}

            // 初期化
            void Init(const _KEY& tKey, _T* pSelf)
            {
                m_HashItem.Init(this);
                m_OrderItem.Init(this);

                IZ_ASSERT(pSelf != NULL);
                m_pSelf = pSelf;

                m_Key = tKey;
            }

            // データを取得
            _T* GetData()
            {
                return m_pSelf;
            }

            // キーを取得
            const _KEY& GetKey() const
            {
                return m_Key;
            }

            void Leave()
            {
                m_HashItem.Leave();
                m_OrderItem.Leave();
            }

            IZ_BOOL HasList() const
            {
                return m_HashItem.HasList();
            }

        protected:
            typename CStdList<Item>::Item* GetHashItem() { return &m_HashItem; }
            typename CStdList<Item>::Item* GetOrderItem() { return &m_OrderItem; }

        protected:
            _KEY m_Key;
            _T* m_pSelf;
            typename CStdList<Item>::Item m_HashItem;
            typename CStdList<Item>::Item m_OrderItem;
        };

    protected:
        CStdHashBase() { m_GetHashListFunc = IZ_NULL; }
        ~CStdHashBase() {}

        CStdList<Item>* GetHashList(const _KEY& tKey)
        {
            IZ_ASSERT(m_GetHashListFunc != IZ_NULL);
            return (((_SELF*)this)->*m_GetHashListFunc)(tKey);
        }

    public:
        // 総データ数取得
        IZ_UINT GetDataNum() const
        {
            IZ_UINT ret = m_OrderList.GetItemNum();
            return ret;
        }

        // 登録
        IZ_BOOL Add(Item* pItem)
        {
            IZ_ASSERT(pItem != IZ_NULL);

            CStdList<Item>* pHashList = GetHashList(pItem->GetKey());
            IZ_ASSERT(pHashList != IZ_NULL);
            
            // ハッシュリストに登録
            IZ_BOOL ret = pHashList->AddTail(pItem->GetHashItem());
            if (ret) {
                // オーダーリストに登録
                ret = m_OrderList.AddTail(pItem->GetOrderItem());
            }

            if (!ret) {
                // 登録に失敗したので、リストから外す
                pItem->GetHashItem()->Leave();
                pItem->GetOrderItem()->Leave();
            }

            return ret;
        }

        Item* Find(const _KEY& tKey)
        {
            // 該当するハッシュリスト
            CStdList<Item>* pHashList = GetHashList(tKey);
            IZ_ASSERT(pHashList != IZ_NULL);
            
            // 探す
            CStdList<Item>::Item* pListItem = pHashList->GetTop();
            while (pListItem != IZ_NULL) {
                Item* pHashItem = pListItem->GetData();

                if (pHashItem->m_Key == tKey) {
                    // 見つかった
                    return pHashItem;
                }

                pListItem = pListItem->GetNext();
            }

            return IZ_NULL;
        }

        // キーに該当するデータを探す
        _T* FindData(const _KEY& tKey)
        {
            _T* ret = IZ_NULL;

            // 該当するハッシュリスト
            CStdList<Item>* pHashList = GetHashList(tKey);
            IZ_ASSERT(pHashList != IZ_NULL);
            
            // 探す
            CStdList<Item>::Item* pListItem = pHashList->GetTop();
            while (pListItem != IZ_NULL) {
                Item* pHashItem = pListItem->GetData();

                if (pHashItem->m_Key == tKey) {
                    // 見つかった
                    ret = pHashItem->GetData();
                    break;
                }

                pListItem = pListItem->GetNext();
            }

            return ret;
        }

        Item* GetOrderTopHashItem()
        {
            CStdList<Item>::Item* pOrderListItem = m_OrderList.GetTop();
            return (pOrderListItem != IZ_NULL ? pOrderListItem->GetData() : IZ_NULL);
        }
        Item* GetOrderTailHashItem()
        {
            CStdList<Item>::Item* pOrderListItem = m_OrderList.GetTail();
            return (pOrderListItem != IZ_NULL ? pOrderListItem->GetData() : IZ_NULL);
        }
        Item* GetOrderAtHashItem(IZ_UINT idx)
        {
            CStdList<Item>::Item* pOrderListItem = m_OrderList.GetAt(idx);
            return (pOrderListItem != IZ_NULL ? pOrderListItem->GetData() : IZ_NULL);
        }

        // うーん・・・
        typename CStdList<Item>::Item* GetOrderTop() { return m_OrderList.GetTop(); }
        typename CStdList<Item>::Item* GetOrderTail() { return m_OrderList.GetTail(); }

        typename CStdList<Item>::Item* GetHashTop(const _KEY& tKey)
        {
            CStdList<Item>* pHashList = GetHashList(tKey);
            IZ_ASSERT(pHashList != IZ_NULL);

            return pHashList->GetTop();
        }

    protected:
        // 登録された順に並ぶリスト
        CStdList<Item> m_OrderList;

        CStdList<Item>* (_SELF::*m_GetHashListFunc)(const _KEY&);
    };

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
