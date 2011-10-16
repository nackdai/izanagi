#include <windows.h>

#include "izStd.h"
#include "izSystem.h"

#include "MyAppl.h"
#include "MySystem.h"
#include "MyCamera.h"
#include "state/StateManager.h"

izanagi::CStandardMemoryAllocator sAllocator;

IZ_UINT8 sBuf[1 * 1024 * 1024];

class CWndProc : public izanagi::CMessageHandler {
public:
	CWndProc() 
	{
		m_Flags.onLBtn = IZ_FALSE;
		m_Flags.onRBtn = IZ_FALSE;
	}
	~CWndProc() {}

public:
	void OnKeyDown(IZ_UINT nChar)
	{
		CStateManager::GetInstance().GetState(STATE_DEBUG_MESH)->OnKeyDown(nChar, 0, 0);
	}

	void OnMouseLBtnDown()
	{
		m_Flags.onLBtn = IZ_TRUE;
	}

	void OnMouseLBtnUp()
	{
		m_Flags.onLBtn = IZ_FALSE;
	}

	void OnMouseRBtnDown()
	{
		m_Flags.onRBtn = IZ_TRUE;
	}

	void OnMouseRBtnUp()
	{
		m_Flags.onRBtn = IZ_FALSE;
	}

	void OnMouseMove(const izanagi::CIntPoint& point)
	{
		if (m_Flags.onLBtn) {
			float fLatitude = (float)(m_ptPrevPoint.y - point.y);
			fLatitude *= 0.005f;

			float fLongitude = (float)(m_ptPrevPoint.x - point.x);
			fLongitude *= 0.005f;

			CMyCamera::GetInstance().Rotate(fLatitude, fLongitude);
		}
		else if (m_Flags.onRBtn) {
			float fOffsetX = (float)(m_ptPrevPoint.x - point.x);
			fOffsetX *= 0.5f;

			float fOffsetY = (float)(m_ptPrevPoint.y - point.y);
			fOffsetY *= 0.5f;

			CMyCamera::GetInstance().Move(fOffsetX, fOffsetY);
		}

		m_ptPrevPoint = point;
	}

	void OnMouseWheel(IZ_INT delta)
	{
		CMyCamera::GetInstance().Dolly(delta * 0.1f);
	}

	void OnPaint()
	{
		CMyAppl::GetInstance().Render();
		CMySystem::GetInstance().GetGraphicsDevice()->Present();
	}

	void OnIdle()
	{
		CMySystem::GetInstance().GetTimer(0).Begin();
		CMySystem::GetInstance().GetTimer(1).Begin();

		CMyAppl::GetInstance().Update();
		CMyAppl::GetInstance().Render();

		CMySystem::GetInstance().GetTimer(1).End();

		CMySystem::GetInstance().GetGraphicsDevice()->Present();

		CMySystem::GetInstance().GetTimer(0).End();
	}

private:
	izanagi::CIntPoint m_ptPrevPoint;
	struct {
		IZ_UINT onLBtn	: 1;
		IZ_UINT onRBtn	: 1;
	} m_Flags;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	sAllocator.Init(sizeof(sBuf), sBuf);

	CWndProc wndProc;

	izanagi::WindowParams wndParam = {
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		TRUE,
		"WindowTest",
		hInstance,
		&wndProc,
	};

	izanagi::WindowHandle wndHandle = izanagi::CSysWindow::Create(&sAllocator, wndParam);

	HWND hWnd = (HWND)izanagi::CSysWindow::GetNativeWindowHandle(wndHandle);

	CMyAppl::GetInstance().Init(
		hWnd,
		hWnd);

	izanagi::CSysWindow::RunLoop(wndHandle);

	CMyAppl::GetInstance().Release();

	izanagi::CSysWindow::Destroy(wndHandle);

	return 0;
}
