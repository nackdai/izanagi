// ChildView.cpp : CChildView クラスの実装
//

#include "stdafx.h"
#include "Shader.h"
#include "ChildView.h"

#include "MyAppl.h"
#include "MySystem.h"
#include "MyCamera.h"
#include "state/StateManager.h"

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
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
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

void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (GetAsyncKeyState(VK_LBUTTON) & 0xff00) {
		float fLatitude = (float)(m_ptPrevPoint.y - point.y);
		fLatitude *= 0.005f;

		float fLongitude = (float)(m_ptPrevPoint.x - point.x);
		fLongitude *= 0.005f;

		CMyCamera::GetInstance().Rotate(fLatitude, fLongitude);
		CMyCamera::GetInstance().UpdateCamera();
	}
	else if (GetAsyncKeyState(VK_RBUTTON) & 0xff00) {
		float fOffsetX = (float)(m_ptPrevPoint.x - point.x);
		fOffsetX *= 0.005f;

		float fOffsetY = (float)(m_ptPrevPoint.y - point.y);
		fOffsetY *= 0.005f;

		CMyCamera::GetInstance().Move(fOffsetX, fOffsetY);
		CMyCamera::GetInstance().UpdateCamera();
	}

	m_ptPrevPoint = point;

	CWnd::OnMouseMove(nFlags, point);
}

BOOL CChildView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CMyCamera::GetInstance().Dolly(zDelta * 0.05f);
	CMyCamera::GetInstance().UpdateCamera();

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CStateManager::GetInstance().OnKeyDown(nChar, nRepCnt, nFlags);

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

#if 0
	RECT rc;
	GetClientRect(&rc);

	izanagi::SGraphicsDeviceInitParams sParams;
	sParams.BackBufferWidth = rc.right - rc.left;
	sParams.BackBufferHeight = rc.bottom - rc.top;

	if ((sParams.BackBufferWidth > 0) && (sParams.BackBufferHeight > 0)) {
		CMyAppl::GetInstance().Reset();
		CMySystem::GetInstance().GetGraphicsDevice()->Reset(sParams);

		// NOTE
		// Reset内部でビューポートをリセットしているので、ここではやらない

		CMyCamera::GetInstance().GetRawInterface().GetParam().aspect = (IZ_FLOAT)sParams.BackBufferHeight / sParams.BackBufferWidth;
		CMyCamera::GetInstance().UpdateCamera();
	}
#endif
}
