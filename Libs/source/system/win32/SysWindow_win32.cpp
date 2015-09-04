#include "system/SysWindow.h"
#include <zmouse.h>

namespace izanagi
{
namespace sys
{
    //////////////////////////////////////////

    class CMsgHandlerManager : public CStdHash<IZ_UINT64, CMessageHandler, 4> {
    public:
        CMsgHandlerManager() {}
        ~CMsgHandlerManager() {}

    public:
        IZ_BOOL Register(
            CMessageHandler* handler,
            HWND hWnd);
    };

    IZ_BOOL CMsgHandlerManager::Register(
        CMessageHandler* handler,
        HWND hWnd)
    {
        IZ_ASSERT(handler != IZ_NULL);
        IZ_ASSERT(hWnd != IZ_NULL);

        IZ_UINT64 id = (IZ_UINT64)hWnd;
        handler->mHashItem.Init(id, handler);
    
        IZ_BOOL ret = Add(&handler->mHashItem);
        return ret;
    }

    static CMsgHandlerManager sMsgHandlerMgr;

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
        CMessageHandler* msgHandler = sMsgHandlerMgr.FindData((IZ_UINT64)hWnd);

        switch (uMsg) {
        case WM_DESTROY:
        case WM_CLOSE:
            ::PostQuitMessage(0);
            return 0;

        case WM_PAINT:
            if (msgHandler) {
                msgHandler->OnPaint();
                return 0;
            }
            break;

        case WM_ERASEBKGND:
            if (msgHandler) {
                msgHandler->OnPaint();
                return 0;
            }
            break;

        case WM_KEYDOWN:
            if (msgHandler) {
                E_KEYBOARD_BUTTON key = CSysWindow::GetKeyMap((IZ_UINT)wParam);
                msgHandler->OnKeyDown(key);
            }
            break;

        case WM_KEYUP:
            if (msgHandler) {
                E_KEYBOARD_BUTTON key = CSysWindow::GetKeyMap((IZ_UINT)wParam);
                msgHandler->OnKeyUp(key);
            }
            break;

        case WM_LBUTTONDOWN:
            if (msgHandler) {
                ::SetCapture(hWnd);
                CIntPoint point(
                    static_cast<short>(LOWORD(lParam)),
                    static_cast<short>(HIWORD(lParam)));
                msgHandler->OnMouseLBtnDown(point);
            }
            break;

        case WM_LBUTTONUP:
            if (msgHandler) {
                ::ReleaseCapture();
                CIntPoint point(
                    static_cast<short>(LOWORD(lParam)),
                    static_cast<short>(HIWORD(lParam)));
                msgHandler->OnMouseLBtnUp(point);
            }
            break;

        case WM_RBUTTONDOWN:
            if (msgHandler) {
                ::SetCapture(hWnd);
                CIntPoint point(
                    static_cast<short>(LOWORD(lParam)),
                    static_cast<short>(HIWORD(lParam)));
                msgHandler->OnMouseRBtnDown(point);
            }
            break;

        case WM_RBUTTONUP:
            if (msgHandler) {
                ::ReleaseCapture();
                CIntPoint point(
                    static_cast<short>(LOWORD(lParam)),
                    static_cast<short>(HIWORD(lParam)));
                msgHandler->OnMouseRBtnUp(point);
            }
            break;

        case WM_MOUSEMOVE:
            if (msgHandler) {
                CIntPoint point(
                    static_cast<short>(LOWORD(lParam)),
                    static_cast<short>(HIWORD(lParam)));
                msgHandler->OnMouseMove(point);
            }
            break;

        case WM_MOUSEWHEEL:
            if (msgHandler) {
                IZ_INT zDelta = static_cast<short>(HIWORD(wParam));
                msgHandler->OnMouseWheel(zDelta);
            }
            break;
        }

        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    // コンソールイベントを処理
    BOOL WINAPI HandlerRoutine(DWORD dwCtrlType)
    {
        switch (dwCtrlType) {
        case CTRL_C_EVENT:
            // コンソールのみを閉じる
            FreeConsole();
            return TRUE;
        }

        return FALSE;
    }

    static const char* registerName = "izanagi";

    static inline void _Destroy(
        HDC hDC,
        HWND hWnd,
        HINSTANCE hInst)
    {
        if (hDC) {
            ::ReleaseDC(hWnd, hDC);
        }

        if (hWnd) {
            DestroyWindow(hWnd);
        }

        if (hInst) {
            UnregisterClass(registerName, hInst);
        }
    }

    // ウインドウ破棄.
    void CSysWindow::Destroy(WindowHandle handle)
    {
        IZ_ASSERT(handle != IZ_NULL);

        CWindow* window = (CWindow*)handle;

        HDC hDC = NULL;
        HWND hWnd = NULL;
        HINSTANCE hInst = NULL;

        if (window != IZ_NULL) {
            hDC = window->GetHDC();
            hWnd = window->GetHWND();
            hInst = window->GetHINSTANCE();
        }

        _Destroy(hDC, hWnd, hInst);

        CWindow::Destroy(window);
    }

