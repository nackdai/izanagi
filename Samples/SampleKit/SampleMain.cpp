#include "SampleMain.h"
#include "SampleWindowProc.h"
#include "SampleApp.h"

// アプリ初期化用パラメータ
izanagi::sample::SSampleAppParams appParam = {0};

static void OnInit(
    const izanagi::sys::WindowHandle& handle,
    izanagi::sample::CSampleApp* app)
{
    app->Init(handle, appParam);
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

    // 実行ファイルから現在のパスを取得
    izanagi::sys::CSysUtil::SetCurrentDirectoryFromExe();

    izanagi::sample::CSampleWndProc wndProc(app);
    wndProc.funcInit = OnInit;

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
    app->Release();

    izanagi::sys::CSysWindow::Destroy(wndHandle);

    allocator.Dump();

    int ret = (result ? 0 : 1);

    return ret;
}