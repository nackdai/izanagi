#if !defined(__STATE_MANAGER_H__)
#define __STATE_MANAGER_H__

#include "izSceneGraph.h"

enum State {
    State_Default,
    State_RotateByFling,
    State_ChangeView,

    StateNum,
};

class StateManager : public izanagi::CFixedSceneStateManager<State, StateNum> {
private:
    static StateManager s_Instance;

public:
    static StateManager& Instance();

private:
    StateManager();
    virtual ~StateManager() {}

public:
    void Create(izanagi::CCamera& camera);
};

#endif    // #if !defined(__STATE_MANAGER_H__)
