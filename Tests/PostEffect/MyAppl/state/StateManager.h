#if !defined(__STATE_MANAGER_H__)
#define __STATE_MANAGER_H__

#include "izDefs.h"
#include "izStd.h"

enum STATE {
	STATE_MENU = 0,
	STATE_MONOTONE,
	STATE_BLOOMSTAR,
	STATE_BLOOMSTAR_EX,
	STATE_HDR,
	STATE_NUM,
};

class CStateManager : public izanagi::CGameStateManagerBase<STATE, STATE_NUM> {
private:
	static CStateManager m_cInstance;

public:
	static CStateManager& GetInstance() { return m_cInstance; }

public:
	inline IZ_BOOL OnKeyDown(IZ_UINT nChar, IZ_UINT nRepCnt, IZ_UINT nFlags);

private:
	CStateManager();
	~CStateManager() {}
};

// inline **********************************

IZ_BOOL CStateManager::OnKeyDown(IZ_UINT nChar, IZ_UINT nRepCnt, IZ_UINT nFlags)
{
	return GetState(GetCurrentState())->OnKeyDown(nChar, nRepCnt, nFlags);
}

// いちいち面倒だろうから
#define CHANGE_STATE(task) \
	CStateManager::GetInstance().ChangeState(STATE_##task)

#define GET_STATE(task) \
	CStateManager::GetInstance().GetState(STATE_##task)


#endif	// #if !defined(__STATE_MANAGER_H__)
