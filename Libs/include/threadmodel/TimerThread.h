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
		/** �^�X�N�����^�C�v.
		*/
		enum TYPE {
			TYPE_DELAY = 0,	///< ���Ԍo��.
			TYPE_INTERVAL,	///< ���Ԋu.
		};

	private:
		// �^�X�N���s�^�C�v��ݒ�.
		inline void SetType(TYPE type);

		// �^�X�N���s�^�C�v���擾.
		inline TYPE GetType();

		// �^�X�N����������鎞�Ԃ�ݒ�.
		inline void SetTimeTaskWillExectute(IZ_TIME time);

		// �^�X�N����������鎞�Ԃ��擾.
		inline IZ_TIME GetTimeTaskWillExectute();

		// �ȑO�̎��Ԃ�ݒ�.
		inline void SetPrev(IZ_TIME time);

		// �ȑO�̎��Ԃ��擾.
		inline IZ_TIME GetPrev();

		// �����Ԋu��ݒ�.
		inline void SetInterval(IZ_FLOAT ms);

		// �������o���擾.
		inline IZ_FLOAT GetInterval();

		// �o�ߎ��Ԃ�ݒ�.
		inline void SetElapsed(IZ_FLOAT ms);

		// �o�ߎ��Ԃ��擾.
		inline IZ_FLOAT GetElapsed();

		// OnRun�ɓn�����Ԃ�ݒ�.
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

	/** ���Ԃɉ����ă^�X�N���s.
	*/
	class CTimerTaskExecuter {
	public:
		CTimerTaskExecuter();
		~CTimerTaskExecuter();

		NO_COPIABLE(CTimerTaskExecuter);

	public:
		/** ���݂̎��Ԃ���w�莞�Ԃ����o�߂�������s����悤�Ƀ^�X�N��o�^.
		*/
		IZ_BOOL PostTask(
			CTimerTask* task,
			CTimerTask::TYPE type,
			IZ_TIME current,
			IZ_FLOAT time,
			IZ_BOOL willDelete = IZ_FALSE);

		/** ���݂̎��Ԃ���w�莞�Ԃ����o�߂�������s����悤�Ƀ^�X�N��o�^.
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
