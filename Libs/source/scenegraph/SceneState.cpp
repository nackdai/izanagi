#include "scenegraph/SceneState.h"
#include "izGraph.h"

using namespace izanagi;

IZ_BOOL CSceneStateBase::Init()
{
	return IZ_TRUE;
}

IZ_BOOL CSceneStateBase::Render(CGraphicsDevice* device)
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

IZ_BOOL CSceneStateBase::OnKeyDown(IZ_UINT nChar, IZ_UINT nRepCnt, IZ_UINT nFlags)
{
	return IZ_TRUE;
}

IZ_BOOL CSceneStateBase::OnMouseMove(IZ_UINT nFlags, IZ_INT x, IZ_INT y)
{
	return IZ_TRUE;
}

IZ_BOOL CSceneStateBase::OnMouseWheel(IZ_UINT nFlags, IZ_SHORT zDelta, IZ_INT x, IZ_INT y)
{
	return IZ_TRUE;
}
