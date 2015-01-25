#include "StateRotateByFling.h"
#include "ItemManager.h"
#include "GestureListener.h"
#include "StateManager.h"

StateRotateByFling::StateRotateByFling(izanagi::CVectorCamera& camera)
    : StateBase(camera)
{
}

StateRotateByFling::~StateRotateByFling()
{
}

IZ_BOOL StateRotateByFling::Update(
    IZ_FLOAT time,
    izanagi::graph::CGraphicsDevice* device)
{
    GestureDetector::Instance().Update();

    if (!ItemManager::Instance().IsRotateAnimating()) {
        StateManager::Instance().ChangeState(State_Default);
    }

    return IZ_TRUE;
}

IZ_BOOL StateRotateByFling::Leave()
{
    // Stop rotateion animation.
    ItemManager::Instance().SetAngleRate(0.0f);
    return IZ_TRUE;
}
