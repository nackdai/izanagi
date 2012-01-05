#if !defined(__STATE_MANAGER_H__)
#define __STATE_MANAGER_H__

#include "izDefs.h"
#include "izStd.h"
#include "izSceneGraph.h"

enum STATE {
	STATE_TEST0,
	STATE_TEST1,

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

#endif	// #if !defined(__STATE_MANAGER_H__)
