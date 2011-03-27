// PostEffect.h : PostEffect アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"       // メイン シンボル


// CPostEffectApp:
// このクラスの実装については、PostEffect.cpp を参照してください。
//

class CPostEffectApp : public CWinApp
{
public:
	CPostEffectApp();


// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnIdle(LONG lCount);
};

extern CPostEffectApp theApp;