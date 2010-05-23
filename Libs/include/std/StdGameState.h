#if !defined(__URANUS_STD_GAME_STATE_H__)
#define __URANUS_STD_GAME_STATE_H__

#include "unDefs.h"

namespace uranus {
	////////////////////////////////////////////////////////
	/**
	* �X�e�[�g�x�[�X
	*/
	class CGameState {
	public:
		CGameState() {}
		
		// �Ă΂�Ȃ��̂ŁA�p�����Ă��������Ȃ����ƁI�I
		virtual ~CGameState() {}
		
	private:
		CGameState(const CGameState& rhs);
		const CGameState& operator =(const CGameState& rhs);
		
	public:
		virtual UN_BOOL Create();
		virtual UN_BOOL Render();
		virtual UN_BOOL Update();
		virtual UN_BOOL Destroy();
		virtual UN_BOOL Enter();
		virtual UN_BOOL Leave();

#if 1
		// For Windows
		virtual UN_BOOL OnKeyDown(UN_UINT nChar, UN_UINT nRepCnt, UN_UINT nFlags);
		virtual UN_BOOL OnMouseMove(UN_UINT nFlags, UN_INT x, UN_INT y);
		virtual UN_BOOL OnMouseWheel(UN_UINT nFlags, UN_SHORT zDelta, UN_INT x, UN_INT y);
#endif
	};

	////////////////////////////////////////////////////////
	/**
	* �X�e�[�g�Ǘ�
	*/
	template <
		typename STATE,
		UN_UINT STATE_NUM
	>
	class CGameStateManagerBase {
	private:
		enum {
			StateNumInternal = STATE_NUM,
		};

	public:
		CGameStateManagerBase() {}
		~CGameStateManagerBase() {}

	public:
		UN_BOOL Create();
		UN_BOOL Render();
		UN_BOOL Update();
		UN_BOOL Destroy();

		// For Windows
		UN_BOOL OnKeyDown(UN_UINT nChar, UN_UINT nRepCnt, UN_UINT nFlags);
		UN_BOOL OnMouseMove(UN_UINT nFlags, UN_INT x, UN_INT y);
		UN_BOOL OnMouseWheel(UN_UINT nFlags, UN_SHORT zDelta, UN_INT x, UN_INT y);

	public:
		void ChangeState(STATE task);
		void ChangePrevState();
		
		CGameState* GetState(STATE task);
		
		UN_BOOL IsCurrentState(STATE task);
		UN_BOOL IsCurrentState(CGameState* task);

		UN_UINT GetStateNum();
		
	public:
		void SetCurrentState(STATE task) { m_nCurrentState = task; }
			
	public:
		STATE GetCurrentState() { return m_nCurrentState; }
		STATE GetPrevState() { return m_nPrevState; }

	protected:
		CGameState* m_pStateTable[STATE_NUM];
	
		STATE m_nCurrentState;	// ���݂̃^�X�N
		STATE m_nNextState;		// ���ɑJ�ڂ���^�X�N
		STATE m_nPrevState;		// �J�ڑO�̃^�X�N
	};

	// **********************

	/**
	*/
	template <typename STATE, UN_UINT STATE_NUM>
	UN_BOOL CGameStateManagerBase<STATE, STATE_NUM>::Create()
	{
		for (UN_INT i = 0; i < STATE_NUM; i++) {
			if (!m_pStateTable[i]->Create()) {
				UN_ASSERT(UN_FALSE);
				return UN_FALSE;
			}
		}
		
		// �J�n�^�X�N�����s
		ChangeState((STATE)0);
		
		return UN_TRUE;
	}

	/**
	*/
	template <typename STATE, UN_UINT STATE_NUM>
	UN_BOOL CGameStateManagerBase<STATE, STATE_NUM>::Update()
	{		
		if (m_nNextState != STATE_NUM) {
			// ���̃^�X�N�ֈڍs
			if (GetCurrentState() != STATE_NUM) {
				// ���݂̃^�X�N�̌�n��
				if (!GetState(GetCurrentState())->Leave()) {
					UN_ASSERT(UN_FALSE);
					return UN_FALSE;
				}
			}
				
			// ���̃^�X�N�̏�����
			m_nPrevState = GetCurrentState();
			if (! GetState(m_nNextState)->Enter()) {
				// �܂�����Ȃ����ĈӖ��ɂ���
				return UN_TRUE;
			}
		
			SetCurrentState(m_nNextState);
			m_nNextState = (STATE)STATE_NUM;		
		}
		
		UN_BOOL bRet = GetState(GetCurrentState())->Update();
		
		return bRet;
	}

