#if !defined(__URANUS_STD_TASK_QUEUE_H__)
#define __URANUS_STD_TASK_QUEUE_H__

#include "unDefs.h"

namespace {
	/**
	* �L���[
	*/
	template<
		class _T,
		UN_UINT _NUM
	>
	class CQueue {
	public:
		CQueue();
		~CQueue() {}
		
	private:
		CQueue(const CQueue& rhs);
		const CQueue& operator =(const CQueue& rhs);
		
	public:
		// ������
		void Init();

	public:		
		// �L���[����t��
		UN_BOOL IsQueueFull() const;

		// �L���[����
		UN_BOOL IsQueueEmpty() const;
		
		// �A�C�e�����L���[����擾
		_T& GetEndFromQueue();
		_T& GetFrontFromQueue();
		
		// ���̃C���f�b�N�X���擾
		UN_UINT16 GetNextIndex(UN_UINT16 idx) const;
		
		// �L���[���v�b�V������
		UN_BOOL PushQueue();
		
		// �L���[���|�b�v����
		UN_BOOL PopQueue();

	public:
		// �ő�o�^�\�A�C�e�����擾
		UN_UINT GetMaxNum() const;

		// �o�^�A�C�e�����擾
		UN_UINT GetNum() const;
					
	private:
		_T m_Items[_NUM];

		struct {
			UN_INT idxFront;
			UN_INT idxRear;
			UN_INT num;
		} m_QueueParam;
	};

	/**
	* �R���X�g���N�^
	*/
	template<class _T, UN_UINT _NUM>
	CQueue<_T, _NUM>::CQueue()
	{
	}

	/**
	* ������
	*/
	template<class _T, UN_UINT _NUM>
	void CQueue<_T, _NUM>::Init()
	{		
		m_QueueParam.idxFront = 0;
		m_QueueParam.idxRear = 0;
		m_QueueParam.num = 0;		
	}

	/**
	* �L���[����t��
	*/
	template<class _T, UN_UINT _NUM>
	UN_BOOL CQueue<_T, _NUM>::IsQueueFull() const
	{
		return (m_QueueParam.num == _NUM);
	}

	/**
	* �A�C�e�����L���[����擾
	*/
	template<class _T, UN_UINT _NUM>
	_T& CQueue<_T, _NUM>::GetEndFromQueue()
	{
		UN_ASSERT(m_QueueParam.idxRear < _NUM);
		
		return m_Items[m_QueueParam.idxRear];
	}

	template<class _T, UN_UINT _NUM>
	_T& CQueue<_T, _NUM>::GetFrontFromQueue()
	{
		UN_ASSERT(m_QueueParam.idxFront < _NUM);
		
		return m_Items[m_QueueParam.idxFront];
	}

	/**
	* �L���[����
	*/
	template<class _T, UN_UINT _NUM>
	UN_BOOL CQueue<_T, _NUM>::IsQueueEmpty() const
	{
		return (m_QueueParam.num == 0);
	}

	/**
	* ���̃C���f�b�N�X���擾
	*/
	template<class _T, UN_UINT _NUM>
	UN_UINT16 CQueue<_T, _NUM>::GetNextIndex(UN_UINT16 idx) const
	{
		UN_C_ASSERT(_NUM > 0);
		return (UINT16)((idx + 1) % _NUM);
	}

	/**
	* �L���[���v�b�V������
	*/
	template<class _T, UN_UINT _NUM>
	UN_BOOL CQueue<_T, _NUM>::PushQueue()
	{
		if (!IsQueueFull()) {
			m_QueueParam.idxRear = GetNextIndex(m_QueueParam.idxRear);
			m_QueueParam.num++;
			return UN_TRUE;
		}
		else {
			return UN_FALSE;
		}
	}

	/**
	* �L���[���|�b�v����
	*/
	template<class _T, UN_UINT _NUM>
	UN_BOOL CQueue<_T, _NUM>::PopQueue()
	{
		if (!IsQueueEmpty()) {
			m_QueueParam.idxFront = GetNextIndex(m_QueueParam.idxFront);
			m_QueueParam.num--;
			return UN_TRUE;
		}
		else {
			return UN_FALSE;
		}
	}

	/**
	* �ő�o�^�\�A�C�e�����擾
	*/
	template<class _T, UN_UINT _NUM>
	UN_UINT CQueue<_T, _NUM>::GetMaxNum() const
	{
		return _NUM;
	}

	// �o�^�A�C�e�����擾
	template<class _T, UN_UINT _NUM>
	UN_UINT CQueue<_T, _NUM>::GetNum() const
	{
		return m_QueueParam.num;
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_STD_TASK_QUEUE_H__)
