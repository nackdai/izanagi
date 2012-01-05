#if !defined(__STATE_MANAGER_H__)
#define __STATE_MANAGER_H__

#include "izDefs.h"
#include "izStd.h"
#include "izSceneGraph.h"

enum STATE {
	STATE_MENU = 0,
	STATE_ANM_LIST,
	STATE_ANM_INTERP,
	STATE_ANM_BLEND,

	STATE_NUM,
};

class CStateManager : public izanagi::CFixedSceneStateManager<STATE, STATE_NUM> {
private:
	static CStateManager s_cInstance;

public:
	static CStateManager& GetInstance() { return s_cInstance; }

private:
	CStateManager();
	~CStateManager() {}
};

// いちいち面倒だろうから
#define CHANGE_STATE(task) \
	CStateManager::GetInstance().ChangeState(STATE_##task)

#define GET_STATE(task) \
	CStateManager::GetInstance().GetState(STATE_##task)

#endif	// #if !defined(__STATE_MANAGER_H__)
