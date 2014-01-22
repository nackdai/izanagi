#if !defined(__IZ_SAMPLE_SAMPLE_WINDOW_H__)
#define __IZ_SAMPLE_SAMPLE_WINDOW_H__

#include "izSystem.h"
#include "SampleApp.h"

namespace izanagi {
namespace sample {
    /** サンプルウインドウメッセージハンドラ.
     */
    class CSampleWndProc : public izanagi::sys::CMessageHandler {
        friend class CSampleApp;

    public:
        CSampleWndProc(CSampleApp* app);
        virtual ~CSampleWndProc();

    public:
        virtual void OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key);
        virtual void OnKeyUp(izanagi::sys::E_KEYBOARD_BUTTON key);

        virtual void OnMouseLBtnDown(const izanagi::CIntPoint& point);
        virtual void OnMouseLBtnUp(const izanagi::CIntPoint& point);
        virtual void OnMouseRBtnDown(const izanagi::CIntPoint& point);
        virtual void OnMouseRBtnUp(const izanagi::CIntPoint& point);
        virtual void OnMouseMove(const izanagi::CIntPoint& point);
        virtual void OnMouseWheel(IZ_INT delta);

        virtual void OnPaint();

        virtual void OnIdle();

        virtual void OnInit(const izanagi::sys::WindowHandle& handle);

        void (*funcInit)(const izanagi::sys::WindowHandle&, CSampleApp*);

        virtual void OnTerminate();

        void (*funcTerminate)(CSampleApp*);

        virtual void OnDestroy();

        void (*funcDestroy)();

    private:
        CSampleApp* m_App;

        izanagi::CIntPoint m_PrevPoint;

        struct {
            IZ_UINT onLBtn    : 1;
            IZ_UINT onRBtn    : 1;
        } m_Flags;
    };
}
}

#endif    // #if !defined(__IZ_SAMPLE_SAMPLE_WINDOW_H__)
