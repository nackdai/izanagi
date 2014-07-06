#include "scenegraph/SceneState.h"
#include "izGraph.h"

using namespace izanagi;

IZ_BOOL CSceneStateBase::Init()
{
    return IZ_TRUE;
}

IZ_BOOL CSceneStateBase::Render(graph::CGraphicsDevice* device)
{
    return IZ_TRUE;
}

IZ_BOOL CSceneStateBase::Update(
    IZ_FLOAT time,
    graph::CGraphicsDevice* device)
{
    return IZ_TRUE;
}

IZ_BOOL CSceneStateBase::Destroy()
{
    return IZ_TRUE;
}

IZ_BOOL CSceneStateBase::Enter(
    IMemoryAllocator* allocator,
    graph::CGraphicsDevice* device,
    CValue& value)
{
    return IZ_TRUE;
}

IZ_BOOL CSceneStateBase::Leave()
{
    return IZ_TRUE;
}

IZ_BOOL CSceneStateBase::OnKeyDown(sys::E_KEYBOARD_BUTTON key)
{
    return IZ_TRUE;
}

IZ_BOOL CSceneStateBase::OnMouseLBtnDown(const izanagi::CIntPoint& point)
{
    return IZ_TRUE;
}

IZ_BOOL CSceneStateBase::OnMouseLBtnUp(const izanagi::CIntPoint& point)
{
    return IZ_TRUE;
}

void CSceneStateBase::OnMouseMove(const izanagi::CIntPoint& point)
{
}

IZ_BOOL CSceneStateBase::OnMouseWheel(IZ_SHORT delta)
{
    return IZ_TRUE;
}
