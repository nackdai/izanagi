#if !defined(__IZANAGI_STD_GAME_STATE_H__)
#define __IZANAGI_STD_GAME_STATE_H__

#include "izDefs.h"

namespace izanagi {
	////////////////////////////////////////////////////////
	/**
	* ステートベース
	*/
	class CGameState {
	public:
		CGameState() {}
		
		// 呼ばれないので、継承しても実装しないこと！！
		virtual ~CGameState() {}
		
	private:
		CGameState(const CGameState& rhs);
		const CGameState& operator =(const CGameState& rhs);
		
	public:
		virtual IZ_BOOL Create();
		virtual IZ_BOOL Render();
		virtual IZ_BOOL Update();
		virtual IZ_BOOL Destroy();
		virtual IZ_BOOL Enter();
		virtual IZ_BOOL Leave();

#if 1
		// For Windows
		virtual IZ_BOOL OnKeyDown(IZ_UINT nChar, IZ_UINT nRepCnt, IZ_UINT nFlags);
		virtual IZ_BOOL OnMouseMove(IZ_UINT nFlags, IZ_INT x, IZ_INT y);
		virtual IZ_BOOL OnMouseWheel(IZ_UINT nFlags, IZ_SHORT zDelta, IZ_INT x, IZ_INT y);
#endif
	};

	////////////////////////////////////////////////////////
	/**
	* ステート管理
	*/
	template <
		typename STATE,
		IZ_UINT STATE_NUM
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
		IZ_BOOL Create();
		IZ_BOOL Render();
		IZ_BOOL Update();
		IZ_BOOL Destroy();

		// For Windows
		IZ_BOOL OnKeyDown(IZ_UINT nChar, IZ_UINT nRepCnt, IZ_UINT nFlags);
		IZ_BOOL OnMouseMove(IZ_UINT nFlags, IZ_INT x, IZ_INT y);
		IZ_BOOL OnMouseWheel(IZ_UINT nFlags, IZ_SHORT zDelta, IZ_INT x, IZ_INT y);

	public:
		void ChangeState(STATE task);
		void ChangePrevState();
		
		CGameState* GetState(STATE task);
		
		IZ_BOOL IsCurrentState(STATE task);
		IZ_BOOL IsCurrentState(CGameState* task);

		IZ_UINT GetStateNum();
		
	private:
		void SetCurrentState(STATE task) { m_nCurrentState = task; }
			
	public:
		STATE GetCurrentState() { return m_nCurrentState; }
		STATE GetPrevState() { return m_nPrevState; }

	protected:
		CGameState* m_pStateTable[STATE_NUM];
	
