#if !defined(__POSTEFFECT_CONVERTER_COMPILE_CMD_CREATEOR_H__)
#define __POSTEFFECT_CONVERTER_COMPILE_CMD_CREATEOR_H__

#include "Cg/cg.h"
#include "izToolKit.h"

/**
* シェーダコンパイルタイプ
*/
enum COMPILE_TYPE {
	COMPILE_TYPE_SHADER = 0,	// 通常
	COMPILE_TYPE_SHADER_ASM,	// アセンブラ出力

	COMPILE_TYPE_NUM,
};

/**
* シェーダコンパイル用コマンド作成
*/
class CCompileCmdCreator {
private:
	static CCompileCmdCreator s_cInstance;

public:
	static CCompileCmdCreator& GetInstance() { return s_cInstance; }

private:
	CCompileCmdCreator();
	~CCompileCmdCreator();

	CCompileCmdCreator(const CCompileCmdCreator& rhs);
	const CCompileCmdCreator& operator=(const CCompileCmdCreator& rhs);

public:
	// コマンド作成
	void CreateCompileCommand(
		izanagi::tool::CString& cmd,
		izanagi::tool::CString& out,
		COMPILE_TYPE type,
		LPCSTR lpszCompileCommand,
		LPCSTR lpszShaderFile,
		LPCSTR lpszEntryPoint,
		CGprofile profile);

public:
	inline void SetIsVS(BOOL flag);

private:
	inline BOOL IsVS();

private:
	// 頂点シェーダかどうか
	BOOL m_bIsVS;
};

// inline *******************************

/**
* 頂点シェーダかどうかセット
*/
void CCompileCmdCreator::SetIsVS(BOOL flag)
{
	m_bIsVS = flag;
}

/**
* 頂点シェーダかどうか
*/
BOOL CCompileCmdCreator::IsVS()
{
	return m_bIsVS;
}

#endif	// #if !defined(__POSTEFFECT_CONVERTER_COMPILE_CMD_CREATEOR_H__)
