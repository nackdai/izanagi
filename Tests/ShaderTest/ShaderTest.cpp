// ShaderTest.cpp : アプリケーションのクラス動作を定義します。
//

#include "stdafx.h"
#include "ShaderTest.h"
#include "MainFrm.h"

#include "MyAppl.h"
#include "MySystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDebugMeshApp

BEGIN_MESSAGE_MAP(CDebugMeshApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CDebugMeshApp::OnAppAbout)
END_MESSAGE_MAP()


// CDebugMeshApp コンストラクション

CDebugMeshApp::CDebugMeshApp()
{
	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}


// 唯一の CDebugMeshApp オブジェクトです。

CDebugMeshApp theApp;


// CDebugMeshApp 初期化

BOOL CDebugMeshApp::InitInstance()
{
	// アプリケーション マニフェストが visual スタイルを有効にするために、
	// ComCtl32.dll Version 6 以降の使用を指定する場合は、
	// Windows XP に InitCommonControlsEx() が必要です。さもなければ、ウィンドウ作成はすべて失敗します。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// アプリケーションで使用するすべてのコモン コントロール クラスを含めるには、
	// これを設定します。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// OLE ライブラリを初期化します。
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// 標準初期化
	// これらの機能を使わずに最終的な実行可能ファイルの
	// サイズを縮小したい場合は、以下から不要な初期化
	// ルーチンを削除してください。
	// 設定が格納されているレジストリ キーを変更します。
	// TODO: 会社名または組織名などの適切な文字列に
	// この文字列を変更してください。
	SetRegistryKey(_T("アプリケーション ウィザードで生成されたローカル アプリケーション"));
	// メイン ウィンドウを作成するとき、このコードは新しいフレーム ウィンドウ オブジェクトを作成し、
	// それをアプリケーションのメイン ウィンドウにセットします
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// フレームをリソースからロードして作成します
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);

	//////////////////////////////////////////////////
	// 初期化
	if (!pFrame->GetChildView()->Init()) {
			ASSERT(FALSE);
			return FALSE;
	}
	//////////////////////////////////////////////////

	// メイン ウィンドウが初期化されたので、表示と更新を行います。
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	// 接尾辞が存在する場合にのみ DragAcceptFiles を呼び出してください。
	//  SDI アプリケーションでは、ProcessShellCommand の直後にこの呼び出しが発生しなければなりません。
	return TRUE;
}


// CDebugMeshApp メッセージ ハンドラ




// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ダイアログ データ
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// ダイアログを実行するためのアプリケーション コマンド
void CDebugMeshApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CDebugMeshApp メッセージ ハンドラ


BOOL CDebugMeshApp::OnIdle(LONG lCount)
{
	CMySystem::GetInstance().GetTimer(0).Begin();
	CMySystem::GetInstance().GetTimer(1).Begin();

	CMyAppl::GetInstance().Update();
	CMyAppl::GetInstance().Render();

	CMySystem::GetInstance().GetTimer(1).End();

	CMySystem::GetInstance().GetGraphicsDevice()->Present();

	CMySystem::GetInstance().GetTimer(0).End();

	//return CWinApp::OnIdle(lCount);
	CWinApp::OnIdle(lCount);
	return TRUE;
}
