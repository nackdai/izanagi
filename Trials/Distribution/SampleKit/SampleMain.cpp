#include "SampleMain.h"
#include "SampleWindowProc.h"
#include "SampleApp.h"

// アプリ初期化用パラメータ
static izanagi::sample::SSampleAppParams appParam = {0};

static izanagi::CStandardMemoryAllocator allocator;
static izanagi::CStandardMemoryAllocator allocatorForGraph;

static izanagi::sample::CSampleWndProc wndProc;

static void OnInit(
    const izanagi::sys::WindowHandle& handle,
    izanagi::sample::CSampleApp* app)
{
    app->Init(handle, appParam);
}

static void OnDestroy()
{
    allocator.Dump();
}

IZ_INT SampleMain(
    IZ_PLATFORM_PARAM systemData,
    izanagi::sample::CSampleApp* app,
    const char* title,
    IZ_UINT screenWidth, IZ_UINT screenHeight,
    void* allocatorBuf, IZ_UINT bufSize,
    void* graphBuf, IZ_UINT graphBufSize,
    IZ_BOOL canInternalLoop/*= IZ_TRUE*/)
{
    allocator.Init(bufSize, allocatorBuf);
    allocatorForGraph.Init(graphBufSize, graphBuf);

    // 実行ファイルから現在のパスを取得
    izanagi::sys::CSysUtil::SetCurrentDirectoryFromExe();

    wndProc.SetApp(app);
    wndProc.funcInit = OnInit;
    wndProc.funcDestroy = OnDestroy;

    char path[256];
    izanagi::sys::CSysUtil::GetExecuteFilePath(path, sizeof(path));

    char* argv[] = {
        path
    };

    // ウインドウ作成用パラメータ
    izanagi::sys::WindowParams wndParam = {
        1, 
        argv,
        screenWidth,
        screenHeight,
        IZ_TRUE,
        title,
        systemData,
        &wndProc,
    };

    // アプリ初期化用パラメータ
    appParam.allocator = &allocator;
    appParam.allocatorForGraph = &allocatorForGraph;
    appParam.screenWidth = screenWidth;
    appParam.screenHeight = screenHeight;
    appParam.wndProc = &wndProc;
    appParam.instanceHandle = systemData;

    IZ_BOOL result = IZ_TRUE;

    // ウインドウ作成
    izanagi::sys::WindowHandle wndHandle = izanagi::sys::CSysWindow::Create(
                                        &allocator,
                                        wndParam);
    VGOTO(result = (wndHandle != IZ_NULL), __EXIT__);

    if (canInternalLoop) {
        izanagi::sys::CSysWindow::RunLoop(wndHandle);
    }

__EXIT__:
    int ret = (result ? 0 : 1);

    return ret;
}