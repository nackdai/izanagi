#include "std/StdProfiler.h"
#include "std/allocator/MemoryAllocator.h"

namespace izanagi
{
    class CStdProfiler::CProfileItem : public CPlacementNew
    {
    public:
        CProfileItem()
        {
            ListItem.Init(this);

            Depth = 0;
            Time = 0;
            Tag = IZ_NULL;
            Parent = IZ_NULL;
        }

        ~CProfileItem()
        {
            ListItem.Leave();
        }

        void Dump(const IStdProfileWriter* writer)
        {
            if (writer != IZ_NULL) {
                for (IZ_INT i = 0; i < Depth; i++) {
                    writer->Write(" ");
                }
                writer->Write("%s : %f\n", Tag, Time);
            }
            else {
                for (IZ_INT i = 0; i < Depth; i++) {
                    IZ_PRINTF(" ");
                }
                IZ_PRINTF("%s : %f\n", Tag, Time);
            }
        }

        IZ_INT Depth;
        IZ_FLOAT Time;
        const char* Tag;

        CProfileItem* Parent;

        CListItem ListItem;
    };

    IZ_BOOL CStdProfiler::s_Enable = IZ_TRUE;
    CStdList<CStdProfiler::CProfileItem> CStdProfiler::s_ItemList;
    IMemoryAllocator* CStdProfiler::s_Allocator = IZ_NULL;
    IStdProfilerTimer* CStdProfiler::s_Timer = IZ_NULL;
    const IStdProfileWriter* CStdProfiler::s_Writer = IZ_NULL;
    CStdProfiler::CProfileItem* CStdProfiler::s_Current = IZ_NULL;
    IZ_INT CStdProfiler::s_Depth = 0;
    
    void CStdProfiler::Init(
        IMemoryAllocator* allocator,
        IStdProfilerTimer* timer,
        const IStdProfileWriter* writer)
    {
        Terminate();

        s_Allocator = allocator;
        s_Timer = timer;
        s_Writer = writer;

        IZ_ASSERT(s_Timer != IZ_NULL);
        s_Timer->Begin();
    }

    void CStdProfiler::Terminate()
    {
        CListItem* listItem = s_ItemList.GetTop();

        while (listItem != IZ_NULL) {
            CProfileItem* item = listItem->GetData();

            listItem = listItem->GetNext();

            delete item;
            FREE(s_Allocator, item);
        }
    }

    void CStdProfiler::Begin(const char* tag)
    {
        if (s_Enable) {
            void* buf = ALLOC(s_Allocator, sizeof(CProfileItem));
            CProfileItem* item = new(buf) CProfileItem;

            item->Tag = tag;
            item->Depth = s_Depth++;

            if (s_Current != IZ_NULL) {
                item->Parent = s_Current;
            }
            s_Current = item;

            s_ItemList.AddTail(&item->ListItem);

            item->Time = s_Timer->GetElapsedTime();
        }
    }

    void CStdProfiler::End()
    {
        if (s_Enable) {
            IZ_FLOAT time = s_Timer->GetElapsedTime();

            s_Current->Time = time - s_Current->Time;

            s_Current = s_Current->Parent;

            s_Depth--;
            IZ_ASSERT(s_Depth >= 0);

            if (s_Depth == 0) {
                // Dump items & Clear lists.

                CListItem* listItem = s_ItemList.GetTop();

                while (listItem != IZ_NULL) {
                    CProfileItem* item = listItem->GetData();

                    item->Dump(s_Writer);

                    listItem = listItem->GetNext();

                    delete item;
                    FREE(s_Allocator, item);
                }
            }
        }
    }

    void CStdProfiler::Enable(IZ_BOOL enable)
    {
        s_Enable = enable;
    }

}   // namespace izanagi
