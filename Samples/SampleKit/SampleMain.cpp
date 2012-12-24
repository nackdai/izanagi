#include "SampleMain.h"
#include "SampleWindowProc.h"
#include "SampleApp.h"

namespace izanagi {
namespace sample {
    /**
     */
    struct SSampleParam {
        izanagi::sample::CSampleApp* app;
        izanagi::IMemoryAllocator* allocator;
        izanagi::IMemoryAllocator* allocatorForGraph;
        IZ_UINT width;
        IZ_UINT height;
        IZ_BOOL isWindowed;
        IZ_PCSTR title;
        void* platformParam;
    };
}
}

IZ_BOOL SampleMainLoop(izanagi::sample::SSampleParam& params)
{
    IZ_BOOL ret = IZ_TRUE;

    // 実行ファイルから現在のパスを取得
    izanagi::sys::CSysUtil::SetCurrentDirectoryFromExe();

    izanagi::sample::CSampleWndProc wndProc;
    void* nativeWndHandle = IZ_NULL;

    // ウインドウ作成用パラメータ
    izanagi::sys::WindowParams wndParam = {
        params.width,
        params.height,
        params.isWindowed,
        params.title,
        params.platformParam,
        &wndProc,
    };

    // アプリ初期化用パラメータ
    izanagi::sample::SSampleAppParams appParam = {
        params.allocator,
        params.allocatorForGraph,
        params.width,
        params.height,
        &wndProc,
        IZ_NULL,    // 後で設定する
        IZ_NULL,    // 後で設定する
        params.platformParam,
    };

    // ウインドウ作成
    izanagi::sys::WindowHandle wndHandle = izanagi::sys::CSysWindow::Create(
                                        params.allocator,
                                        wndParam);
    VGOTO(ret = (wndHandle != IZ_NULL), __EXIT__);

    // プラットフォームごとのウインドウハンドルを取得
    nativeWndHandle = izanagi::sys::CSysWindow::GetNativeWindowHandle(wndHandle);
    VGOTO(ret = (nativeWndHandle != IZ_NULL), __EXIT__);

    appParam.deviceWindow = nativeWndHandle;
    appParam.focusWindow = nativeWndHandle;

    // アプリ初期化
    ret = params.app->Init(appParam);
    VGOTO(ret, __EXIT__);

    izanagi::sys::CSysWindow::RunLoop(wndHandle);

__EXIT__:
    params.app->Release();

    izanagi::sys::CSysWindow::Destroy(wndHandle);

    params.allocator->Dump();

    return ret;
}

IZ_INT SampleMain(
    void* systemData,
    izanagi::sample::CSampleApp* app,
    const char* title,
    IZ_UINT screenWidth, IZ_UINT screenHeight,
    void* allocatorBuf, IZ_UINT bufSize,
    void* graphBuf, IZ_UINT graphBufSize)
{
    izanagi::CStandardMemoryAllocator allocator(bufSize, allocatorBuf);
    izanagi::CStandardMemoryAllocator allocatorForGraph(graphBufSize, graphBuf);

    izanagi::sample::SSampleParam sampleParam = {
        app,
        &allocator,
        &allocatorForGraph,
        screenWidth,
        screenHeight,
        IZ_TRUE,
        title,
        systemData,
    };

    IZ_BOOL result = SampleMainLoop(sampleParam);

    int ret = (result ? 0 : 1);

    return ret;
}