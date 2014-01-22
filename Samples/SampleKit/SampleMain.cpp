#include "SampleMain.h"
#include "SampleWindowProc.h"
#include "SampleApp.h"

// アプリ初期化用パラメータ
static izanagi::sample::SSampleAppParams appParam = {0};

static izanagi::CStandardMemoryAllocator allocator;
static izanagi::CStandardMemoryAllocator allocatorForGraph;

static void OnInit(
    const izanagi::sys::WindowHandle& handle,
    izanagi::sample::CSampleApp* app)
{
    app->Init(handle, appParam);
}

static void OnTerminate(izanagi::sample::CSampleApp* app)
{
    app->Release();
}

static void OnDestroy()
{
    allocator.Dump();
}

IZ_INT SampleMain(
    void* systemData,
    izanagi::sample::CSampleApp* app,
    const char* title,
    IZ_UINT screenWidth, IZ_UINT screenHeight,
    void* allocatorBuf, IZ_UINT bufSize,
    void* graphBuf, IZ_UINT graphBufSize)
{
    allocator.Init(bufSize, allocatorBuf);
    allocatorForGraph.Init(graphBufSize, graphBuf);

    // 実行ファイルから現在のパスを取得
    izanagi::sys::CSysUtil::SetCurrentDirectoryFromExe();

    izanagi::sample::CSampleWndProc wndProc(app);
    wndProc.funcInit = OnInit;
    wndProc.funcTerminate = OnTerminate;
    wndProc.funcDestroy = OnDestroy;

    char* argv[] = {
        const_cast<char*>(izanagi::sys::CSysUtil::GetExecuteFilePath()),
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

    izanagi::sys::CSysWindow::RunLoop(wndHandle);

__EXIT__:
    int ret = (result ? 0 : 1);

    return ret;
}