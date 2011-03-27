// ShaderTest.h : ShaderTest アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"       // メイン シンボル


// CDebugMeshApp:
// このクラスの実装については、ShaderTest.cpp を参照してください。
//

class CDebugMeshApp : public CWinApp
{
public:
	CDebugMeshApp();


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

extern CDebugMeshApp theApp;