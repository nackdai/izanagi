#ifdef __IZ_GLFW__
#include "system/SysWindow.h"

extern "C" {
    #include <GLFW/glfw3.h>
}

namespace izanagi
{
namespace sys
{
    //////////////////////////////////////////
    class CMsgHandlerManager : public CStdHash < IZ_UINT64, CMessageHandler, 4 > {
    public:
        CMsgHandlerManager() {}
        ~CMsgHandlerManager() {}

    public:
        IZ_BOOL Register(
            CMessageHandler* handler,
            void* key)
        {
            IZ_ASSERT(handler != IZ_NULL);
            //IZ_ASSERT(hWnd != IZ_NULL);

            IZ_UINT64 id = (IZ_UINT64)key;
            handler->mHashItem.Init(id, handler);

            IZ_BOOL ret = Add(&handler->mHashItem);
            return ret;
        }
    };
    //////////////////////////////////////////

    // ウインドのもろもろを保持しておく用
    class CWindowGLFW : public CPlacementNew {
    public:
        static CWindowGLFW* s_Instance;

    public:
        // インスタンス作成
        static CWindowGLFW* Create(
            IMemoryAllocator* allocator,
            CMessageHandler* handler);

        // インスタンス破棄
        static void Destroy(CWindowGLFW* window);

    protected:
        CWindowGLFW() {}
        ~CWindowGLFW() {}

    public:
        CMessageHandler* GetHandler()
        {
            IZ_UINT64 key = (IZ_UINT64)this;
            auto handler = m_handlerMgr.FindData(key);
            return handler;
        }

        IZ_BOOL registerHandler(CMessageHandler* handler, void* key)
        {
            auto ret = m_handlerMgr.Register(handler, key);
            return ret;
        }

        CMessageHandler* get(IZ_UINT64 key)
        {
            auto handler = m_handlerMgr.FindData(key);
            return handler;
        }

        GLFWwindow* glfwWindow{ nullptr };

        int mouseX{ 0 };
        int mouseY{ 0 };

    private:
        IMemoryAllocator* m_Allocator;
        CMsgHandlerManager m_handlerMgr;
    };

    CWindowGLFW* CWindowGLFW::s_Instance = IZ_NULL;

    // インスタンス作成
    CWindowGLFW* CWindowGLFW::Create(
        IMemoryAllocator* allocator,
        CMessageHandler* handler)
    {
        if (s_Instance == IZ_NULL) {
            void* buf = ALLOC(allocator, sizeof(CWindowGLFW));
            VRETURN_NULL(buf != IZ_NULL);

            CWindowGLFW* window = new(buf)CWindowGLFW();

            window->m_Allocator = allocator;
            window->registerHandler(handler, window);

            s_Instance = window;
        }

        return s_Instance;
    }

    // インスタンス破棄
    void CWindowGLFW::Destroy(CWindowGLFW* window)
    {
        IMemoryAllocator* allocator = window->m_Allocator;

        delete window;
        FREE(allocator, window);

        s_Instance = IZ_NULL;
    }

    ///////////////////////////////////////////////////////////////////

    static void Display()
    {
        IZ_ASSERT(CWindowGLFW::s_Instance != IZ_NULL);

        CWindowGLFW::s_Instance->GetHandler()->OnIdle();
    }

    static void closeCallback(GLFWwindow* window)
    {
        IZ_ASSERT(CWindowGLFW::s_Instance != IZ_NULL);

#if 0
        CMessageHandler* handler = CWindowGLFW::s_Instance->GetHandler();

        handler->OnTerminate();
        
        CWindowGLFW::Destroy(CWindowGLFW::s_Instance);

        handler->OnDestroy();

        glfwDestroyWindow(CWindowGLFW::s_Instance->glfwWindow);
        glfwTerminate();
#else
        ::glfwSetWindowShouldClose(window, GL_TRUE);
#endif
    }

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        IZ_ASSERT(CWindowGLFW::s_Instance != IZ_NULL);

        //if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)

        E_KEYBOARD_BUTTON mappedKey = CSysWindow::GetKeyMap(key);

        CMessageHandler* handler = CWindowGLFW::s_Instance->GetHandler();

