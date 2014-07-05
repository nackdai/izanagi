#if !defined(__STATE_DEFAULT_H__)
#define __STATE_DEFAULT_H__

#include "izSceneGraph.h"
#include "StateBase.h"

class StateDefault : public StateBase {
public:
    StateDefault(izanagi::CVectorCamera& camera);
    virtual ~StateDefault();

public:
    virtual IZ_BOOL Update();

    virtual IZ_BOOL OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key);
};

#endif    // #if !defined(__STATE_DEFAULT_H__)
