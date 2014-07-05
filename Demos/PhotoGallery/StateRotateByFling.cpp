#include "StateRotateByFling.h"
#include "PhotoItemManager.h"
#include "GestureListener.h"
#include "StateManager.h"

StateRotateByFling::StateRotateByFling(izanagi::CVectorCamera& camera)
    : StateBase(camera)
{
}

StateRotateByFling::~StateRotateByFling()
{
}

IZ_BOOL StateRotateByFling::Update()
{
    GestureDetector::Instance().Update();
    PhotoItemManager::Instance().Update();

    if (!PhotoItemManager::Instance().IsRotateAnimating()) {
        StateManager::Instance().ChangeState(State_Default);
    }

    return IZ_TRUE;
}

IZ_BOOL StateRotateByFling::Leave()
{
    // Stop rotateion animation.
    PhotoItemManager::Instance().SetAngleRate(0.0f);
    return IZ_TRUE;
}
