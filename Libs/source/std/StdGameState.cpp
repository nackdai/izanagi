#include "std/StdGameState.h"

using namespace uranus;

UN_BOOL CGameState::Create()
{
	return UN_TRUE;
}

UN_BOOL CGameState::Render()
{
	return UN_TRUE;
}

UN_BOOL CGameState::Update()
{
	return UN_TRUE;
}

UN_BOOL CGameState::Destroy()
{
	return UN_TRUE;
}

UN_BOOL CGameState::Enter()
{
	return UN_TRUE;
}

UN_BOOL CGameState::Leave()
{
	return UN_TRUE;
}

UN_BOOL CGameState::OnKeyDown(UN_UINT nChar, UN_UINT nRepCnt, UN_UINT nFlags)
{
	return UN_TRUE;
}

UN_BOOL CGameState::OnMouseMove(UN_UINT nFlags, UN_INT x, UN_INT y)
{
	return UN_TRUE;
}

UN_BOOL CGameState::OnMouseWheel(UN_UINT nFlags, UN_SHORT zDelta, UN_INT x, UN_INT y)
{
	return UN_TRUE;
}
