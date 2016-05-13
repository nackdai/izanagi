#if !defined(__IZANAGI_THREAD_MODEL_TASK_H__)
#define __IZANAGI_THREAD_MODEL_TASK_H__

#include "izStd.h"
#include "izSystem.h"

namespace izanagi
{
namespace threadmodel
{
    /**
     */
    class CTask : public CPlacementNew
    {
        friend class CThreadPool;
        friend class CParallel;
        friend class CTimerTaskExecuter;

    public:
		/** タスク生成.
		 */
        template <typename _T>
        static _T* CreateTask(IMemoryAllocator* allocator)
        {
            void* buf = ALLOC(allocator, sizeof(_T));
            VRETURN_NULL(buf != IZ_NULL);

            _T* ret = new(buf) _T();
            {
                ret->m_Allocator = allocator;
            }

            return ret;
        }

		/** タスク破棄.
		 */
        static void DeleteTask(CTask* task);

    protected:
        CTask();
        virtual ~CTask();

    public:
        /** タスクが処理するまで待つ.
         */
        void Wait();

        /** タスクをキャンセル.
         */
        IZ_BOOL Cancel();

        /** タスクがキャンセル予定かどうかを返す.
         */
        IZ_BOOL WillCancel();

        /** タスクがキャンセルされたかどうかを返す.
         */
        IZ_BOOL IsCanceled();

        /** タスクが終了したかどうかを返す.
         */
        IZ_BOOL IsFinished();

		/** タスクが登録可能かどうかを返す.
		 */
		IZ_BOOL CanRegister();

	private:
		IZ_BOOL Reset();

        IZ_BOOL ResetWithoutWait();

    protected:
        /** タスク実行.
         */
        virtual void OnRun() = 0;

    private:
        void Run(void* userData);

		template <typename _T>
		IZ_BOOL RegisterTo(CStdList<_T>& list)
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			list.AddTail(reinterpret_cast<CStdList<_T>::Item*>(&m_ListItem));
			m_State = State_Registered;
			return IZ_TRUE;
		}

        void SetAllocator(IMemoryAllocator* allocator);

		// タスク終了時にdeleteするかどうかを設定する.
        void SetIsDeleteSelf(IZ_BOOL isDeleteSelf);

		// タスク終了時にdeleteするかどうかを返す.
        IZ_BOOL IsDeleteSelf();

		CStdList<CTask>::Item* GetListItem()
		{
			return &m_ListItem;
		}

    private:
        IMemoryAllocator* m_Allocator;

		std::mutex m_mutex;
		std::condition_variable m_condVar;

        CStdList<CTask>::Item m_ListItem;

		// タスク終了時にdeleteするかどうか
        IZ_BOOL m_IsDeleteSelf;

        enum State {
			State_Init = 0,		// 初期状態.
			State_Registered,	// 登録済み.
			State_Running,		// 実行中.
			State_Finished,		// 終了.
			State_WillCancel,	// キャンセル予定.
			State_Canceled,		// キャンセルされた.
        };

        State m_State;
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREAD_MODEL_TASK_H__)
