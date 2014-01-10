#include <GL/glut.h>
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

    static void Display()
    {
        IZ_ASSERT(CWindowGLUT::s_Instance != IZ_NULL);

        CWindowGLUT::s_Instance->GetHandler()->OnIdle();
    }

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
    }

    WindowHandle CSysWindow::CreateGLUT(
        IMemoryAllocator* allocator,
        const WindowParams& param)
    {
        CWindowGLUT* window = CWindowGLUT::Create(allocator, param.handler);
        VRETURN_NULL(window != IZ_NULL);

        glutInit(
            const_cast<int*>(&param.argc), 
            param.argv);

        glutInitWindowSize(param.width, param.height);
        glutInitDisplayMode(GLUT_RGBA);

        glutCreateWindow(param.title);

        

        glutDisplayFunc(Display);

        glutMainLoop();

        return window;
    }
}   // namespace sys
}   // namespace izanagi
