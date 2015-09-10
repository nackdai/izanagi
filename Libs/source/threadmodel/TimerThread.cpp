#include "threadmodel/TimerThread.h"

namespace izanagi
{
namespace threadmodel
{
	CTimerTask::CTimerTask()
	{
		m_Type = TYPE_DELAY;
		sys::CTimer::SetTimeZero(m_Time);
		sys::CTimer::SetTimeZero(m_Prev);
		m_Interval = 0;
		m_Elapsed = 0;
	}

	void CTimerTask::SetType(CTimerTask::TYPE type)
	{
		m_Type = type;
	}

	CTimerTask::TYPE CTimerTask::GetType()
	{
		return m_Type;
	}

	void CTimerTask::SetTimeTaskWillExectute(IZ_TIME ms)
	{
		m_Time = ms;
	}

	IZ_TIME CTimerTask::GetTimeTaskWillExectute()
	{
		return m_Time;
	}

	void CTimerTask::SetPrev(IZ_TIME ms)
	{
		m_Prev = ms;
	}

	IZ_TIME CTimerTask::GetPrev()
	{
		return m_Prev;
	}

	void CTimerTask::SetInterval(IZ_FLOAT ms)
	{
		m_Interval = ms;
	}

	IZ_FLOAT CTimerTask::GetInterval()
	{
		return m_Interval;
	}

	void CTimerTask::SetElapsed(IZ_FLOAT ms)
	{
		m_Elapsed = ms;
	}

	IZ_FLOAT CTimerTask::GetElapsed()
	{
		return m_Elapsed;
	}

	void CTimerTask::SetTimeForRun(IZ_FLOAT time)
	{
		m_TempTime = time;
	}

	void CTimerTask::OnRun()
	{
		OnRun(m_TempTime);
	}

	///////////////////////////////////////////////////

	CTimerTaskExecuter::CTimerTaskExecuter()
	{
	}

	CTimerTaskExecuter::~CTimerTaskExecuter()
	{
		Clear();
	}

	// 現在の時間から指定時間だけ経過したら実行するようにタスクを登録.
	IZ_BOOL CTimerTaskExecuter::PostTask(
		CTimerTask* task,
		CTimerTask::TYPE type,
		IZ_TIME current,
		IZ_FLOAT time,
		IZ_BOOL willDelete/*= IZ_FALSE*/)
	{
		if (!task->CanRegister()) {
			return IZ_FALSE;
		}

		task->SetType(type);

		if (type == CTimerTask::TYPE_INTERVAL) {
			task->SetInterval(time);
		}

		task->SetIsDeleteSelf(willDelete);

		task->SetTimeTaskWillExectute(sys::CTimer::Add(current, time));
		task->SetPrev(current);

		auto ret = task->RegisterTo(m_TaskList);
		return ret;
	}

	// 現在の時間から指定時間だけ経過したら実行するようにタスクを登録.
	IZ_BOOL CTimerTaskExecuter::PostTask(
		CTimerTask* task,
		CTimerTask::TYPE type,
		IZ_FLOAT time,
		IZ_BOOL willDelete/*= IZ_FALSE*/)
	{
		// 現在の時間を取得
		IZ_TIME cur = sys::CTimer::GetCurTime();

		return PostTask(
			task,
			type,
			cur,
			time,
			willDelete);
	}

	void CTimerTaskExecuter::Update()
	{
		ListItem* item = m_TaskList.GetTop();
		while (item != IZ_NULL) {
			CTimerTask* task = item->GetData();
			ListItem* next = item->GetNext();

			// タスク処理タイプ.
			CTimerTask::TYPE type = task->GetType();

			// 実行されるタイミング.
			IZ_TIME timeWillExecute = task->GetTimeTaskWillExectute();

			IZ_TIME cur = sys::CTimer::GetCurTime();
			IZ_TIME base = cur;

			IZ_FLOAT taskElapsed = task->GetElapsed();

			if (taskElapsed > 0) {
				base = sys::CTimer::Sub(cur, taskElapsed);
			}

			if (task->WillCancel()
				|| sys::CTimer::GreaterThan(base, timeWillExecute))
			{
				// タスクがキャンセルされた or 時間を過ぎた

				// 経過時間を計算
				IZ_FLOAT elapsed = sys::CTimer::ComputeTime(
					task->GetPrev(),
					cur);

				if (type == CTimerTask::TYPE_DELAY) {
					// 時間経過で処理.

					// １回のみの処理なのでリストから抜く.
					item->Leave();

					task->SetTimeForRun(elapsed);
					task->Run(IZ_NULL);

					if (task->IsDeleteSelf()) {
						CTask::DeleteTask(task);
						task = IZ_NULL;
					}
				}
				else {
					// 一定間隔で処理.
					task->SetTimeForRun(elapsed);
					task->Run(IZ_NULL);

					if (task->IsCanceled()) {
						// キャンセルされているときはリストから抜く.
						item->Leave();

						if (task->IsDeleteSelf()) {
							CTask::DeleteTask(task);
							task = IZ_NULL;
						}
					}
					else {
						// 次に備える.
						IZ_FLOAT interval = task->GetInterval();

						// 次の処理タイミングを設定.
						task->SetTimeTaskWillExectute(sys::CTimer::Add(cur, interval));
						task->SetPrev(cur);

						task->SetElapsed(elapsed - interval);
					}
				}
			}

			item = next;
		}
	}

