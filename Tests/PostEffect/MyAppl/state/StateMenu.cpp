#include "StateMenu.h"
#include "StateManager.h"
#include "MySystem.h"

CStateMenu::CStateMenu()
{
	m_nPos = 0;
}

CStateMenu::~CStateMenu()
{
}

IZ_BOOL CStateMenu::Init()
{
	return IZ_TRUE;
}

IZ_BOOL CStateMenu::Render(izanagi::CGraphicsDevice* device)
{
	Render2D();

	return IZ_TRUE;
}

void CStateMenu::Render2D()
{
	static const D3DCOLOR color = D3DCOLOR_RGBA(255, 255, 255, 255);
	static const IZ_INT nPosX = 80;
	static const IZ_INT nPosY = 80;
	const IZ_INT nFontSize = CMySystem::GetInstance().GetDebugFont()->GetFontSize();

	static const char* MENU_NAME[] = {
		"Monotone",
		"BloomStar",
		"BloomStarEx",
		"HDR",
	};

	if (CMySystem::GetInstance().GetGraphicsDevice()->Begin2D()) {
		CMySystem::GetInstance().GetDebugFont()->Begin();

		CMySystem::GetInstance().GetDebugFont()->DBPrint(
			0, 0,
			color,
			"MENU\n");
		CMySystem::GetInstance().GetDebugFont()->DBPrint(
			color,
			"Up,Down Cursor : Select\n");
		CMySystem::GetInstance().GetDebugFont()->DBPrint(
			color,
			"Enter : Decision\n");

		CMySystem::GetInstance().GetDebugFont()->DBPrint(
			nPosX - nFontSize,
			nPosY + nFontSize * m_nPos,
			color,
			">");

		for (IZ_INT i = 0; i < COUNTOF(MENU_NAME); i++) {
			CMySystem::GetInstance().GetDebugFont()->DBPrint(
				nPosX,
				nPosY + nFontSize * i,
				color,
				MENU_NAME[i]);
		}

		CMySystem::GetInstance().GetDebugFont()->End();
		CMySystem::GetInstance().GetGraphicsDevice()->End2D();
	}
}

IZ_BOOL CStateMenu::Update()
{
	if (m_bDecide) {
		IZ_UINT pos = m_nPos + 1;

		switch (pos) {
		case STATE_MONOTONE:		CHANGE_STATE(MONOTONE);		break;
		case STATE_BLOOMSTAR:		CHANGE_STATE(BLOOMSTAR);	break;
		case STATE_BLOOMSTAR_EX:	CHANGE_STATE(BLOOMSTAR_EX);	break;
		case STATE_HDR:				CHANGE_STATE(HDR);			break;
		default:					IZ_ASSERT(IZ_FALSE);		break;
		}
	}

	return IZ_TRUE;
}

IZ_BOOL CStateMenu::Destroy()
{
	return IZ_TRUE;
}

IZ_BOOL CStateMenu::Enter(izanagi::IMemoryAllocator* allocator, void* val)
{
	m_bDecide = IZ_FALSE;

	//CPostEffectManager::GetInstance().ClearFileName();

	return IZ_TRUE;
}

IZ_BOOL CStateMenu::Leave()
{
	return IZ_TRUE;
}

IZ_BOOL CStateMenu::OnKeyDown(UINT nChar)
{
	if (nChar == VK_UP) {
		m_nPos--;
	}
	else if (nChar == VK_DOWN) {
		m_nPos++;
	}

	if (nChar == VK_RETURN) {
		m_bDecide = IZ_TRUE;
	}

	if (m_nPos < 0) {
		m_nPos = CStateManager::GetInstance().GetStateNum() - 2;
	}
	else if (m_nPos >= CStateManager::GetInstance().GetStateNum() - 1) {
		m_nPos = 0;
	}

	return IZ_TRUE;
}
