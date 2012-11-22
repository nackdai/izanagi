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

IZ_BOOL CSceneStateBase::Update()
{
	return IZ_TRUE;
}

IZ_BOOL CSceneStateBase::Destroy()
{
	return IZ_TRUE;
}

IZ_BOOL CSceneStateBase::Enter(
	IMemoryAllocator* allocator,
	void* val/* = IZ_NULL*/)
{
	return IZ_TRUE;
}

IZ_BOOL CSceneStateBase::Leave()
{
	return IZ_TRUE;
}

IZ_BOOL CSceneStateBase::OnKeyDown(IZ_UINT nChar)
{
	return IZ_TRUE;
}

IZ_BOOL CSceneStateBase::OnMouseMove(IZ_INT x, IZ_INT y)
{
	return IZ_TRUE;
}

IZ_BOOL CSceneStateBase::OnMouseWheel(IZ_SHORT delta)
{
	return IZ_TRUE;
}
