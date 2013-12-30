#if !defined(__IZANAGI_STD_HASH_BASE_H__)
#define __IZANAGI_STD_HASH_BASE_H__

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
        IZ_BOOL Add(Item* pItem);

        Item* Find(const _KEY& tKey);

        // キーに該当するデータを探す
        _T* FindData(const _KEY& tKey);

        Item* GetOrderTopHashItem();
        Item* GetOrderTailHashItem();
        Item* GetOrderAtHashItem(IZ_UINT idx);

        // うーん・・・
        typename CStdList<Item>::Item* GetOrderTop();
        typename CStdList<Item>::Item* GetOrderTail();

        typename CStdList<Item>::Item* GetHashTop(const _KEY& tKey);

    protected:
        // 登録された順に並ぶリスト
        CStdList<Item> m_OrderList;

        CStdList<Item>* (_SELF::*m_GetHashListFunc)(const _KEY&);
    };

    template <typename _KEY, typename _T, typename _SELF>
    IZ_BOOL CStdHashBase<_KEY, _T, _SELF>::Add(typename CStdHashBase<_KEY, _T, _SELF>::Item* pItem)
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

    template <typename _KEY, typename _T, typename _SELF>
    typename CStdHashBase<_KEY, _T, _SELF>::Item* CStdHashBase<_KEY, _T, _SELF>::Find(const _KEY& tKey)
    {
        // 該当するハッシュリスト
        CStdList<Item>* pHashList = GetHashList(tKey);
        IZ_ASSERT(pHashList != IZ_NULL);
            
        // 探す
        typename CStdList<Item>::Item* pListItem = pHashList->GetTop();
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

    template <typename _KEY, typename _T, typename _SELF>
    _T* CStdHashBase<_KEY, _T, _SELF>::FindData(const _KEY& tKey)
    {
        _T* ret = IZ_NULL;

        // 該当するハッシュリスト
        CStdList<Item>* pHashList = GetHashList(tKey);
        IZ_ASSERT(pHashList != IZ_NULL);
            
        // 探す
        typename CStdList<Item>::Item* pListItem = pHashList->GetTop();
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

    template <typename _KEY, typename _T, typename _SELF>
    typename CStdHashBase<_KEY, _T, _SELF>::Item* CStdHashBase<_KEY, _T, _SELF>::GetOrderTopHashItem()
    {
        typename CStdList<typename CStdHashBase<_KEY, _T, _SELF>::Item>::Item* pOrderListItem = m_OrderList.GetTop();
        return (pOrderListItem != IZ_NULL ? pOrderListItem->GetData() : IZ_NULL);
    }

    template <typename _KEY, typename _T, typename _SELF>
    typename CStdHashBase<_KEY, _T, _SELF>::Item* CStdHashBase<_KEY, _T, _SELF>::GetOrderTailHashItem()
    {
        typename CStdList<typename CStdHashBase<_KEY, _T, _SELF>::Item>::Item* pOrderListItem = m_OrderList.GetTail();
        return (pOrderListItem != IZ_NULL ? pOrderListItem->GetData() : IZ_NULL);
    }

    template <typename _KEY, typename _T, typename _SELF>
    typename CStdHashBase<_KEY, _T, _SELF>::Item* CStdHashBase<_KEY, _T, _SELF>::GetOrderAtHashItem(IZ_UINT idx)
    {
        typename CStdList<typename CStdHashBase<_KEY, _T, _SELF>::Item>::Item* pOrderListItem = m_OrderList.GetAt(idx);
        return (pOrderListItem != IZ_NULL ? pOrderListItem->GetData() : IZ_NULL);
    }

    template <typename _KEY, typename _T, typename _SELF>
    typename CStdList<typename CStdHashBase<_KEY, _T, _SELF>::Item>::Item* CStdHashBase<_KEY, _T, _SELF>::GetOrderTop()
    {
        return this->m_OrderList.GetTop();
    }

    template <typename _KEY, typename _T, typename _SELF>
    typename CStdList<typename CStdHashBase<_KEY, _T, _SELF>::Item>::Item* CStdHashBase<_KEY, _T, _SELF>::GetOrderTail()
    {
        return this->m_OrderList.GetTail();
    }

    template <typename _KEY, typename _T, typename _SELF>
    typename CStdList<typename CStdHashBase<_KEY, _T, _SELF>::Item>::Item* CStdHashBase<_KEY, _T, _SELF>::GetHashTop(const _KEY& tKey)
    {
        CStdList<typename CStdHashBase<_KEY, _T, _SELF>::Item>* pHashList = GetHashList(tKey);
        IZ_ASSERT(pHashList != IZ_NULL);

        return pHashList->GetTop();
    }
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_STD_HASH_BASE_H__)
