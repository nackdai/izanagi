#include "StateRotateByFling.h"
#include "PhotoItemManager.h"

StateRotateByFling::StateRotateByFling(izanagi::CCamera& camera)
    : StateBase(camera)
{
}

StateRotateByFling::~StateRotateByFling()
{
}

IZ_BOOL StateRotateByFling::Update()
{
    PhotoItemManager::Instance().Update();
    return IZ_TRUE;
}

IZ_BOOL StateRotateByFling::Leave()
{
    // Stop rotateion animation.
    PhotoItemManager::Instance().SetAngleRate(0.0f);
    return IZ_TRUE;
}