    // ウインドウ作成.
    WindowHandle CSysWindow::Create(
        IMemoryAllocator* allocator,
        const WindowParams& param)
    {
        IZ_ASSERT(allocator != IZ_NULL);

        //HINSTANCE hInst = (HINSTANCE)param.platformParam;
        HINSTANCE hInst = GetModuleHandleA(NULL);
        IZ_ASSERT(hInst != IZ_NULL);

        HDC hDC = NULL;
        HWND hWnd = NULL;

        IZ_BOOL result = IZ_TRUE;
        CWindow* window = IZ_NULL;

        WNDCLASSEX wcex;
        {
            wcex.cbSize         = sizeof(WNDCLASSEX);
            wcex.style          = CS_HREDRAW | CS_VREDRAW;
            wcex.lpfnWndProc    = WndProc;
            wcex.cbClsExtra     = 0;
            wcex.cbWndExtra     = 0;
            wcex.hInstance      = hInst;
            wcex.hIcon          = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
            wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
            //wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1); // システムカラー（白色）
            wcex.hbrBackground  = (HBRUSH)::GetStockObject(BLACK_BRUSH);
            wcex.lpszMenuName   = NULL;
            wcex.lpszClassName  = (param.title != IZ_NULL ? param.title : registerName);
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
            //style &= ~WS_MINIMIZEBOX;
            style &= ~WS_MAXIMIZEBOX;

            // TODO
            // サイズ変更無効化
            style &= ~WS_THICKFRAME;

            exStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

            // ウインドウモード
            result = ::AdjustWindowRect(
                &rect,
                style,
                FALSE);
            VGOTO(result, __EXIT__);

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
        hWnd = ::CreateWindowExA(
            exStyle,
            wcex.lpszClassName,
            param.title,
            style,
            rect.left, rect.top,
            rect.right, rect.bottom,
            NULL,
            NULL,
            hInst,
            NULL);
        VGOTO(result = (hWnd != NULL), __EXIT__);

        // ウインドウ表示
        ::ShowWindow(hWnd, SW_SHOWNORMAL);
        ::SetFocus(hWnd);
        ::SetForegroundWindow(hWnd);

        ::UpdateWindow(hWnd);

        hDC = ::GetDC(hWnd);
        VGOTO(result = (hDC != NULL), __EXIT__);

        window = CWindow::Create(
            allocator,
            hInst,
            hWnd,
            hDC);
        VGOTO(result = (window != IZ_NULL), __EXIT__);

        // メッセージハンドラを保持
        if (param.handler) {
            sMsgHandlerMgr.Register(param.handler, hWnd);

            if (window) {
                param.handler->OnInit(window);
            }
        }

        // コンソールウインドウの閉じるを無効にする
        HWND hConsoleWnd = ::GetConsoleWindow();
        if (hConsoleWnd != NULL) {
            HMENU hMenu = ::GetSystemMenu(hConsoleWnd, FALSE);
		    ::RemoveMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
        }

        // コンソールウインドウ用のハンドラを設定
        ::SetConsoleCtrlHandler(HandlerRoutine, TRUE);

__EXIT__:
        if (!result) {
            _Destroy(hDC, hWnd, hInst);
        }

        return window;
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
    void CSysWindow::RunLoop(WindowHandle handle)
    {
        IZ_ASSERT(handle != IZ_NULL);
    
        CWindow* window = (CWindow*)handle;
        HWND hWnd = window->GetHWND();

        CMessageHandler* msgHandler = sMsgHandlerMgr.FindData((IZ_UINT64)hWnd);

        while (ProcMsg()) {
            if (msgHandler) {
                msgHandler->OnIdle();
            }
        }

        if (msgHandler) {
            msgHandler->OnTerminate();
        }

        Destroy(handle);

        if (msgHandler) {
            msgHandler->OnDestroy();
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

    E_KEYBOARD_BUTTON CSysWindow::GetKeyMap(IZ_UINT key)
    {
        if ('0' <= key && key <= '9') {
            key = key - '0';
            return (E_KEYBOARD_BUTTON)(E_KEYBOARD_BUTTON_0 + key);
        }
        else if ('A' <= key && key <= 'Z') {
            key = key - 'A';
            return (E_KEYBOARD_BUTTON)(E_KEYBOARD_BUTTON_A + key);
        }
        else {
            switch (key) {
            case VK_UP:
                return E_KEYBOARD_BUTTON_UP;
            case VK_LEFT:
                return E_KEYBOARD_BUTTON_LEFT;
            case VK_DOWN:
                return E_KEYBOARD_BUTTON_DOWN;
            case VK_RIGHT:
                return E_KEYBOARD_BUTTON_RIGHT;
            case VK_CONTROL:
                return E_KEYBOARD_BUTTON_CONTROL;
            case VK_SHIFT:
                return E_KEYBOARD_BUTTON_SHIFT;
            case VK_RETURN:
                return E_KEYBOARD_BUTTON_RETURN;
            case VK_SPACE:
                return E_KEYBOARD_BUTTON_SPACE;
            case VK_BACK:
                return E_KEYBOARD_BUTTON_BACK;
            case VK_DELETE:
                return E_KEYBOARD_BUTTON_DELETE;
            default:
                break;
            }
        }

        //IZ_ASSERT(IZ_FALSE);
        return E_KEYBOARD_BUTTON_UNDEFINED;
    }
}   // namespace sys
}   // namespace izanagi
