#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include "system/SysWindow.h"

using namespace izanagi;

static CMessageHandler* sHandler = IZ_NULL;

//////////////////////////////////////////

// ウインドのもろもろを保持しておく用
class CWindow : public CPlacementNew {
public:
	// インスタンス作成
	static CWindow* Create(
		IMemoryAllocator* allocator,
		HINSTANCE hInst,
		HWND hWnd,
		HDC hDC);

	// インスタンス破棄
	static void Destroy(CWindow* window);

protected:
	CWindow(HINSTANCE hInst, HWND hWnd, HDC hDC)
	{
		m_hInst = hInst;
		m_hWnd = hWnd;
		m_hDC = hDC;
	}

	~CWindow() {}

public:
	HINSTANCE GetHINSTANCE() { return m_hInst; }
	HWND GetHWND() { return m_hWnd; }
	HDC GetHDC() { return m_hDC; }

protected:
	IMemoryAllocator* m_Allocator;
	HINSTANCE m_hInst;
	HWND m_hWnd;
	HDC m_hDC;
};

// インスタンス作成
CWindow* CWindow::Create(
	IMemoryAllocator* allocator,
	HINSTANCE hInst,
	HWND hWnd,
	HDC hDC)
{
	void* buf = ALLOC_ZERO(allocator, sizeof(CWindow));
	VRETURN_NULL(buf != IZ_NULL);

	CWindow* window = new(buf) CWindow(hInst, hWnd, hDC);
	window->m_Allocator = allocator;

	return window;
}

// インスタンス破棄
void CWindow::Destroy(CWindow* window)
{
	IMemoryAllocator* allocator = window->m_Allocator;

	delete window;
	FREE(allocator, window);
}

//////////////////////////////////////////

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_DESTROY:
	case WM_CLOSE:
		::PostQuitMessage(0);
		return 0;

	case WM_PAINT:
	case WM_ERASEBKGND:
		if (sHandler) {
			sHandler->OnPaint();
		}
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

static const char* registerName = "izanagi";

// ウインドウ作成.
WindowHandle CSysWindow::Create(
	IMemoryAllocator* allocator,
	const WindowParams& param)
{
	IZ_ASSERT(allocator != IZ_NULL);

	HINSTANCE hInst = (HINSTANCE)param.platformParam;
	IZ_ASSERT(hInst != IZ_NULL);

	{
		WNDCLASSEX wcex;

		wcex.cbSize         = sizeof(WNDCLASSEX);
		wcex.style          = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc    = WndProc;
		wcex.cbClsExtra     = 0;
		wcex.cbWndExtra     = 0;
		wcex.hInstance      = hInst;
		wcex.hIcon          = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
		wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
		//wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);	// システムカラー（白色）
		wcex.hbrBackground  = (HBRUSH)::GetStockObject(BLACK_BRUSH);
		wcex.lpszMenuName   = NULL;
		wcex.lpszClassName  = registerName;
		wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

		VRETURN_NULL(RegisterClassEx(&wcex));
	}

	DWORD style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	DWORD exStyle = 0;

	RECT rect = {
		0, 0,
		param.width,
		param.height
	};

	if (param.isWindowed) {
		style |= WS_OVERLAPPEDWINDOW;

		// TODO
		// 最大化、最小化ボタンは無し
		style &= ~WS_MINIMIZEBOX;
		style &= ~WS_MAXIMIZEBOX;

		// TODO
		// サイズ変更無効化
		style &= ~WS_THICKFRAME;

		exStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

		// ウインドウモード
		VRETURN_NULL(
			::AdjustWindowRect(
				&rect,
				style,
				FALSE));

		rect.right = rect.right - rect.left;
		rect.bottom = rect.bottom - rect.top;
		rect.left = CW_USEDEFAULT;
		rect.top = CW_USEDEFAULT;
	}
	else {
		// フルスクリーンモード
		style |= WS_POPUP;
		exStyle = WS_EX_APPWINDOW;
	}

	// ウインドウ作成
	HWND hWnd = ::CreateWindowExA(
					exStyle,
					registerName,
					param.title,
					style,
					rect.left, rect.top,
					rect.right, rect.bottom,
					NULL,
					NULL,
					hInst,
					NULL);
	VRETURN_NULL(hWnd != NULL);

	// ウインドウ表示
	::ShowWindow(hWnd, SW_SHOWNORMAL);
	::UpdateWindow(hWnd);

	HDC hDC = ::GetDC(hWnd);
	VRETURN_NULL(hDC != NULL);

	CWindow* window = CWindow::Create(
						allocator,
						hInst,
						hWnd,
						hDC);

	// メッセージハンドラを保持
	sHandler = param.handler;

	return window;
}

// ウインドウ破棄.
void CSysWindow::Destroy(WindowHandle handle)
{
	IZ_ASSERT(handle != IZ_NULL);

	CWindow* window = (CWindow*)handle;

	HDC hDC = window->GetHDC();
	HWND hWnd = window->GetHWND();
	HINSTANCE hInst = window->GetHINSTANCE();

	if (hDC) {
		::ReleaseDC(hWnd, hDC);
    }

    if (hWnd) {
        DestroyWindow(hWnd);
    }

    if (hInst) {
        UnregisterClass(registerName, hInst);
    }

	CWindow::Destroy(window);
}

static IZ_BOOL ProcMsg()
{
	MSG msg;

	if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			return IZ_FALSE;
		}
		else {
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
	
	return IZ_TRUE;
}

// ループ実行.
void CSysWindow::RunLoop()
{
	while (ProcMsg()) {
		if (sHandler) {
			sHandler->OnIdle();
		}
	}
}

void* CSysWindow::GetNativeWindowHandle(const WindowHandle& handle)
{
	IZ_ASSERT(handle != IZ_NULL);
	
	CWindow* window = (CWindow*)handle;
	return window->GetHWND();
}

void* CSysWindow::GetNativeDisplayHandle(const WindowHandle& handle)
{
	CWindow* window = (CWindow*)handle;
	return window->GetHDC();
}
