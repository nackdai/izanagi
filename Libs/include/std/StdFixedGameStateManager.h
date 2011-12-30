#if !defined(__IZANAGI_STD_FIXED_GAME_STATE_MANAGER_H__)
#define __IZANAGI_STD_FIXED_GAME_STATE_MANAGER_H__

#include "izDefs.h"
#include "StdGameState.h"

namespace izanagi {
	/** 固定ステート管理.
	 */
	template <
		typename STATE,
		IZ_UINT STATE_NUM
	>
	class CFixedGameStateManagerBase {
	private:
		enum {
			StateNumInternal = STATE_NUM,
		};

	public:
		CFixedGameStateManagerBase()
		{
			FILL_ZERO(m_pStateTable, sizeof(m_pStateTable));
		}

		~CFixedGameStateManagerBase() {}

		NO_COPIABLE(CFixedGameStateManagerBase);

	public:
		/** 登録
		 */
		IZ_BOOL Register(IZ_UINT idx, CGameStateBase* state)
		{
			VRETURN(
				idx < StateNumInternal
				&& m_pStateTable[idx] == IZ_NULL);

			m_pStateTable[idx] = state;
		}

		/** 初期化.
		 */
		IZ_BOOL Init()
		{
			for (IZ_INT i = 0; i < STATE_NUM; ++i) {
				if (!m_pStateTable[i]->Init()) {
					IZ_ASSERT(IZ_FALSE);
					return IZ_FALSE;
				}
			}
			
			// 開始タスクを実行
			ChangeState((STATE)0);
			
			return IZ_TRUE;
		}

		/** 描画.
		 */
		IZ_BOOL Render()
		{
			if (GetCurrentState() == STATE_NUM) {
				return IZ_TRUE;
			}

			// 描画
			IZ_BOOL bRet = GetState(GetCurrentState())->Render();
			
			return bRet;
		}

		/** 更新.
		 */
		IZ_BOOL Update()
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

		/** 終了.
		 */
		IZ_BOOL Destroy()
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

		// For Windows

		/** キー押下.
		 */
		IZ_BOOL OnKeyDown(IZ_UINT nChar, IZ_UINT nRepCnt, IZ_UINT nFlags)
		{
			return GetState(GetCurrentState())->OnKeyDown(nChar, nRepCnt, nFlags);
		}

		/** マウス移動.
		 */
		IZ_BOOL OnMouseMove(IZ_UINT nFlags, IZ_INT x, IZ_INT y)
		{
			return GetState(GetCurrentState())->OnMouseMove(nFlags, x, y);
		}

		/** マウスホイール操作.
		 */
		IZ_BOOL OnMouseWheel(IZ_UINT nFlags, IZ_SHORT zDelta, IZ_INT x, IZ_INT y)
		{
			return GetState(GetCurrentState())->OnMouseWheel(nFlags, zDelta, x, y);
		}

	public:
		/** ステート変更.
		 */
		void ChangeState(STATE task)
		{
			IZ_ASSERT((task >= (STATE)0) && (task < STATE_NUM));
			
			m_nNextState = task;
		}

		/** 前のステートに変更.
		 */
		void ChangePrevState();
		{
			if (m_nPrevState != STATE_NUM) {
				ChangeState(m_nPrevState);
			}
		}

		/** ステート取得.
		 */
		CGameStateBase* GetState(STATE task)
		{
			IZ_ASSERT((task >= (STATE)0) && (task < STATE_NUM));
			return m_pStateTable[task];
		}
		
		/** 現在のステートかどうか.
		 */ 
		IZ_BOOL IsCurrentState(STATE task)
		{
			return (m_nCurrentState == task);
		}

		/** 現在のステートかどうか.
		 */ 
		IZ_BOOL IsCurrentState(CGameStateBase* task)
		{
			CGameStateBase* current_task = GetState(m_nCurrentState);
			return (current_task == task);
		}

		/** ステート数を取得.
		 */
		IZ_UINT GetStateNum()
		{
			return StateNumInternal;
		}

		/** 現在のステートを取得.
		 */
		STATE GetCurrentState()
		{
			return m_nCurrentState;
		}

		/** 前のステートを取得
		 */
		STATE GetPrevState()
		{
			return m_nPrevState;
		}
		
	private:
		void SetCurrentState(STATE task) { m_nCurrentState = task; }

	protected:
		CGameStateBase* m_pStateTable[STATE_NUM];
	
		STATE m_nCurrentState;	// 現在のタスク
		STATE m_nNextState;		// 次に遷移するタスク
		STATE m_nPrevState;		// 遷移前のタスク
	};
}

#endif	// #if !defined(__IZANAGI_STD_FIXED_GAME_STATE_MANAGER_H__)
