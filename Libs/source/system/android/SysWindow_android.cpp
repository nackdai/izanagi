#include "system/SysWindow.h"

namespace izanagi
{
namespace sys
{
    // ウインドのもろもろを保持しておく用
    class CWindowAndroid : public CPlacementNew {
    public:
        static CWindowAndroid* s_Instance;

    public:
        // インスタンス作成
        static CWindowAndroid* Create(
            IMemoryAllocator* allocator,
            CMessageHandler* handler);

        // インスタンス破棄
        static void Destroy(CWindowAndroid* window);

    protected:
        CWindowAndroid() {}
        ~CWindowAndroid() {}

    public:
        CMessageHandler* GetHandler() { return m_MsgHandler; }

    private:
        IMemoryAllocator* m_Allocator;
        CMessageHandler* m_MsgHandler;
    };

    CWindowAndroid* CWindowAndroid::s_Instance = IZ_NULL;

    // インスタンス作成
    CWindowAndroid* CWindowAndroid::Create(
        IMemoryAllocator* allocator,
        CMessageHandler* handler)
    {
        if (s_Instance == IZ_NULL) {
            void* buf = ALLOC(allocator, sizeof(CWindowAndroid));
            VRETURN_NULL(buf != IZ_NULL);

            CWindowAndroid* window = new(buf) CWindowAndroid();

            window->m_Allocator = allocator;
            window->m_MsgHandler = handler;

            s_Instance = window;
        }

        return s_Instance;
    }

    // インスタンス破棄
    void CWindowAndroid::Destroy(CWindowAndroid* window)
    {
        IMemoryAllocator* allocator = window->m_Allocator;

        delete window;
        FREE(allocator, window);

        s_Instance = IZ_NULL;
    }

    ///////////////////////////////////////////////////////////////////

    E_KEYBOARD_BUTTON CSysWindow::GetKeyMap(IZ_UINT key)
    {
        IZ_ASSERT(IZ_FALSE);
        return E_KEYBOARD_BUTTON_UNDEFINED;
    }

    WindowHandle CSysWindow::Create(
        IMemoryAllocator* allocator,
        const WindowParams& param)
    {
        CWindowAndroid* window = CWindowAndroid::Create(allocator, param.handler);
        VRETURN_NULL(window != IZ_NULL);

        if (param.handler) {
            param.handler->OnInit(IZ_NULL);
        }

        return window;
    }

    // ウインドウ破棄.
    void CSysWindow::Destroy(WindowHandle handle)
    {
        IZ_ASSERT(CWindowAndroid::s_Instance != IZ_NULL);

        CMessageHandler* handler = CWindowAndroid::s_Instance->GetHandler();

        handler->OnTerminate();
        
        CWindowAndroid::Destroy(CWindowAndroid::s_Instance);

        handler->OnDestroy();
    }

    // ループ実行.
    void CSysWindow::RunLoop(WindowHandle handle)
    {
        IZ_ASSERT(CWindowAndroid::s_Instance != IZ_NULL);
        CWindowAndroid::s_Instance->GetHandler()->OnIdle();
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
