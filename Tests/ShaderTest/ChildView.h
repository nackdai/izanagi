// ChildView.h : CChildView クラスのインターフェイス
//


#pragma once


// CChildView ウィンドウ

class CChildView : public CWnd
{
// コンストラクション
public:
	CChildView();

// 属性
public:

// 操作
public:

// オーバーライド
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	BOOL Init();

protected:
	CPoint m_ptPrevPoint;

// 実装
public:
	virtual ~CChildView();

	// 生成された、メッセージ割り当て関数
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
