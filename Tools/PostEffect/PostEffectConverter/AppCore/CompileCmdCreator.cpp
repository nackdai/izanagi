#include <stdafx.h>
#include <vector>
#include "shlwapi.h"
#include "CompileCmdCreator.h"

CCompileCmdCreator CCompileCmdCreator::s_cInstance;

/**
* コンストラクタ
*/
CCompileCmdCreator::CCompileCmdCreator()
{
	m_bIsVS = FALSE;
}

/**
* デストラクタ
*/
CCompileCmdCreator::~CCompileCmdCreator()
{
}

// 拡張子
static const char* COMPILED_PS_EXT = ".pso";
static const char* COMPILED_VS_EXT = ".vso";
static const char* COMPILED_ASM_PS_EXT = ".psh";
static const char* COMPILED_ASM_VS_EXT = ".vsh";

/**
* コマンド作成
*/
void CCompileCmdCreator::CreateCompileCommand(
	CString& cmd,
	CString& out,
	COMPILE_TYPE type,
	LPCSTR lpszCompileCommand,
	LPCSTR lpszShaderFile,
	LPCSTR lpszEntryPoint,
	CGprofile profile)
{
	// 拡張子
	LPCSTR lpszExt = NULL;
	if (type == COMPILE_TYPE_SHADER_ASM) {
		lpszExt = (IsVS() ? COMPILED_ASM_VS_EXT : COMPILED_ASM_PS_EXT);
	}
	else if (type == COMPILE_TYPE_SHADER) {
		lpszExt = (IsVS() ? COMPILED_VS_EXT : COMPILED_PS_EXT);
	}

	// プロファイル設定
	CString strProfile(IsVS() ? "vs_2_a" : "ps_2_a");
	if (profile != CG_PROFILE_GENERIC) {
		strProfile.Format(_T("%s"), cgGetProfileString(profile));
	}

	// アセンブラ出力するかどうか
	BOOL bIsAsm = (type == COMPILE_TYPE_SHADER_ASM);

	cmd.Format(
		bIsAsm ? _T("%s /Fh %s%s /E %s /T %s %s") : _T("%s /Fo %s%s /E %s /T %s %s"),
		lpszCompileCommand,
		out, lpszExt,
		lpszEntryPoint,
		strProfile,
		lpszShaderFile);

	if (!out.IsEmpty()) {
		// 出力ファイルに拡張子をつける
		out += lpszExt;
	}
}
