#include "StateBase.h"
#include "PhotoItemManager.h"

StateBase::StateBase(izanagi::CCamera& camera)
    : m_Camera(camera)
{
}

IZ_BOOL StateBase::Render(izanagi::graph::CGraphicsDevice* device)
{
    PhotoItemManager::Instance().Render(
        device,
        m_Camera);

    return IZ_TRUE;
}
