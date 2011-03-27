// ChildView.cpp : CChildView クラスの実装
//

#include "stdafx.h"
#include "Font.h"
#include "ChildView.h"

#include "MyAppl.h"
#include "MySystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CChildView メッセージ ハンドラ

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // 描画のデバイス コンテキスト
	
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	
	// メッセージの描画のために CWnd::OnPaint() を呼び出さないでください。
}

BOOL CChildView::Init()
{
	// 初期化
	BOOL ret = CMyAppl::GetInstance().Init(
				::AfxGetMainWnd()->GetSafeHwnd(),
				GetSafeHwnd());
	return ret;
}

void CChildView::OnDestroy()
{
	CWnd::OnDestroy();

	// 開放
	CMyAppl::GetInstance().Release();
}
