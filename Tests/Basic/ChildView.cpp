// ChildView.cpp : CChildView クラスの実装
//

#include "stdafx.h"
#include "Basic.h"
#include "ChildView.h"
#include ".\childview.h"

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
	ON_WM_SIZE()
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
	
	CMyAppl::GetInstance().Render();

	CMySystem::GetInstance().GetGraphicsDevice()->Present();
	
	// メッセージの描画のために CWnd::OnPaint() を呼び出さないでください。
}

BOOL CChildView::Init()
{
	// 初期化
	BOOL ret = CMyAppl::GetInstance().Init(
				::AfxGetInstanceHandle(),
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

void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	RECT rc;
	GetClientRect(&rc);

	izanagi::SGraphicsDeviceInitParams sParams;
	sParams.BackBufferWidth = rc.right - rc.left;
	sParams.BackBufferHeight = rc.bottom - rc.top;

	if ((sParams.BackBufferWidth > 0) && (sParams.BackBufferHeight > 0)) {
		CMySystem::GetInstance().GetGraphicsDevice()->Reset(sParams);

		izanagi::SViewport vp;
		{
			vp.x = 0;
			vp.y = 0;
			vp.width = sParams.BackBufferWidth;
			vp.height = sParams.BackBufferHeight;
			vp.minZ = 0.0f;
			vp.maxZ = 1.0f;
		}
		CMySystem::GetInstance().GetGraphicsDevice()->SetViewport(vp);
	}
}
