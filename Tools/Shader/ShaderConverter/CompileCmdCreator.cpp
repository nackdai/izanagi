#include <vector>
#include "shlwapi.h"
#include "CompileCmdCreator.h"
#include "izToolKit.h"

// 拡張子
static const char* COMPILED_PS_EXT = ".pso";
static const char* COMPILED_VS_EXT = ".vso";
static const char* COMPILED_ASM_PS_EXT = ".psh";
static const char* COMPILED_ASM_VS_EXT = ".vsh";

class CCompileCmdCreator_DX9 : CCompileCmdCreator {
public:
    CCompileCmdCreator_DX9() {}
    virtual ~CCompileCmdCreator_DX9() {}

    // コマンド作成
    virtual void CreateCompileCommand(
        izanagi::tool::CString& cmd,
        izanagi::tool::CString& out,
        COMPILE_TYPE type,
        const SShaderConfig& config,
        IZ_PCSTR entry,
        CGprofile profile);
};

/**
* コマンド作成
*/
void CCompileCmdCreator_DX9::CreateCompileCommand(
    izanagi::tool::CString& cmd,
    izanagi::tool::CString& out,
    COMPILE_TYPE type,
    const SShaderConfig& config,
    IZ_PCSTR entry,
    CGprofile profile)
{
    // 拡張子
    LPCSTR ext = NULL;
    if (type == COMPILE_TYPE_SHADER_ASM) {
        ext = (IsVS() ? COMPILED_ASM_VS_EXT : COMPILED_ASM_PS_EXT);
    }
    else if (type == COMPILE_TYPE_SHADER) {
        ext = (IsVS() ? COMPILED_VS_EXT : COMPILED_PS_EXT);
    }

    if (!out.empty()) {
        // 出力ファイルに拡張子をつける
        out += ext;
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
            ? "\"%s\" %s /Fh %s /E %s /T %s %s"
            : "\"%s\" %s /Fo %s /E %s /T %s %s",
        config.compiler.c_str(),
        config.compile_opt.empty() ? "" : config.compile_opt.c_str(),
        out.c_str(),
        entry,
        strProfile.c_str(),
        config.preproc_file.c_str());
}

class CCompileCmdCreator_GLES2 : CCompileCmdCreator {
public:
    CCompileCmdCreator_GLES2() {}
    virtual ~CCompileCmdCreator_GLES2() {}

    // コマンド作成
    virtual void CreateCompileCommand(
        izanagi::tool::CString& cmd,
        izanagi::tool::CString& out,
        COMPILE_TYPE type,
        const SShaderConfig& config,
        IZ_PCSTR entry,
        CGprofile profile);
};

/**
* コマンド作成
*/
void CCompileCmdCreator_GLES2::CreateCompileCommand(
    izanagi::tool::CString& cmd,
    izanagi::tool::CString& out,
    COMPILE_TYPE type,
    const SShaderConfig& config,
    IZ_PCSTR entry,
    CGprofile profile)
{
    // 拡張子
    LPCSTR ext = NULL;
    if (type == COMPILE_TYPE_SHADER_ASM) {
        ext = (IsVS() ? COMPILED_ASM_VS_EXT : COMPILED_ASM_PS_EXT);
    }
    else if (type == COMPILE_TYPE_SHADER) {
        ext = (IsVS() ? COMPILED_VS_EXT : COMPILED_PS_EXT);
    }

    if (!out.empty()) {
        // 出力ファイルに拡張子をつける
        out += ext;
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
            ? "\"%s\" -i %s -o %s -p %s -e %s"
            : "\"%s\" -i %s -o %s -p %s -e %s",
        config.compiler.c_str(),
        config.preproc_file.c_str(),
        out.c_str(),
        strProfile.c_str(),
        entry);

    // nativeのコンパイラに渡すコンパイルオプション
    if (!config.compile_opt.empty())
    {
        cmd += " -opt ";
        cmd += config.compile_opt;
    }

    printf("%s\n", cmd.c_str());
}

CCompileCmdCreator_DX9 s_InstanceDX9;
CCompileCmdCreator_GLES2 s_InstanceGLES2;

CCompileCmdCreator& CCompileCmdCreator::GetInstance(izanagi::E_PLATFORM type)
{
    CCompileCmdCreator* ret = IZ_NULL;

    switch (type)
    {
    case izanagi::E_PLATFORM_DX9:
        ret = (CCompileCmdCreator*)&s_InstanceDX9;
        break;
    case izanagi::E_PLATFORM_GLES2:
        ret = (CCompileCmdCreator*)&s_InstanceGLES2;
        break;
    default:
        IZ_ASSERT(IZ_FALSE);
        break;
    }

    return *ret;
}
