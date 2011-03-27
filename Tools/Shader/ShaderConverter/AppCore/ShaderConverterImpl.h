#if !defined(__SHADER_CONVERTER_POSTEFFECT_CONVERTER_IMPL_H_)
#define __SHADER_CONVERTER_POSTEFFECT_CONVERTER_IMPL_H_

#include <vector>
#include <Cg/cg.h>
#include "izToolkit.h"
#include "izShader.h"

class CCgParameterHolderBase;
class CCgTechniqueHolder;
struct SShaderConfig;

/**
* コンバータ本体
*/
class CShaderConverter {
private:
	static CShaderConverter s_cInstance;

public:
	static CShaderConverter& GetInstance() { return s_cInstance; }

private:
	CShaderConverter();
	~CShaderConverter();

	CShaderConverter(const CShaderConverter& rhs);
	const CShaderConverter& operator=(const CShaderConverter& rhs);

public:
	BOOL Begin(LPCSTR lpszShaderFile);

	BOOL Export(const SShaderConfig& sConfig);

	// シェーダコンパイル
	BOOL CompileShader(
		BOOL bIsAsm,
		LPCSTR lpszCompileCommand,
		LPCSTR lpszShaderFile,
		LPCSTR lpszObjDir);

protected:
	template <typename _T>
	BOOL ExportData(const _T& data);

	BOOL ExportTechnique();
	BOOL ExportTexture();
	BOOL ExportSampler();
	BOOL ExportParameter();
	BOOL ExportPass();

	BOOL ExportParamAnn(
		izanagi::S_SHD_PARAM_HEADER& sParamHeader,
		IZ_INT nAnnNum = -1);

#if 0
	BOOL DoNotRemoveParam(CGparameter param);
#endif

	BOOL ExportUsedParamAndSamplerIdxByPass();
	BOOL ExportUsedParamAndSamplerIdxByPass(CGpass pass);

	BOOL ExportProgram();

	BOOL ExportStringBuffer();

	BOOL CompileProgram(
		BOOL bIsVS,
		BOOL bIsAsm,
		LPCSTR lpszCompileCommand,
		LPCSTR lpszShaderFile,
		LPCSTR lpszObjDir);

private:
	CGcontext m_pCgContext;
	CGeffect m_pCgEffect;

	izanagi::S_SHD_HEADER m_ShdHeader;

	std::vector<CGparameter> m_TexList;
	std::vector<CGparameter> m_SamplerList;
	std::vector<CGparameter> m_ParamList;

	izanagi::CFileOutputStream m_Out;

	std::vector<CString> m_CompiledVSList;
	std::vector<CString> m_CompiledPSList;
};

#endif	// #if !defined(__SHADER_CONVERTER_POSTEFFECT_CONVERTER_IMPL_H_)
