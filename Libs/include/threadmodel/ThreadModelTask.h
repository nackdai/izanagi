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
		/** �^�X�N����.
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

		/** �^�X�N�j��.
		 */
        static void DeleteTask(CTask* task);

    protected:
        CTask();
        virtual ~CTask();

    public:
        /** �^�X�N����������܂ő҂�.
         */
        void Wait();

        /** �^�X�N���L�����Z��.
         */
        IZ_BOOL Cancel();

        /** �^�X�N���L�����Z���\�肩�ǂ�����Ԃ�.
         */
        IZ_BOOL WillCancel();

        /** �^�X�N���L�����Z�����ꂽ���ǂ�����Ԃ�.
         */
        IZ_BOOL IsCanceled();

        /** �^�X�N���I���������ǂ�����Ԃ�.
         */
        IZ_BOOL IsFinished();

		/** �^�X�N���o�^�\���ǂ�����Ԃ�.
		 */
		IZ_BOOL CanRegister();

	private:
		IZ_BOOL Reset();

        IZ_BOOL ResetWithoutWait();

    protected:
        /** �^�X�N���s.
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

		// �^�X�N�I������delete���邩�ǂ�����ݒ肷��.
        void SetIsDeleteSelf(IZ_BOOL isDeleteSelf);

		// �^�X�N�I������delete���邩�ǂ�����Ԃ�.
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

		// �^�X�N�I������delete���邩�ǂ���
        IZ_BOOL m_IsDeleteSelf;

        enum State {
			State_Init = 0,		// �������.
			State_Registered,	// �o�^�ς�.
			State_Running,		// ���s��.
			State_Finished,		// �I��.
			State_WillCancel,	// �L�����Z���\��.
			State_Canceled,		// �L�����Z�����ꂽ.
        };

        State m_State;
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREAD_MODEL_TASK_H__)
