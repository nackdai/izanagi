#include "SampleMain.h"
#include "SampleWindow.h"
#include "SampleApp.h"

IZ_BOOL SampleMainLoop(izanagi::sample::SSampleParam& params)
{
	IZ_BOOL ret = IZ_TRUE;

	// 実行ファイルから現在のパスを取得
	izanagi::CSysUtil::SetCurrentDirectoryFromExe();

	izanagi::sample::CSampleWndProc wndProc;
	void* nativeWndHandle = IZ_NULL;

	// ウインドウ作成用パラメータ
	izanagi::WindowParams wndParam = {
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
		0,			// TODO
		IZ_NULL,	// TODO
		params.width,
		params.height,
		&wndProc,
		nativeWndHandle,
		nativeWndHandle,
		params.platformParam,
	};

	// ウインドウ作成
	izanagi::WindowHandle wndHandle = izanagi::CSysWindow::Create(
										params.allocator,
										wndParam);
	VGOTO(ret = (wndHandle != IZ_NULL), __EXIT__);

	// プラットフォームごとのウインドウハンドルを取得
	nativeWndHandle = izanagi::CSysWindow::GetNativeWindowHandle(wndHandle);
	VGOTO(ret = (nativeWndHandle != IZ_NULL), __EXIT__);

	// アプリ初期化
	ret = params.app->Init(appParam);
	VGOTO(ret, __EXIT__);

	izanagi::CSysWindow::RunLoop(wndHandle);

__EXIT__:
	params.app->Release();

	izanagi::CSysWindow::Destroy(wndHandle);

	return ret;
}
