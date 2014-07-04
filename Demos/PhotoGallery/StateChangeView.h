#if !defined(__STATE_CHANGE_VIEW_H__)
#define __STATE_CHANGE_VIEW_H__

#include "izSceneGraph.h"
#include "StateBase.h"

class StateChangeView : public StateBase {
public:
    StateChangeView(izanagi::CCamera& camera);
    virtual ~StateChangeView();

public:
    virtual IZ_BOOL Update();

    virtual IZ_BOOL OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key);
};

#endif    // #if !defined(__STATE_CHANGE_VIEW_H__)
