#if !defined(__IZ_SAMPLE_SAMPLE_APP_H__)
#define __IZ_SAMPLE_SAMPLE_APP_H__

#include <izDefs.h>
#include <izStd.h>
#include <izGraph.h>
#include <izSystem.h>
#include <izDebugUtil.h>
#include <izIo.h>

#include "SampleCamera.h"

namespace izanagi {
namespace sample {
    class CSampleWndProc;

    enum {
        SAMPLE_TIMER_NUM = 2,
    };

    /** サンプルアプリケーション初期化パラメータ
     */
    struct SSampleAppParams {
        izanagi::IMemoryAllocator* allocator;            ///< メモリアロケータ

        izanagi::IMemoryAllocator* allocatorForGraph;    ///< グラフィックスデバイス用メモリアロケータ

        IZ_UINT screenWidth;                    ///< スクリーン幅
        IZ_UINT screenHeight;                   ///< スクリーン高さ

        CSampleWndProc* wndProc;                ///< メッセージハンドラへのポインタ

        // ここから下はプラットフォーム依存
        void* deviceWindow;                     ///< デバイスウインドウハンドル
        void* focusWindow;                      ///< フォーカスウインドウハンドル（基本的にはデバイスウインドウハンドルと同じ）
        void* instanceHandle;                   ///< インスタンスハンドル
    };

    /** サンプルアプリケーションベースクラス.
     */
    class CSampleApp {
    public:
        CSampleApp();
        virtual ~CSampleApp() = 0;

        NO_COPIABLE(CSampleApp);

    public:
        /** 初期化.
         */
        IZ_BOOL Init(const SSampleAppParams& params);

        /** 解放.
         */
        void Release();

        /** 更新.
         */
        void Update();

        /** 描画.
         */
        void Render();

        /** V同期.
         */
        void Present();

        /** スクリーン幅取得
         */
        IZ_UINT GetScreenWidth() const;

        /** スクリーン高さ取得
         */
        IZ_UINT GetScreenHeight() const;

        /** タイマ取得.
         */
        izanagi::sys::CTimer& GetTimer(IZ_UINT idx);

        /** カメラ取得.
         */
        CSampleCamera& GetCamera();

        /** デバッグフォント取得
         */
        izanagi::CDebugFont* GetDebugFont() { return m_DebugFont; }

        /** キーボード押下
         */
        virtual IZ_BOOL OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key) { return IZ_TRUE; }

        /** キーボードアップ
         */
        virtual void OnKeyUp(izanagi::sys::E_KEYBOARD_BUTTON key) {}

        /** マウス左ボタン押下
         */
        virtual IZ_BOOL OnMouseLBtnDown(const CIntPoint& point) { return IZ_TRUE; }



    protected:
        /** 背景色取得.
         */
        virtual IZ_COLOR GetBgColor() const;

        /** 初期化.
         */
        virtual IZ_BOOL InitInternal(
            izanagi::IMemoryAllocator* allocator,
            izanagi::graph::CGraphicsDevice* device,
            izanagi::sample::CSampleCamera& camera) = 0;

        /** 解放.
         */
        virtual void ReleaseInternal() = 0;

        /** 更新.
         */
        virtual void UpdateInternal(izanagi::graph::CGraphicsDevice* device) = 0;

        /** 描画.
         */
        virtual void RenderInternal(izanagi::graph::CGraphicsDevice* device) = 0;

        izanagi::sys::CPad* GetPad()
        {
            return m_Pad;
        }

    protected:
        izanagi::graph::CGraphicsDevice* m_Device;
        izanagi::IMemoryAllocator* m_Allocator;

        izanagi::CDebugFont* m_DebugFont;

        izanagi::sys::CPad* m_Pad;

        IZ_UINT m_ScreenWidth;
        IZ_UINT m_ScreenHeight;

        izanagi::sys::CTimer m_Timer[SAMPLE_TIMER_NUM];

        CSampleCamera m_Camera;
    };
}
}

#endif    // #if !defined(__IZ_SAMPLE_SAMPLE_APP_H__)
