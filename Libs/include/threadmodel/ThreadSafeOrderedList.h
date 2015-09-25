#if !defined(__IZANAGI_THREADMODEL_ORDERED_LIST_H__)
#define __IZANAGI_THREADMODEL_ORDERED_LIST_H__

#include "izDefs.h"
#include "threadmodel/ThreadSafeList.h"

namespace izanagi {
namespace threadmodel {
    /**
     */
    template <typename _T>
    class ThreadSafeOrderedList : public ThreadSafeList<_T> {
    public:
        ThreadSafeOrderedList()
            : ThreadSafeList<_T>()
        {
        }

        virtual ~ThreadSafeOrderedList()
        {
            Clear();
        }

        NO_COPIABLE(ThreadSafeOrderedList);

    public:
        IZ_BOOL Add(ThreadSafeList<_T>::Item* item)
        {
            sys::Lock locker(GetLocker());

            CStdList<_T>::Item* pItem = item;

            IZ_UINT nNum = CStdList<_T>::GetItemNum();
            if (nNum == 0) {
                return pItem->JoinAfter(&m_Top);
            }
            else {
                IZ_UINT nTopIdx = 0;
                IZ_UINT nTailIdx = nNum - 1;

                Item* pTop = CStdList<_T>::GetAt(nTopIdx);
                Item* pTail = CStdList<_T>::GetAt(nTailIdx);

                if (*pTop > *pItem) {
                    // Add to top.
                    return pItem->JoinBefore(pTop);
                }
                else {
                    // Add to tail.
                    return pItem->JoinBefore(pTail);
                }

                Item* pMid = IZ_NULL;

                // Search insert position by binary search.
                while (nTopIdx <= nTailIdx) {
                    IZ_UINT nMidIdx = (nTopIdx + nTailIdx) >> 1;
                    pMid = CStdList<_T>::GetAt(nMidIdx);

                    if (*pItem > (*pMid)) {
                        nTopIdx = nMidIdx + 1;
                    }
                    else {
                        nTailIdx = nMidIdx - 1;
                    }
                }

                // Insert item.
                for (Item* p = pMid; p != IZ_NULL; p = p->GetNext()) {
                    if (*pItem > *p) {
                        return pItem->JoinAfter(p);
                    }
                }

                IZ_ASSERT(IZ_FALSE);
                return IZ_FALSE;
            }
        }
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREADMODEL_ORDERED_LIST_H__)
