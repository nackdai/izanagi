#ifdef __IZ_GLUT__
extern "C" {
    #include <GL/glut.h>
    #include <GL/freeglut.h>
}

#include "system/SysWindow.h"

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
    class CWindowGLUT : public CPlacementNew {
    public:
        static CWindowGLUT* s_Instance;

    public:
        // インスタンス作成
        static CWindowGLUT* Create(
            IMemoryAllocator* allocator,
            CMessageHandler* handler);

        // インスタンス破棄
        static void Destroy(CWindowGLUT* window);

    protected:
        CWindowGLUT() {}

        ~CWindowGLUT() {}

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

    private:
        IMemoryAllocator* m_Allocator;
        CMsgHandlerManager m_handlerMgr;
    };

    CWindowGLUT* CWindowGLUT::s_Instance = IZ_NULL;

    // インスタンス作成
    CWindowGLUT* CWindowGLUT::Create(
        IMemoryAllocator* allocator,
        CMessageHandler* handler)
    {
        if (s_Instance == IZ_NULL) {
            void* buf = ALLOC(allocator, sizeof(CWindowGLUT));
            VRETURN_NULL(buf != IZ_NULL);

            CWindowGLUT* window = new(buf) CWindowGLUT();

            window->m_Allocator = allocator;
            window->m_handlerMgr.Register(handler, window);

            s_Instance = window;
        }

        return s_Instance;
    }

    // インスタンス破棄
    void CWindowGLUT::Destroy(CWindowGLUT* window)
    {
        IMemoryAllocator* allocator = window->m_Allocator;

        delete window;
        FREE(allocator, window);

        s_Instance = IZ_NULL;
    }

    ///////////////////////////////////////////////////////////////////

    static void Display()
    {
        IZ_ASSERT(CWindowGLUT::s_Instance != IZ_NULL);

        CWindowGLUT::s_Instance->GetHandler()->OnIdle();
    }

    static void Close()
    {
        IZ_ASSERT(CWindowGLUT::s_Instance != IZ_NULL);

        CMessageHandler* handler = CWindowGLUT::s_Instance->GetHandler();
        CMessageHandler* handlerEx = CWindowGLUT::s_Instance->get(0);

        handler->OnTerminate();

        if (handlerEx) {
            handlerEx->OnTerminate();
        }
        
        CWindowGLUT::Destroy(CWindowGLUT::s_Instance);

        handler->OnDestroy();
    }

    static void Idle()
    {
        glutPostRedisplay();
    }

    static void KeyDown(unsigned char key, int x, int y)
    {
        IZ_ASSERT(CWindowGLUT::s_Instance != IZ_NULL);

        E_KEYBOARD_BUTTON mappedKey = CSysWindow::GetKeyMap(key);

        CWindowGLUT::s_Instance->GetHandler()->OnKeyDown(mappedKey);

        CMessageHandler* handlerEx = CWindowGLUT::s_Instance->get(0);
        if (handlerEx) {
            handlerEx->OnKeyDown(mappedKey);
        }
    }

    static void KeyUp(unsigned char key, int x, int y)
    {
        IZ_ASSERT(CWindowGLUT::s_Instance != IZ_NULL);

        E_KEYBOARD_BUTTON mappedKey = CSysWindow::GetKeyMap(key);

        CWindowGLUT::s_Instance->GetHandler()->OnKeyUp(mappedKey);

        CMessageHandler* handlerEx = CWindowGLUT::s_Instance->get(0);
        if (handlerEx) {
            handlerEx->OnKeyUp(mappedKey);
        }
    }

    static inline E_KEYBOARD_BUTTON _GetKeyMap(int key)
    {
        switch (key) {
        case GLUT_KEY_UP:
            return E_KEYBOARD_BUTTON_UP;
        case GLUT_KEY_LEFT:
            return E_KEYBOARD_BUTTON_LEFT;
        case GLUT_KEY_DOWN:
            return E_KEYBOARD_BUTTON_DOWN;
        case GLUT_KEY_RIGHT:
            return E_KEYBOARD_BUTTON_RIGHT;
        case GLUT_KEY_CTRL_L:
        case GLUT_KEY_CTRL_R:
            return E_KEYBOARD_BUTTON_CONTROL;
        case GLUT_KEY_SHIFT_L:
        case GLUT_KEY_SHIFT_R:
            return E_KEYBOARD_BUTTON_SHIFT;
        }

        return E_KEYBOARD_BUTTON_UNDEFINED;
    }

    static void SpecialKeyDown(int key, int x, int y)
    {
        IZ_ASSERT(CWindowGLUT::s_Instance != IZ_NULL);

        E_KEYBOARD_BUTTON mappedKey = _GetKeyMap(key);

        CWindowGLUT::s_Instance->GetHandler()->OnKeyDown(mappedKey);

        CMessageHandler* handlerEx = CWindowGLUT::s_Instance->get(0);
        if (handlerEx) {
            handlerEx->OnKeyDown(mappedKey);
        }
    }

    static void SpecialKeyUp(int key, int x, int y)
    {
        IZ_ASSERT(CWindowGLUT::s_Instance != IZ_NULL);

        E_KEYBOARD_BUTTON mappedKey = _GetKeyMap(key);

        CWindowGLUT::s_Instance->GetHandler()->OnKeyUp(mappedKey);

        CMessageHandler* handlerEx = CWindowGLUT::s_Instance->get(0);
        if (handlerEx) {
            handlerEx->OnKeyUp(mappedKey);
        }
    }

    static void Mouse(int button, int state, int x, int y)
    {
        IZ_ASSERT(CWindowGLUT::s_Instance != IZ_NULL);

        CMessageHandler* handler = CWindowGLUT::s_Instance->GetHandler();

        CIntPoint pt(x, y);

        if (button == GLUT_LEFT_BUTTON) {
            if (state == GLUT_DOWN) {
                handler->OnMouseLBtnDown(pt);
            }
            else if (state == GLUT_UP) {
                handler->OnMouseLBtnUp(pt);
            }
        }
        else if (button == GLUT_RIGHT_BUTTON) {
            if (state == GLUT_DOWN) {
                handler->OnMouseRBtnDown(pt);
            }
            else if (state == GLUT_UP) {
                handler->OnMouseRBtnUp(pt);
            }
        }

        CMessageHandler* handlerEx = CWindowGLUT::s_Instance->get(0);
        if (handlerEx) {
            if (button == GLUT_LEFT_BUTTON) {
                if (state == GLUT_DOWN) {
                    handlerEx->OnMouseLBtnDown(pt);
                }
                else if (state == GLUT_UP) {
                    handlerEx->OnMouseLBtnUp(pt);
                }
            }
            else if (button == GLUT_RIGHT_BUTTON) {
                if (state == GLUT_DOWN) {
                    handlerEx->OnMouseRBtnDown(pt);
                }
                else if (state == GLUT_UP) {
                    handlerEx->OnMouseRBtnUp(pt);
                }
            }
        }
    }

    static void Motion(int x, int y)
    {
        IZ_ASSERT(CWindowGLUT::s_Instance != IZ_NULL);

        CIntPoint pt(x, y);

        CWindowGLUT::s_Instance->GetHandler()->OnMouseMove(pt);

        CMessageHandler* handlerEx = CWindowGLUT::s_Instance->get(0);
        if (handlerEx) {
            handlerEx->OnMouseMove(pt);
        }
    }

    static void Wheel(int wheel_number, int direction, int x, int y)
    {
        IZ_ASSERT(CWindowGLUT::s_Instance != IZ_NULL);

        // NOTE
        // We can not wheel delta on GLUT.
        // So, I send temporary value.
        auto delta = direction * 10;

        CWindowGLUT::s_Instance->GetHandler()->OnMouseWheel(delta);

        CMessageHandler* handlerEx = CWindowGLUT::s_Instance->get(0);
        if (handlerEx) {
            handlerEx->OnMouseWheel(delta);
        }
    }

