extern "C" {
    #include <GL/glut.h>
    #include <GL/freeglut.h>
}

#include "system/SysWindow.h"

namespace izanagi
{
namespace sys
{
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
        CMessageHandler* GetHandler() { return m_MsgHandler; }

    private:
        IMemoryAllocator* m_Allocator;
        CMessageHandler* m_MsgHandler;
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
            window->m_MsgHandler = handler;

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

        handler->OnTerminate();
        
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
    }

    static void KeyUp(unsigned char key, int x, int y)
    {
        IZ_ASSERT(CWindowGLUT::s_Instance != IZ_NULL);

        E_KEYBOARD_BUTTON mappedKey = CSysWindow::GetKeyMap(key);

        CWindowGLUT::s_Instance->GetHandler()->OnKeyUp(mappedKey);
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
        }

        return E_KEYBOARD_BUTTON_UNDEFINED;
    }

    static void SpecialKeyDown(int key, int x, int y)
    {
        IZ_ASSERT(CWindowGLUT::s_Instance != IZ_NULL);

        E_KEYBOARD_BUTTON mappedKey = _GetKeyMap(key);

        CWindowGLUT::s_Instance->GetHandler()->OnKeyDown(mappedKey);
    }

    static void SpecialKeyUp(int key, int x, int y)
    {
        IZ_ASSERT(CWindowGLUT::s_Instance != IZ_NULL);

        E_KEYBOARD_BUTTON mappedKey = _GetKeyMap(key);

        CWindowGLUT::s_Instance->GetHandler()->OnKeyUp(mappedKey);
    }

    static void Mouse(int button, int state, int x, int y)
    {
        IZ_ASSERT(CWindowGLUT::s_Instance != IZ_NULL);

        CMessageHandler* handler = CWindowGLUT::s_Instance->GetHandler();

        if (button == GLUT_LEFT_BUTTON) {
            if (state == GLUT_DOWN) {
                handler->OnMouseLBtnDown(CIntPoint(x, y));
            }
            else if (state == GLUT_UP) {
                handler->OnMouseLBtnUp(CIntPoint(x, y));
            }
        }
        else if (button == GLUT_RIGHT_BUTTON) {
            if (state == GLUT_DOWN) {
                handler->OnMouseRBtnDown(CIntPoint(x, y));
            }
            else if (state == GLUT_UP) {
                handler->OnMouseRBtnUp(CIntPoint(x, y));
            }
        }
    }

    static void Motion(int x, int y)
    {
        IZ_ASSERT(CWindowGLUT::s_Instance != IZ_NULL);

        CWindowGLUT::s_Instance->GetHandler()->OnMouseMove(CIntPoint(x, y));
    }

    static void Wheel(int wheel_number, int direction, int x, int y)
    {
        IZ_ASSERT(CWindowGLUT::s_Instance != IZ_NULL);

        // NOTE
        // We can not wheel delta on GLUT.
        // So, I send temporary value.
        CWindowGLUT::s_Instance->GetHandler()->OnMouseWheel(direction * 10);
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
            default:
                break;
            }
        }

        IZ_ASSERT(IZ_FALSE);
        return E_KEYBOARD_BUTTON_UNDEFINED;
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

}   // namespace sys
}   // namespace izanagi
