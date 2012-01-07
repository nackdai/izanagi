#if !defined(__STATE_MANAGER_H__)
#define __STATE_MANAGER_H__

#include "izSceneGraph.h"

enum State {
	State_MirrorMap = 0,

	StateNum,
};

class CStateManager : public izanagi::CFixedSceneStateManager<State, StateNum> {
private:
	static CStateManager s_Instance;

public:
	static CStateManager& GetInstance() { return s_Instance; }

public:
	CStateManager();
	virtual ~CStateManager() {}

public:
	void Create(izanagi::SCameraParam& camera);
};

#endif	// #if !defined(__STATE_MANAGER_H__)
