#if !defined(__STATE_CHANGE_VIEW_H__)
#define __STATE_CHANGE_VIEW_H__

#include "izSceneGraph.h"
#include "StateBase.h"
#include "izAnimation.h"

class StateChangeView : public StateBase {
public:
    enum ChangeViewMode {
        ChangeViewModeNone,
        ChangeViewModeUp,
        ChangeViewModeDown,
    };

public:
    StateChangeView(izanagi::CVectorCamera& camera);
    virtual ~StateChangeView();

public:
    virtual IZ_BOOL Enter(
        izanagi::IMemoryAllocator* allocator,
        izanagi::CValue& arg);

    virtual IZ_BOOL Update();

    virtual IZ_BOOL OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key);

private:
    enum ViewState {
        ViewState_Low,
        ViewState_Mid,
        ViewState_High,

        ViewState_Num,
    };

    static const IZ_FLOAT ViewRotAngle[ViewState_Num];

    ViewState GetNextState(IZ_BOOL isUp);

private:
    ViewState m_State;
    ViewState m_NextState;

    IZ_BOOL m_IsAnimating;

    izanagi::math::SMatrix m_ViewMtx[ViewState_Num];

    izanagi::sys::CTimer m_Timer;
    izanagi::animation::CTimeline m_Timeline;
};

#endif    // #if !defined(__STATE_CHANGE_VIEW_H__)
