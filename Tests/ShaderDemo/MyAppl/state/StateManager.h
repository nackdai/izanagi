#if !defined(__STATE_MANAGER_H__)
#define __STATE_MANAGER_H__

#include "izDefs.h"
#include "izStd.h"

enum STATE {
	STATE_BASIC = 0,
	STATE_PHONG,
	STATE_HEMISPHERE,
	STATE_NORMAL_MAP,
	STATE_PARALLAX,
	STATE_RIM_LIGHT,

	STATE_NUM,
};

namespace izanagi {
}	// namespace izanagi

class CStateManager : public izanagi::CGameStateManagerBase<STATE, STATE_NUM> {
private:
	static CStateManager s_cInstance;

public:
	static CStateManager& GetInstance() { return s_cInstance; }

public:
	IZ_BOOL OnKeyDown(IZ_UINT nChar, IZ_UINT nRepCnt, IZ_UINT nFlags);

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
