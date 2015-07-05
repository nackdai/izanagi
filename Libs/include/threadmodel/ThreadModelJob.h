#if !defined(__IZANAGI_THREAD_MODEL_JOB_H__)
#define __IZANAGI_THREAD_MODEL_JOB_H__

#include "izStd.h"
#include "izSystem.h"

namespace izanagi
{
namespace threadmodel
{
    class CJobQueue;

    /** ジョブのベース
     */
    class CJob : public CPlacementNew
    {
        friend class CJobQueue;
        friend class CJobWorker;

    public:
        enum State
        {
            State_None = 0,
            State_Waiting,
            State_Registered,
            State_Running,
            State_WillFinish,
            State_Finished,
        };

        template <typename _T>
        static CJob* CreateJob(IMemoryAllocator* allocator)
        {
            void* buf = ALLOC(allocator, sizeof(_T));
            VRETURN_NULL(buf != IZ_NULL);

            _T* ret = new(buf) _T();
            {
                ret->m_Allocator = allocator;
            }

            return ret;
        }

        static void DeleteJob(CJob*& job);
        
    protected:
        CJob();
        virtual ~CJob();

        IZ_DEFINE_INTERNAL_RELEASE();

    public:
        /** キャンセル.
         */
        void Cancel();

        /** 状態を取得
         */
        State GetState();

        /** キャンセルされたかどうか
         */
        IZ_BOOL IsCanceled();

    protected:
        /**
         */
        virtual IZ_BOOL OnRun() = 0;

        /**
         */
        virtual void OnFinish(IZ_BOOL runResult);

        /**
         */
        virtual void OnCancel();

    private:
        void Run();
        void Finish();

        CStdList<CJob>::Item* GetListItem() { return &m_ListItem; }

        void SetState(State state);

        // 指定されたジョブキューに登録済みかどうか
        IZ_BOOL IsRegistered(CJobQueue* jobQueue);

        void SetEnableDeleteWhenFinish();
        IZ_BOOL EnableDeleteWhenFinish();

    private:
        IMemoryAllocator* m_Allocator;

        CStdList<CJob>::Item m_ListItem;

        // 登録されているジョブキュー
        CJobQueue* m_JobQueue;

        std::mutex m_Mutex;

        State m_State;
        IZ_BOOL m_RunResult;

        IZ_BOOL m_IsCanceled;

        IZ_BOOL m_EnableDeleteWhenFinish;
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREAD_MODEL_JOB_H__)
