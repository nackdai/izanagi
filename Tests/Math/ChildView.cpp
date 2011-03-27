// ChildView.cpp : CChildView クラスの実装
//

#include "stdafx.h"
#include "Math.h"
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
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
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

	if (CMySystem::GetInstance().GetGraphicsDevice() != IZ_NULL) {
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
}

void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	CWnd::OnMouseMove(nFlags, point);
}

BOOL CChildView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}
