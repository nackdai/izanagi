#include "std/StdGameState.h"

using namespace izanagi;

IZ_BOOL CGameState::Create()
{
	return IZ_TRUE;
}

IZ_BOOL CGameState::Render()
{
	return IZ_TRUE;
}

IZ_BOOL CGameState::Update()
{
	return IZ_TRUE;
}

IZ_BOOL CGameState::Destroy()
{
	return IZ_TRUE;
}

IZ_BOOL CGameState::Enter()
{
	return IZ_TRUE;
}

IZ_BOOL CGameState::Leave()
{
	return IZ_TRUE;
}

IZ_BOOL CGameState::OnKeyDown(IZ_UINT nChar, IZ_UINT nRepCnt, IZ_UINT nFlags)
{
	return IZ_TRUE;
}

IZ_BOOL CGameState::OnMouseMove(IZ_UINT nFlags, IZ_INT x, IZ_INT y)
{
	return IZ_TRUE;
}

IZ_BOOL CGameState::OnMouseWheel(IZ_UINT nFlags, IZ_SHORT zDelta, IZ_INT x, IZ_INT y)
{
	return IZ_TRUE;
}
