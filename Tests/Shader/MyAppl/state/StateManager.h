#if !defined(__STATE_MANAGER_H__)
#define __STATE_MANAGER_H__

#include "izDefs.h"
#include "izStd.h"
#include "izSceneGraph.h"

enum STATE {
	STATE_CONST_COLOR = 0,
	STATE_NUM,
};

class CStateManager : public izanagi::CFixedSceneStateManager<STATE, STATE_NUM> {
private:
	static CStateManager m_cInstance;

public:
	static CStateManager& GetInstance() { return m_cInstance; }

public:
	inline IZ_BOOL OnKeyDown(IZ_UINT nChar);

private:
	CStateManager();
	~CStateManager() {}
};

// inline **********************************

IZ_BOOL CStateManager::OnKeyDown(IZ_UINT nChar)
{
	return GetState(GetCurrentState())->OnKeyDown(nChar);
}

// いちいち面倒だろうから
#define CHANGE_STATE(task) \
	CStateManager::GetInstance().ChangeState(STATE_##task)

#define GET_STATE(task) \
	CStateManager::GetInstance().GetState(STATE_##task)


#endif	// #if !defined(__STATE_MANAGER_H__)