	void CTimerTaskExecuter::Clear()
	{
		// 後始末
		ListItem* item = m_TaskList.GetTop();

		while (item != IZ_NULL) {
			CTimerTask* task = item->GetData();
			item = item->GetNext();

			task->Cancel();
			task->Run(IZ_NULL);

			if (task->IsDeleteSelf()) {
				CTask::DeleteTask(task);
			}
		}

		m_TaskList.Clear();
	}

	IZ_UINT CTimerTaskExecuter::GetRegisteredTaskNum()
	{
		return m_TaskList.GetItemNum();
	}

	void CTimerTaskExecuter::Cancel(
		std::function<IZ_BOOL(CTimerTask*, void*)> funcIsCancel,
		void* userData)
	{
		IZ_ASSERT(funcIsCancel);

		ListItem* item = m_TaskList.GetTop();

		while (item != IZ_NULL) {
			ListItem* next = item->GetNext();
			CTimerTask* task = item->GetData();

			if (funcIsCancel(task, userData)) {
				item->Leave();

				task->Cancel();
				task->Run(IZ_NULL);

				if (task->IsDeleteSelf()) {
					CTask::DeleteTask(task);
				}
			}

			item = next;
		}
	}

    ///////////////////////////////////////////////////

    CTimerThread::CTimerThread()
    {
        m_WillTerminate = IZ_FALSE;

        m_State = CTimerThread::STATE_NONE;
    }

    CTimerThread::~CTimerThread()
    {
    }

    void CTimerThread::Run()
    {
        m_Timer.Begin();

        while (IZ_TRUE) {
            m_Event.Wait();

            {
                std::lock_guard<std::mutex> lock(m_Mutex);

                if (m_WillTerminate) {
                    break;
                }

                m_TaskExecuter.Update();

                if (m_TaskExecuter.GetRegisteredTaskNum() == 0) {
                    // There is no task, so thread wait.
                    m_Event.Reset();
                }
            }
        }
    }

    void CTimerThread::Join()
    {
        if (m_State == STATE_TERMINATED) {
            return;
        }

        m_WillTerminate = IZ_TRUE;

        // Thread may be waited by event.
        m_Event.Set();

        sys::CThread::Join();

        // 後始末
        m_TaskExecuter.Clear();

        m_State = STATE_TERMINATED;
    }

    IZ_BOOL CTimerThread::PostDelayedTask(
		IMemoryAllocator* allocator,
        CTimerTask* task, 
        IZ_FLOAT delay, 
        IZ_BOOL willDelete/*= IZ_FALSE*/)
    {
        return PostTaskInternal(
			allocator,
            task,
            CTimerTask::TYPE_DELAY,
            delay, 
            willDelete);
    }

    IZ_BOOL CTimerThread::PostIntervalTask(
		IMemoryAllocator* allocator,
        CTimerTask* task, 
        IZ_FLOAT interval, 
        IZ_BOOL willDelete/*= IZ_FALSE*/)
    {
        return PostTaskInternal(
			allocator,
            task, 
            CTimerTask::TYPE_INTERVAL,
            interval,
            willDelete);
    }

    IZ_BOOL CTimerThread::PostTaskInternal(
		IMemoryAllocator* allocator,
        CTimerTask* task, 
        CTimerTask::TYPE type,
        IZ_FLOAT time, 
        IZ_BOOL willDelete/*= IZ_FALSE*/)
    {
        if (m_WillTerminate) {
            return IZ_FALSE;
        }

        if (m_State == STATE_TERMINATED) {
            return IZ_FALSE;
        }

        if (m_State == STATE_NONE) {
			VRETURN(sys::CThread::Start());
            m_State = STATE_INITIALIZED;
        }

        {
			std::lock_guard<std::mutex> lock(m_Mutex);
            m_TaskExecuter.PostTask(task, type, time, willDelete);
        }

        m_Event.Set();

        return IZ_TRUE;
    }
}   // namespace threadmodel
}   // namespace izanagi
