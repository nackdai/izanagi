#if !defined(__SHADER_CONVERTER_COMPILE_CMD_CREATEOR_H__)
#define __SHADER_CONVERTER_COMPILE_CMD_CREATEOR_H__

#include "Cg/cg.h"
#include "izToolKit.h"
#include "ShaderConfig.h"

/**
* シェーダコンパイルタイプ
*/
enum COMPILE_TYPE {
    COMPILE_TYPE_SHADER = 0,    // 通常
    COMPILE_TYPE_SHADER_ASM,    // アセンブラ出力

    COMPILE_TYPE_NUM,
};

/**
* シェーダコンパイル用コマンド作成
*/
class CCompileCmdCreator {
public:
    static CCompileCmdCreator& GetInstance(izanagi::E_PLATFORM type);

protected:
    CCompileCmdCreator() { m_bIsVS = IZ_FALSE; }
    virtual ~CCompileCmdCreator() {}

    NO_COPIABLE(CCompileCmdCreator);

public:
    // コマンド作成
    virtual void CreateCompileCommand(
        izanagi::tool::CString& cmd,
        izanagi::tool::CString& out,
        COMPILE_TYPE type,
        const SShaderConfig& config,
        IZ_PCSTR entry,
        CGprofile profile) = 0;

    /**
    * 頂点シェーダかどうかセット
    */
    void SetIsVS(BOOL flag)
    {
        m_bIsVS = flag;
    }

    /**
    * 頂点シェーダかどうか
    */
    BOOL IsVS()
    {
        return m_bIsVS;
    }

private:
    // 頂点シェーダかどうか
    BOOL m_bIsVS;
};

#endif  // #if !defined(__SHADER_CONVERTER_COMPILE_CMD_CREATEOR_H__)
