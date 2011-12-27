#include "SampleWindowProc.h"
#include "SampleApp.h"

using namespace izanagi;
using namespace sample;

CSampleWndProc::CSampleWndProc() 
{
	m_App = IZ_NULL;

	m_Flags.onLBtn = IZ_FALSE;
	m_Flags.onRBtn = IZ_FALSE;
}

CSampleWndProc::~CSampleWndProc()
{
}

void CSampleWndProc::Init(CSampleApp* app)
{
	m_App = app;
}

void CSampleWndProc::OnKeyDown(IZ_UINT nChar)
{
	//CStateManager::GetInstance().OnKeyDown(nChar, 0, 0);
}

void CSampleWndProc::OnMouseLBtnDown()
{
	m_Flags.onLBtn = IZ_TRUE;
}

void CSampleWndProc::OnMouseLBtnUp()
{
	m_Flags.onLBtn = IZ_FALSE;
}

void CSampleWndProc::OnMouseRBtnDown()
{
	m_Flags.onRBtn = IZ_TRUE;
}

void CSampleWndProc::OnMouseRBtnUp()
{
	m_Flags.onRBtn = IZ_FALSE;
}

void CSampleWndProc::OnMouseMove(const izanagi::CIntPoint& point)
{
	IZ_ASSERT(m_App != IZ_NULL);

	if (m_Flags.onLBtn) {
		float fLatitude = (float)(m_PrevPoint.y - point.y);
		fLatitude *= 0.005f;

		float fLongitude = (float)(m_PrevPoint.x - point.x);
		fLongitude *= 0.005f;

		m_App->GetCamera().Rotate(fLatitude, fLongitude);
	}
	else if (m_Flags.onRBtn) {
		float fOffsetX = (float)(m_PrevPoint.x - point.x);
		fOffsetX *= 0.5f;

		float fOffsetY = (float)(m_PrevPoint.y - point.y);
		fOffsetY *= 0.5f;

		m_App->GetCamera().Move(fOffsetX, fOffsetY);
	}

	m_PrevPoint = point;
}

void CSampleWndProc::OnMouseWheel(IZ_INT delta)
{
	m_App->GetCamera().Dolly(delta * 0.1f);
}

void CSampleWndProc::OnPaint()
{
	IZ_ASSERT(m_App != IZ_NULL);

	m_App->Render();
	m_App->Present();
}

void CSampleWndProc::OnIdle()
{
	IZ_ASSERT(m_App != IZ_NULL);

	m_App->GetTimer(0).Begin();
	m_App->GetTimer(1).Begin();

	m_App->Update();
	m_App->Render();

	m_App->GetTimer(1).End();

	m_App->Present();

	m_App->GetTimer(0).End();
}
