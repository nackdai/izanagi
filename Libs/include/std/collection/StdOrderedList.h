#if !defined(__IZANAGI_STD_ORDERED_LIST_H__)
#define __IZANAGI_STD_ORDERED_LIST_H__

#include "izDefs.h"
#include "std/collection/StdList.h"

namespace izanagi {
    /**
     */
    template <typename _T>
    class CStdOrderedList : public CStdList<_T> {
    public:
        CStdOrderedList()
            : CStdList()
        {
        }

        virtual ~CStdOrderedList()
        {
            Clear();
        }

        NO_COPIABLE(CStdOrderedList);

    public:
        IZ_BOOL Add(Item* pItem)
        {
            IZ_UINT nNum = GetItemNum();
            if (nNum == 0) {
                return pItem->JoinAfter(&m_Top);
            }
            else {
                IZ_UINT nTopIdx = 0;
                IZ_UINT nTailIdx = nNum - 1;

                Item* pTop = GetAt(nTopIdx);
                Item* pTail = GetAt(nTailIdx);

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
                    pMid = GetAt(nMidIdx);

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
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_STD_ORDERED_LIST_H__)