		STATE m_nCurrentState;	// 現在のタスク
		STATE m_nNextState;		// 次に遷移するタスク
		STATE m_nPrevState;		// 遷移前のタスク
	};

	// **********************

	/**
	*/
	template <typename STATE, IZ_UINT STATE_NUM>
	IZ_BOOL CGameStateManagerBase<STATE, STATE_NUM>::Create()
	{
		for (IZ_INT i = 0; i < STATE_NUM; ++i) {
			if (!m_pStateTable[i]->Create()) {
				IZ_ASSERT(IZ_FALSE);
				return IZ_FALSE;
			}
		}
		
		// 開始タスクを実行
		ChangeState((STATE)0);
		
		return IZ_TRUE;
	}

	/**
	*/
	template <typename STATE, IZ_UINT STATE_NUM>
	IZ_BOOL CGameStateManagerBase<STATE, STATE_NUM>::Update()
	{		
		if (m_nNextState != STATE_NUM) {
			// 次のタスクへ移行
			if (GetCurrentState() != STATE_NUM) {
				// 現在のタスクの後始末
				if (!GetState(GetCurrentState())->Leave()) {
					IZ_ASSERT(IZ_FALSE);
					return IZ_FALSE;
				}
			}
				
			// 次のタスクの初期化
			m_nPrevState = GetCurrentState();
			if (! GetState(m_nNextState)->Enter()) {
				// まだ入れないって意味にする
				return IZ_TRUE;
			}
		
			SetCurrentState(m_nNextState);
			m_nNextState = (STATE)STATE_NUM;		
		}
		
		IZ_BOOL bRet = GetState(GetCurrentState())->Update();
		
		return bRet;
	}

	/**
	*/
	template <typename STATE, IZ_UINT STATE_NUM>
	IZ_BOOL CGameStateManagerBase<STATE, STATE_NUM>::Render()
	{
		if (GetCurrentState() == STATE_NUM) {
			return IZ_TRUE;
		}

		// 描画
		IZ_BOOL bRet = GetState(GetCurrentState())->Render();
		
		return bRet;
	}

	/**
	*/
	template <typename STATE, IZ_UINT STATE_NUM>
	IZ_BOOL CGameStateManagerBase<STATE, STATE_NUM>::Destroy()
	{
		// ねんのため 
		GetState(GetCurrentState())->Leave();

		for (IZ_INT i = STATE_NUM - 1; i >= 0; --i) {
			if (!GetState((STATE)i)->Destroy()) {
				IZ_ASSERT(IZ_FALSE);
				return IZ_FALSE;
			}
		}

		return IZ_TRUE;
	}

	/**
	*/
	template <typename STATE, IZ_UINT STATE_NUM>
	IZ_BOOL CGameStateManagerBase<STATE, STATE_NUM>::OnKeyDown(
		IZ_UINT nChar, IZ_UINT nRepCnt, IZ_UINT nFlags)
	{
		return GetState(GetCurrentState())->OnKeyDown(nChar, nRepCnt, nFlags);
	}

	/**
	*/
	template <typename STATE, IZ_UINT STATE_NUM>
	IZ_BOOL CGameStateManagerBase<STATE, STATE_NUM>::OnMouseMove(
		IZ_UINT nFlags, IZ_INT x, IZ_INT y)
	{
		return GetState(GetCurrentState())->OnMouseMove(nFlags, x, y);
	}

	/**
	*/
	template <typename STATE, IZ_UINT STATE_NUM>
	IZ_BOOL CGameStateManagerBase<STATE, STATE_NUM>::OnMouseWheel(
		IZ_UINT nFlags, IZ_SHORT zDelta, IZ_INT x, IZ_INT y)
	{
		return GetState(GetCurrentState())->OnMouseWheel(nFlags, zDelta, x, y);
	}

	/**
	* ステート変更
	*/
	template <typename STATE, IZ_UINT STATE_NUM>
	void CGameStateManagerBase<STATE, STATE_NUM>::ChangeState(STATE task)
	{
		IZ_ASSERT((task >= (STATE)0) && (task < STATE_NUM));
		
		m_nNextState = task;
	}

	/**
	* 前のステートに変更
	*/
	template <typename STATE, IZ_UINT STATE_NUM>
	void CGameStateManagerBase<STATE, STATE_NUM>::ChangePrevState()
	{
		if (m_nPrevState != STATE_NUM) {
			ChangeState(m_nPrevState);
		}
	}

	/**
	* ステート取得
	*/
	template <typename STATE, IZ_UINT STATE_NUM>
	CGameState* CGameStateManagerBase<STATE, STATE_NUM>::GetState(STATE task)
	{
		IZ_ASSERT((task >= (STATE)0) && (task < STATE_NUM));
		return m_pStateTable[task];
	}
	
	/**
	* 現在のステートかどうか
	*/
	template <typename STATE, IZ_UINT STATE_NUM>
	IZ_BOOL CGameStateManagerBase<STATE, STATE_NUM>::IsCurrentState(STATE task)
	{
		return (m_nCurrentState == task);
	}

	/**
	* 現在のステートかどうか
	*/
	template <typename STATE, IZ_UINT STATE_NUM>
	IZ_BOOL CGameStateManagerBase<STATE, STATE_NUM>::IsCurrentState(CGameState* task)
	{
		CGameState* current_task = GetState(m_nCurrentState);
		return (current_task == task);
	}

	/**
	* ステート数の取得
	*/
	template <typename STATE, IZ_UINT STATE_NUM>
	IZ_UINT CGameStateManagerBase<STATE, STATE_NUM>::GetStateNum()
	{
		return StateNumInternal;
	}
}

#endif	// #if !defined(__IZANAGI_STD_GAME_STATE_H__)
