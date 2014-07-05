#if !defined(__STATE_MOVE_TO_ITEM_H__)
#define __STATE_MOVE_TO_ITEM_H__

#include "izSceneGraph.h"
#include "StateBase.h"
#include "izAnimation.h"

class StateMoveToItem : public StateBase {
public:
    StateMoveToItem(izanagi::CVectorCamera& camera);
    virtual ~StateMoveToItem();

public:
    virtual IZ_BOOL Enter(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        izanagi::CValue& arg);

    virtual IZ_BOOL Update(
        IZ_FLOAT time,
        izanagi::graph::CGraphicsDevice* device);

    virtual IZ_BOOL OnMouseLBtnDown(const izanagi::CIntPoint& point);
    virtual IZ_BOOL OnMouseLBtnUp(const izanagi::CIntPoint& point);

private:
    enum State {
        State_None,
        State_Move,
        State_Moved,
        State_Return,
    };

private:
    State m_State;

    izanagi::math::SMatrix m_CamMtx;
    izanagi::math::SMatrix m_TargetMtx;

    izanagi::animation::CTimeline m_Timeline;
};

#endif    // #if !defined(__STATE_MOVE_TO_ITEM_H__)
