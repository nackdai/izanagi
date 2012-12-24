#if !defined(__IZANAGI_STD_QUEUE_H__)
#define __IZANAGI_STD_QUEUE_H__

#include "izDefs.h"
#include "StdList.h"

namespace izanagi
{
    /** キュー
     */
    template <typename _T>
    class CStdQueue
    {
    public:
        typedef typename CStdList<_T>::Item  Item;

    public:
        CStdQueue() {}
        ~CStdQueue() {}

        NO_COPIABLE(CStdQueue);

    public:
        /** 登録要素数取得
         */
        IZ_UINT GetItemNum() const
        {
            return m_List.GetItemNum();
        }

        /** 要素が登録されているかどうか
         */
        IZ_BOOL HasItem()
        {
            return m_List.HasItem();
        }

        /** エンキュー
         */
        IZ_BOOL Enqueue(Item* item)
        {
            return m_List.AddTail(item);
        }

        /** デキュー
         */
        Item* Dequeue()
        {
            Item* ret = m_List.GetTop();
            if (ret != IZ_NULL)
            {
                // リストから外れる
                ret->Leave();
            }
            return ret;
        }

        /** キューから削除
         */
        IZ_BOOL Remove(Item* item)
        {
            IZ_ASSERT(item != IZ_NULL);

            // そもそも登録されているものか
            IZ_BOOL isRegistered = (item->GetList() == &m_List);

            IZ_BOOL ret = isRegistered;

            if (isRegistered)
            {
                item->Leave();
            }

            return ret;
        }

        /** キューをクリア
         */
        void Clear()
        {
            m_List.Clear();
        }

        /** 指定された要素を持っているかどうか
         */ 
        IZ_BOOL HasItem(Item* item)
        {
            return (item->GetList() == &m_List);
        }

        operator CStdList<_T>&()
        {
            return m_List;
        }

        operator CStdList<_T>*()
        {
            return &m_List;
        }

    private:
        CStdList<_T> m_List;
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_STD_LIST_H__)
