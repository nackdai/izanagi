﻿// Basic.h : Basic アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error このファイルを PCH に含める前に、'stdafx.h' を含めてください。
#endif

#include "resource.h"       // メイン シンボル


// CBasicApp:
// このクラスの実装については、Basic.cpp を参照してください。
//

class CBasicApp : public CWinApp
{
public:
	CBasicApp();


// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern CBasicApp theApp;
