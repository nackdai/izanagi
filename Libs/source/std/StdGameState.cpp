#include "std/StdGameState.h"

using namespace izanagi;

IZ_BOOL CGameStateBase::Init()
{
	return IZ_TRUE;
}

IZ_BOOL CGameStateBase::Render()
{
	return IZ_TRUE;
}

IZ_BOOL CGameStateBase::Update()
{
	return IZ_TRUE;
}

IZ_BOOL CGameStateBase::Destroy()
{
	return IZ_TRUE;
}

IZ_BOOL CGameStateBase::Enter()
{
	return IZ_TRUE;
}

IZ_BOOL CGameStateBase::Leave()
{
	return IZ_TRUE;
}

IZ_BOOL CGameStateBase::OnKeyDown(IZ_UINT nChar, IZ_UINT nRepCnt, IZ_UINT nFlags)
{
	return IZ_TRUE;
}

IZ_BOOL CGameStateBase::OnMouseMove(IZ_UINT nFlags, IZ_INT x, IZ_INT y)
{
	return IZ_TRUE;
}

IZ_BOOL CGameStateBase::OnMouseWheel(IZ_UINT nFlags, IZ_SHORT zDelta, IZ_INT x, IZ_INT y)
{
	return IZ_TRUE;
}
