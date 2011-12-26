#if !defined(__POSTEFFECT_CONVERTER_PREPROC_H__)
#define __POSTEFFECT_CONVERTER_PREPROC_H__

#include "izToolKit.h"

class COption;

/**
* プリプロセス処理を行う
*/
extern int Preproc(COption& cOption);

/**
* 自分自身をプリプロセス処理モードで呼び出す
*/
extern BOOL ExecWithPreprocMode(
	const char* lpszExe,
	COption& cOption);

#endif	// #if !defined(__POSTEFFECT_CONVERTER_PREPROC_H__)