        if (action == GLFW_PRESS) {
            handler->OnKeyDown(mappedKey);
        }
        else if (action == GLFW_RELEASE) {
            handler->OnKeyUp(mappedKey);
        }

        CMessageHandler* handlerEx = CWindowGLFW::s_Instance->get(0);
        if (handlerEx) {
            if (action == GLFW_PRESS) {
                handlerEx->OnKeyDown(mappedKey);
            }
            else if (action == GLFW_RELEASE) {
                handlerEx->OnKeyUp(mappedKey);
            }
        }
    }

    static void mouseCallback(GLFWwindow* window, int button, int action, int mods)
    {
        IZ_ASSERT(CWindowGLFW::s_Instance != IZ_NULL);

        CMessageHandler* handler = CWindowGLFW::s_Instance->GetHandler();

        auto x = CWindowGLFW::s_Instance->mouseX;
        auto y = CWindowGLFW::s_Instance->mouseY;

        CIntPoint pt(x, y);

        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            if (action == GLFW_PRESS) {
                handler->OnMouseLBtnDown(pt);
            }
            else if (action == GLFW_RELEASE) {
                handler->OnMouseLBtnUp(pt);
            }
        }
        else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            if (action == GLFW_PRESS) {
                handler->OnMouseRBtnDown(pt);
            }
            else if (action == GLFW_RELEASE) {
                handler->OnMouseRBtnUp(pt);
            }
        }

        CMessageHandler* handlerEx = CWindowGLFW::s_Instance->get(0);
        if (handlerEx) {
            if (button == GLFW_MOUSE_BUTTON_LEFT) {
                if (action == GLFW_PRESS) {
                    handlerEx->OnMouseLBtnDown(pt);
                }
                else if (action == GLFW_RELEASE) {
                    handlerEx->OnMouseLBtnUp(pt);
                }
            }
            else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
                if (action == GLFW_PRESS) {
                    handlerEx->OnMouseRBtnDown(pt);
                }
                else if (action == GLFW_RELEASE) {
                    handlerEx->OnMouseRBtnUp(pt);
                }
            }
        }
    }

    static void motionCallback(GLFWwindow* window, double xpos, double ypos)
    {
        IZ_ASSERT(CWindowGLFW::s_Instance != IZ_NULL);

        CWindowGLFW::s_Instance->mouseX = (int)xpos;
        CWindowGLFW::s_Instance->mouseY = (int)ypos;

        CIntPoint pt(CWindowGLFW::s_Instance->mouseX, CWindowGLFW::s_Instance->mouseY);

        CWindowGLFW::s_Instance->GetHandler()->OnMouseMove(pt);

        CMessageHandler* handlerEx = CWindowGLFW::s_Instance->get(0);
        if (handlerEx) {
            handlerEx->OnMouseMove(pt);
        }
    }

    static void wheelCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        IZ_ASSERT(CWindowGLFW::s_Instance != IZ_NULL);

        // TODO.
        // scale.
        auto offset = (int)(yoffset * 10.0f);

        CWindowGLFW::s_Instance->GetHandler()->OnMouseWheel(offset);

        CMessageHandler* handlerEx = CWindowGLFW::s_Instance->get(0);
        if (handlerEx) {
            handlerEx->OnMouseWheel(offset);
        }
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
            case GLFW_KEY_ESCAPE:
                return E_KEYBOARD_BUTTON_ESCAPE;
            case GLFW_KEY_ENTER:
                return E_KEYBOARD_BUTTON_RETURN;
            case GLFW_KEY_TAB:
                return E_KEYBOARD_BUTTON_TAB;
            case GLFW_KEY_BACKSPACE:
                return E_KEYBOARD_BUTTON_BACKSPACE;
            case GLFW_KEY_INSERT:
                return E_KEYBOARD_BUTTON_INSERT;
            case GLFW_KEY_DELETE:
                return E_KEYBOARD_BUTTON_DELETE;
            case GLFW_KEY_RIGHT:
                return E_KEYBOARD_BUTTON_RIGHT;
            case GLFW_KEY_LEFT:
                return E_KEYBOARD_BUTTON_LEFT;
            case GLFW_KEY_DOWN:
                return E_KEYBOARD_BUTTON_DOWN;
            case GLFW_KEY_UP:
                return E_KEYBOARD_BUTTON_UP;
            case GLFW_KEY_F1:
                return E_KEYBOARD_BUTTON_F1;
            case GLFW_KEY_F2:
                return E_KEYBOARD_BUTTON_F2;
            case GLFW_KEY_F3:
                return E_KEYBOARD_BUTTON_F3;
            case GLFW_KEY_F4:
                return E_KEYBOARD_BUTTON_F4;
            case GLFW_KEY_F5:
                return E_KEYBOARD_BUTTON_F5;
            case GLFW_KEY_F6:
                return E_KEYBOARD_BUTTON_F6;
            case GLFW_KEY_F7:
                return E_KEYBOARD_BUTTON_F7;
            case GLFW_KEY_F8:
                return E_KEYBOARD_BUTTON_F8;
            case GLFW_KEY_F9:
                return E_KEYBOARD_BUTTON_F9;
            case GLFW_KEY_F10:
                return E_KEYBOARD_BUTTON_F10;
            case GLFW_KEY_F11:
                return E_KEYBOARD_BUTTON_F11;
            case GLFW_KEY_F12:
                return E_KEYBOARD_BUTTON_F12;
            case GLFW_KEY_LEFT_SHIFT:
                return E_KEYBOARD_BUTTON_SHIFT;
            case GLFW_KEY_LEFT_CONTROL:
                return E_KEYBOARD_BUTTON_CONTROL;
            case GLFW_KEY_RIGHT_SHIFT:
                return E_KEYBOARD_BUTTON_SHIFT;
            case GLFW_KEY_RIGHT_CONTROL:
                return E_KEYBOARD_BUTTON_CONTROL;
            case GLFW_KEY_SPACE:
                return E_KEYBOARD_BUTTON_SPACE;
            default:
                break;
            }
        }

        //IZ_ASSERT(IZ_FALSE);
        return E_KEYBOARD_BUTTON_UNDEFINED;
    }

    IZ_INT CSysWindow::GetOriginalKey(E_KEYBOARD_BUTTON key)
    {
        if (E_KEYBOARD_BUTTON_0 <= key && key <= E_KEYBOARD_BUTTON_9) {
            auto c = key - E_KEYBOARD_BUTTON_0 + '0';
            return c;
        }
        else if (E_KEYBOARD_BUTTON_A <= key && key <= E_KEYBOARD_BUTTON_Z) {
            auto c = key - E_KEYBOARD_BUTTON_A + 'A';
            return c;
        }
        else if (E_KEYBOARD_BUTTON_F1 <= key && key <= E_KEYBOARD_BUTTON_F12) {
            auto c = key - E_KEYBOARD_BUTTON_F1 + GLFW_KEY_F1;
            return c;
        }
        else {
            switch (key) {
            case E_KEYBOARD_BUTTON_UP:
                return GLFW_KEY_UP;
            case E_KEYBOARD_BUTTON_LEFT:
                return GLFW_KEY_LEFT;
            case E_KEYBOARD_BUTTON_DOWN:
                return GLFW_KEY_DOWN;
            case E_KEYBOARD_BUTTON_RIGHT:
                return GLFW_KEY_RIGHT;
            case E_KEYBOARD_BUTTON_CONTROL:
                return GLFW_KEY_LEFT_CONTROL;   // TODO
            case E_KEYBOARD_BUTTON_SHIFT:
                return GLFW_KEY_LEFT_SHIFT;     // TODO
            case E_KEYBOARD_BUTTON_RETURN:
                return GLFW_KEY_ENTER;
            case E_KEYBOARD_BUTTON_SPACE:
                return GLFW_KEY_SPACE;
            case E_KEYBOARD_BUTTON_BACK:
                return GLFW_KEY_BACKSPACE;
            case E_KEYBOARD_BUTTON_DELETE:
                return GLFW_KEY_DELETE;
            case E_KEYBOARD_BUTTON_TAB:
                return GLFW_KEY_TAB;
            case E_KEYBOARD_BUTTON_PRIOR:
                return GLFW_KEY_PAGE_UP;
            case E_KEYBOARD_BUTTON_NEXT:
                return GLFW_KEY_PAGE_DOWN;
            case E_KEYBOARD_BUTTON_HOME:
                return GLFW_KEY_HOME;
            case E_KEYBOARD_BUTTON_END:
                return GLFW_KEY_END;
            case E_KEYBOARD_BUTTON_ESCAPE:
                return GLFW_KEY_ESCAPE;
            default:
                break;
            }
        }

        return -1;
    }

    static void errorCallback(int error, const char* description)
    {
        IZ_PRINTF("Error: %s\n", description);
    }

    WindowHandle CSysWindow::Create(
        IMemoryAllocator* allocator,
        const WindowParams& param)
    {
        ::glfwSetErrorCallback(errorCallback);

        auto result = ::glfwInit();
        if (!result) {
            IZ_ASSERT(IZ_FALSE);
            return nullptr;
        }

        ::glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        ::glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

        CWindowGLFW* window = CWindowGLFW::Create(allocator, param.handler);
        VRETURN_NULL(window != IZ_NULL);

        window->glfwWindow = ::glfwCreateWindow(
            param.width,
            param.height,
            param.title,
            NULL, NULL);

        if (!window->glfwWindow) {
            ::glfwTerminate();
            IZ_ASSERT(IZ_FALSE);
            return nullptr;
        }

        ::glfwSetWindowCloseCallback(
            window->glfwWindow,
            closeCallback);

        ::glfwSetKeyCallback(
            window->glfwWindow,
            keyCallback);

        ::glfwSetMouseButtonCallback(
            window->glfwWindow,
            mouseCallback);

        ::glfwSetCursorPosCallback(
            window->glfwWindow,
            motionCallback);

        ::glfwSetScrollCallback(
            window->glfwWindow,
            wheelCallback);

        glfwMakeContextCurrent(window->glfwWindow);
        glfwSwapInterval(1);

        if (param.handler) {
            param.handler->OnInit(IZ_NULL);
        }

        return window;
    }

    // ウインドウ破棄.
    void CSysWindow::Destroy(WindowHandle handle)
    {
    }

    // ループ実行.
    void CSysWindow::RunLoop(WindowHandle handle)
    {
        IZ_ASSERT(CWindowGLFW::s_Instance != IZ_NULL);

        auto window = CWindowGLFW::s_Instance->glfwWindow;
        IZ_ASSERT(window != nullptr);

        while (!glfwWindowShouldClose(window))
        {
            CWindowGLFW::s_Instance->GetHandler()->OnIdle(window);

            glfwPollEvents();
        }

        CMessageHandler* handler = CWindowGLFW::s_Instance->GetHandler();
        CMessageHandler* handlerEx = CWindowGLFW::s_Instance->get(0);

        handler->OnTerminate();

        if (handlerEx) {
            handlerEx->OnTerminate();
        }

        CWindowGLFW::Destroy(CWindowGLFW::s_Instance);

        handler->OnDestroy();

        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void* CSysWindow::GetNativeWindowHandle(const WindowHandle& handle)
    {
        return IZ_NULL;
    }

    void* CSysWindow::GetNativeDisplayHandle(const WindowHandle& handle)
    {
        return IZ_NULL;
    }

    void CSysWindow::HideCursor()
    {
        glfwSetInputMode(CWindowGLFW::s_Instance->glfwWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }

    CIntPoint CSysWindow::getDisplaySize()
    {
        int w, h;
        glfwGetWindowSize(CWindowGLFW::s_Instance->glfwWindow, &w, &h);

        CIntPoint ret(w, h);

        return std::move(ret);
    }

    IZ_BOOL CSysWindow::registerExtendMsgHandler(CMessageHandler* handler)
    {
        auto h = CWindowGLFW::s_Instance->get(0);

        if (h) {
            return IZ_FALSE;
        }
        else {
            CWindowGLFW::s_Instance->registerHandler(handler, 0);
            return IZ_TRUE;
        }
    }
}   // namespace sys
}   // namespace izanagi
#endif  // #ifdef __IZ_GLFW__
