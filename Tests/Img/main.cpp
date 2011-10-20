#include <windows.h>

#include "izStd.h"
#include "izSystem.h"

#include "MyAppl.h"
#include "MySystem.h"

izanagi::CStandardMemoryAllocator sAllocator;

IZ_UINT8 sBuf[1 * 1024 * 1024];

class CWndProc : public izanagi::CMessageHandler {
public:
	CWndProc() {}
	~CWndProc() {}

public:
	void OnKeyDown(IZ_UINT nChar)
	{
		IZ_PRINTF("%d\n", nChar);
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
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	izanagi::CSysUtil::SetCurrentDirectoryFromExe();

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
		hInstance,
		hWnd,
		hWnd);

	izanagi::CSysWindow::RunLoop(wndHandle);

	CMyAppl::GetInstance().Release();

	izanagi::CSysWindow::Destroy(wndHandle);

	return 0;
}
