#if !defined(__STATE_MANAGER_H__)
#define __STATE_MANAGER_H__

#include "izSceneGraph.h"

namespace izanagi {
    namespace sample {
        class CSampleApp;
    }
}

enum State {
    State_MirrorMap = 0,
    State_PhongShader,
    State_PointLight,
    State_SpotLight,
    State_NormalMap,
    State_SSAO,
    State_BevelShader,

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
    void Create(
        izanagi::sample::CSampleApp* app,
        izanagi::SCameraParam& camera);

    // �X�e�[�g�ύX.
    void ChangeState(IZ_INT task)
    {
        if (task < 0) {
            task = StateNum - 1;
        }
        else if (task >= StateNum) {
            task = 0;
        }
        
        izanagi::CFixedSceneStateManager<State, StateNum>::ChangeState(static_cast<State>(task));
    }
};

#endif    // #if !defined(__STATE_MANAGER_H__)