#ifndef VK_CONTROL
    #define VK_CONTROL 0x11
#endif

#ifndef VK_SHIFT
    #define VK_SHIFT 0x10
#endif

#ifndef VK_RETURN
    #define VK_RETURN 0x0D
#endif

#ifndef VK_SPACE
    #define VK_SPACE 0x20
#endif

#ifndef VK_BACK
    #define VK_BACK 0x08
#endif

#ifndef VK_DELETE
    #define VK_DELETE 0x2E
#endif

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
            case VK_PAUSE:
                return E_KEYBOARD_BUTTON_PAUSE;
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
            auto c = key - E_KEYBOARD_BUTTON_F1 + VK_F1;
            return c;
        }
        else {
            switch (key) {
            case E_KEYBOARD_BUTTON_UP:
                return VK_UP;
            case E_KEYBOARD_BUTTON_LEFT:
                return VK_LEFT;
            case E_KEYBOARD_BUTTON_DOWN:
                return VK_DOWN;
            case E_KEYBOARD_BUTTON_RIGHT:
                return VK_RIGHT;
            case E_KEYBOARD_BUTTON_CONTROL:
                return VK_CONTROL;
            case E_KEYBOARD_BUTTON_SHIFT:
                return VK_SHIFT;
            case E_KEYBOARD_BUTTON_RETURN:
                return VK_RETURN;
            case E_KEYBOARD_BUTTON_SPACE:
                return VK_SPACE;
            case E_KEYBOARD_BUTTON_BACK:
                return VK_BACK;
            case E_KEYBOARD_BUTTON_DELETE:
                return VK_DELETE;
            case E_KEYBOARD_BUTTON_TAB:
                return VK_TAB;
            case E_KEYBOARD_BUTTON_PRIOR:
                return VK_PRIOR;
            case E_KEYBOARD_BUTTON_NEXT:
                return VK_NEXT;
            case E_KEYBOARD_BUTTON_HOME:
                return VK_HOME;
            case E_KEYBOARD_BUTTON_END:
                return VK_END;
            case E_KEYBOARD_BUTTON_ESCAPE:
                return VK_ESCAPE;
            default:
                break;
            }
        }

        return -1;
    }

    WindowHandle CSysWindow::Create(
        IMemoryAllocator* allocator,
        const WindowParams& param)
    {
        CWindowGLUT* window = CWindowGLUT::Create(allocator, param.handler);
        VRETURN_NULL(window != IZ_NULL);

        ::glutInit(
            const_cast<int*>(&param.argc), 
            param.argv);

        ::glutInitWindowSize(param.width, param.height);

        ::glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
        //::glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);

        ::glutCreateWindow(param.title);

        if (param.handler) {
            param.handler->OnInit(IZ_NULL);
        }

        ::glutDisplayFunc(Display);
        ::glutIdleFunc(Idle);
        ::glutKeyboardFunc(KeyDown);
        ::glutKeyboardUpFunc(KeyUp);
        ::glutSpecialFunc(SpecialKeyDown);
        ::glutSpecialUpFunc(SpecialKeyUp);
        ::glutMouseFunc(Mouse);
        ::glutMotionFunc(Motion);
        ::glutMouseWheelFunc(Wheel);

        ::glutCloseFunc(Close);

        ::glutMainLoop();

        return window;
    }

    // ウインドウ破棄.
    void CSysWindow::Destroy(WindowHandle handle)
    {
    }

    // ループ実行.
    void CSysWindow::RunLoop(WindowHandle handle)
    {
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
        glutSetCursor(GLUT_CURSOR_NONE);
    }

    CIntPoint CSysWindow::getDisplaySize()
    {
        auto w = glutGet(GLUT_WINDOW_WIDTH);
        auto h = glutGet(GLUT_WINDOW_HEIGHT);

        CIntPoint ret(w, h);

        return std::move(ret);
    }

    IZ_BOOL CSysWindow::registerExtendMsgHandler(CMessageHandler* handler)
    {
        auto h = CWindowGLUT::s_Instance->get(0);

        if (h) {
            return IZ_FALSE;
        }
        else {
            CWindowGLUT::s_Instance->registerHandler(handler, 0);
            return IZ_TRUE;
        }
    }
}   // namespace sys
}   // namespace izanagi
#endif  // #ifdef __IZ_GLUT__
