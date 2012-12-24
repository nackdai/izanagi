#include <vector>
#include "shlwapi.h"
#include "CompileCmdCreator.h"
#include "izToolKit.h"

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
    izanagi::tool::CString& cmd,
    izanagi::tool::CString& out,
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
    izanagi::tool::CString strProfile(IsVS() ? "vs_2_a" : "ps_2_a");
    if (profile != CG_PROFILE_GENERIC) {
        strProfile.format("%s", cgGetProfileString(profile));
    }

    // アセンブラ出力するかどうか
    BOOL bIsAsm = (type == COMPILE_TYPE_SHADER_ASM);

    cmd.format(
        bIsAsm
            ? "%s /Fh %s%s /E %s /T %s %s"
            : "%s /Fo %s%s /E %s /T %s %s",
        lpszCompileCommand,
        out.c_str(),
        lpszExt,
        lpszEntryPoint,
        strProfile.c_str(),
        lpszShaderFile);

    if (!out.empty()) {
        // 出力ファイルに拡張子をつける
        out += lpszExt;
    }
}