	/**
	*/
	template <typename STATE, UN_UINT STATE_NUM>
	UN_BOOL CGameStateManagerBase<STATE, STATE_NUM>::Render()
	{
		if (GetCurrentState() == STATE_NUM) {
			return UN_TRUE;
		}

		// �`��
		UN_BOOL bRet = GetState(GetCurrentState())->Render();
		
		return bRet;
	}

	/**
	*/
	template <typename STATE, UN_UINT STATE_NUM>
	UN_BOOL CGameStateManagerBase<STATE, STATE_NUM>::Destroy()
	{
		// �˂�̂��� 
		GetState(GetCurrentState())->Leave();

		for (UN_INT i = STATE_NUM - 1; i >= 0; --i) {
			if (!GetState((STATE)i)->Destroy()) {
				UN_ASSERT(UN_FALSE);
				return UN_FALSE;
			}
		}

		return UN_TRUE;
	}

	/**
	*/
	template <typename STATE, UN_UINT STATE_NUM>
	UN_BOOL CGameStateManagerBase<STATE, STATE_NUM>::OnKeyDown(
		UN_UINT nChar, UN_UINT nRepCnt, UN_UINT nFlags)
	{
		return GetState(GetCurrentState())->OnKeyDown(nChar, nRepCnt, nFlags);
	}

	/**
	*/
	template <typename STATE, UN_UINT STATE_NUM>
	UN_BOOL CGameStateManagerBase<STATE, STATE_NUM>::OnMouseMove(
		UN_UINT nFlags, UN_INT x, UN_INT y)
	{
		return GetState(GetCurrentState())->OnMouseMove(nFlags, x, y);
	}

	/**
	*/
	template <typename STATE, UN_UINT STATE_NUM>
	UN_BOOL CGameStateManagerBase<STATE, STATE_NUM>::OnMouseWheel(
		UN_UINT nFlags, UN_SHORT zDelta, UN_INT x, UN_INT y)
	{
		return GetState(GetCurrentState())->OnMouseWheel(nFlags, zDelta, x, y);
	}

	/**
	* �X�e�[�g�ύX
	*/
	template <typename STATE, UN_UINT STATE_NUM>
	void CGameStateManagerBase<STATE, STATE_NUM>::ChangeState(STATE task)
	{
		UN_ASSERT((task >= (STATE)0) && (task < STATE_NUM));
		
		m_nNextState = task;
	}

	/**
	* �O�̃X�e�[�g�ɕύX
	*/
	template <typename STATE, UN_UINT STATE_NUM>
	void CGameStateManagerBase<STATE, STATE_NUM>::ChangePrevState()
	{
		if (m_nPrevState != STATE_NUM) {
			ChangeState(m_nPrevState);
		}
	}

	/**
	* �X�e�[�g�擾
	*/
	template <typename STATE, UN_UINT STATE_NUM>
	CGameState* CGameStateManagerBase<STATE, STATE_NUM>::GetState(STATE task)
	{
		UN_ASSERT((task >= (STATE)0) && (task < STATE_NUM));
		return m_pStateTable[task];
	}
	
	/**
	* ���݂̃X�e�[�g���ǂ���
	*/
	template <typename STATE, UN_UINT STATE_NUM>
	UN_BOOL CGameStateManagerBase<STATE, STATE_NUM>::IsCurrentState(STATE task)
	{
		return (m_nCurrentState == task);
	}

	/**
	* ���݂̃X�e�[�g���ǂ���
	*/
	template <typename STATE, UN_UINT STATE_NUM>
	UN_BOOL CGameStateManagerBase<STATE, STATE_NUM>::IsCurrentState(CGameState* task)
	{
		CGameState* current_task = GetState(m_nCurrentState);
		return (current_task == task);
	}

	/**
	* �X�e�[�g���̎擾
	*/
	template <typename STATE, UN_UINT STATE_NUM>
	UN_UINT CGameStateManagerBase<STATE, STATE_NUM>::GetStateNum()
	{
		return StateNumInternal;
	}
}

#endif	// #if !defined(__URANUS_STD_GAME_STATE_H__)
