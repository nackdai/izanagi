#if !defined(__IZANAGI_SYSTEM_SYS_WINDOW_H__)
#define __IZANAGI_SYSTEM_SYS_WINDOW_H__

#include "izDefs.h"
#include "izStd.h"
#include "system/Keyboard.h"
#include "system/Mouse.h"

namespace izanagi
{
namespace sys
{
    class CMsgHandlerManager;

    /**
     */
    typedef void*   WindowHandle;

    /**
     */
    class CMessageHandler {
        friend class CMsgHandlerManager;

    protected:
        CMessageHandler() {}
        virtual ~CMessageHandler() {}

        NO_COPIABLE(CMessageHandler);

    public:
        virtual void OnKeyDown(E_KEYBOARD_BUTTON key) {}

        virtual void OnKeyUp(E_KEYBOARD_BUTTON key) {}

        virtual void OnMouseLBtnDown(const CIntPoint& point) {}

        virtual void OnMouseLBtnUp(const CIntPoint& point) {}

        virtual void OnMouseRBtnDown(const CIntPoint& point) {}

        virtual void OnMouseRBtnUp(const CIntPoint& point) {}

        virtual void OnMouseMove(const CIntPoint& point) {}

        virtual void OnMouseWheel(IZ_INT delta) {}

        virtual void OnPaint() {}

        virtual void OnIdle(void* nativeParam = nullptr) {}

        virtual void OnInit(const WindowHandle& handle) {}

        virtual void OnTerminate() {}
        virtual void OnDestroy() {}

    private:
        CStdHash<IZ_UINT64, CMessageHandler, 4>::Item mHashItem;
    };

    /**
     */
    struct WindowParams {
        IZ_INT argc;
        char** argv;
        IZ_UINT width;              ///< ウインドウ幅.
        IZ_UINT height;             ///< ウインドウ高さ.
        IZ_BOOL isWindowed;         ///< ウインドウモードにするかどうか.
        IZ_PCSTR title;             ///< ウインドウタイトル.
        IZ_PLATFORM_PARAM platformParam;    ///< プラットフォーム固有パラメータ.
        CMessageHandler* handler;           ///< メッセージハンドラ.
        CMessageHandler* handlerEx;         ///< メッセージハンドラ拡張.
    };

    /**
     */
    class CSysWindow {
    private:
        CSysWindow();
        ~CSysWindow();

        NO_COPIABLE(CSysWindow);

    private:
        /** ウインドウ破棄.
         */
        static void Destroy(WindowHandle handle);

    public:
        /** ウインドウ作成.
         */
        static WindowHandle Create(
            IMemoryAllocator* allocator,
            const WindowParams& param);

        /** ループ実行.
         */
        static void RunLoop(WindowHandle handle);

        static void* GetNativeWindowHandle(const WindowHandle& handle);

        static void* GetNativeDisplayHandle(const WindowHandle& handle);

        static E_KEYBOARD_BUTTON GetKeyMap(IZ_UINT key);

        static IZ_INT GetOriginalKey(E_KEYBOARD_BUTTON key);

        static void HideCursor();

        static CIntPoint getDisplaySize();

        static CKeyboard* GetKeyboard(const WindowHandle& handle);
        static CMouse* GetMouse(const WindowHandle& handle);
    };
}   // namespace sys
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SYSTEM_SYS_WINDOW_H__)
