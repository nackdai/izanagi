#if !defined(__IZANAGI_THREAD_MODEL_TIMER_THREAD_H__)
#define __IZANAGI_THREAD_MODEL_TIMER_THREAD_H__

#include "izStd.h"
#include "izSystem.h"

#include "ThreadModelTask.h"

namespace izanagi
{
namespace threadmodel
{
	/**
	*/
	class CTimerTask : public CTask
	{
		friend class CTimerThread;
		friend class CTimerTaskExecuter;

	protected:
		CTimerTask();
		virtual ~CTimerTask() {}

	public:
		/** タスク処理タイプ.
		*/
		enum TYPE {
			TYPE_DELAY = 0,	///< 時間経過.
			TYPE_INTERVAL,	///< 一定間隔.
		};

	private:
		// タスク実行タイプを設定.
		inline void SetType(TYPE type);

		// タスク実行タイプを取得.
		inline TYPE GetType();

		// タスクが処理される時間を設定.
		inline void SetTimeTaskWillExectute(IZ_TIME time);

		// タスクが処理される時間を取得.
		inline IZ_TIME GetTimeTaskWillExectute();

		// 以前の時間を設定.
		inline void SetPrev(IZ_TIME time);

		// 以前の時間を取得.
		inline IZ_TIME GetPrev();

		// 処理間隔を設定.
		inline void SetInterval(IZ_FLOAT ms);

		// 処理感覚を取得.
		inline IZ_FLOAT GetInterval();

		// 経過時間を設定.
		inline void SetElapsed(IZ_FLOAT ms);

		// 経過時間を取得.
		inline IZ_FLOAT GetElapsed();

		// OnRunに渡す時間を設定.
		inline void SetTimeForRun(IZ_FLOAT time);

		virtual void OnRun();

	protected:
		virtual void OnRun(IZ_FLOAT time) = 0;

	private:
		TYPE m_Type;
		IZ_TIME m_Time;
		IZ_TIME m_Prev;
		IZ_FLOAT m_Interval;
		IZ_FLOAT m_Elapsed;

		IZ_FLOAT m_TempTime;
	};

	/** 時間に応じてタスク実行.
	*/
	class CTimerTaskExecuter {
	public:
		CTimerTaskExecuter();
		~CTimerTaskExecuter();

		NO_COPIABLE(CTimerTaskExecuter);

	public:
		/** 現在の時間から指定時間だけ経過したら実行するようにタスクを登録.
		*/
		IZ_BOOL PostTask(
			CTimerTask* task,
			CTimerTask::TYPE type,
			IZ_TIME current,
			IZ_FLOAT time,
			IZ_BOOL willDelete = IZ_FALSE);

		/** 現在の時間から指定時間だけ経過したら実行するようにタスクを登録.
		*/
		IZ_BOOL PostTask(
			CTimerTask* task,
			CTimerTask::TYPE type,
			IZ_FLOAT time,
			IZ_BOOL willDelete = IZ_FALSE);

		void Update();

		void Clear();

		IZ_UINT GetRegisteredTaskNum();

		void Cancel(
			std::function<IZ_BOOL(CTimerTask*, void*)> funcIsCancel,
			void* userData);

	private:
		CStdList<CTimerTask> m_TaskList;

		typedef CStdList<CTimerTask>::Item ListItem;
	};

    /**
     */
    class CTimerThread : public sys::CThread
    {
    public:
        CTimerThread();
        ~CTimerThread();

		NO_COPIABLE(CTimerThread);

    private:
        virtual void Run() final;

    public:
        IZ_BOOL PostDelayedTask(
			IMemoryAllocator* allocator,
            CTimerTask* task, 
            IZ_FLOAT delay, 
            IZ_BOOL willDelete = IZ_FALSE);

        IZ_BOOL PostIntervalTask(
			IMemoryAllocator* allocator,
            CTimerTask* task, 
            IZ_FLOAT interval, 
            IZ_BOOL willDelete = IZ_FALSE);

        virtual void Join() final;

    private:
        IZ_BOOL PostTaskInternal(
			IMemoryAllocator* allocator,
            CTimerTask* task, 
            CTimerTask::TYPE type,
            IZ_FLOAT time, 
            IZ_BOOL willDelete = IZ_FALSE);

        virtual IZ_BOOL Start(
            std::function<void(void*)> func,
            void* userData) final
        {
            IZ_ASSERT(IZ_FALSE);
            return IZ_FALSE;
        }

		virtual IZ_BOOL Start() final
		{
			IZ_ASSERT(IZ_FALSE);
			return IZ_FALSE;
		}

    private:
        enum STATE {
            STATE_NONE = 0,
            STATE_INITIALIZED,
            STATE_TERMINATED,
        };

        STATE m_State;

        sys::CTimer m_Timer;
        std::mutex m_Mutex;
        sys::CEvent m_Event;
        
        CTimerTaskExecuter m_TaskExecuter;

        IZ_BOOL m_WillTerminate;
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREAD_MODEL_TIMER_THREAD_H__)
