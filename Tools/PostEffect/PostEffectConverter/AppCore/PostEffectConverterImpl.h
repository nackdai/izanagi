#if !defined(__POSTEFFECT_CONVERTER_POSTEFFECT_CONVERTER_IMPL_H_)
#define __POSTEFFECT_CONVERTER_POSTEFFECT_CONVERTER_IMPL_H_

#include <vector>
#include <Cg/cg.h>
#include "izToolkit.h"
#include "izPostEffect.h"

class CCgParameterHolderBase;
class CCgTechniqueHolder;

/**
* コンバータ本体
*/
class CPostEffectConverter {
private:
	static CPostEffectConverter s_cInstance;

public:
	static CPostEffectConverter& GetInstance() { return s_cInstance; }

private:
	CPostEffectConverter();
	~CPostEffectConverter();

	CPostEffectConverter(const CPostEffectConverter& rhs);
	const CPostEffectConverter& operator=(const CPostEffectConverter& rhs);

public:
	BOOL Begin(LPCSTR lpszShaderFile);

	BOOL Export(LPCSTR lpszOutFile);

	// シェーダコンパイル
	BOOL CompileShader(
		BOOL bIsAsm,
		LPCSTR lpszCompileCommand,
		LPCSTR lpszShaderFile,
		LPCSTR lpszObjDir);

protected:
	template <typename _T>
	BOOL ExportData(const _T& data);

	void SetPesId();

	BOOL ExportTechnique();
	BOOL ExportTexture();
	BOOL ExportSampler();
	BOOL ExportParameter();
	BOOL ExportPass();

	BOOL ExportParamAnn(IZ_INT nAnnNum = -1);

	BOOL DoNotRemoveParam(CGparameter param);

	void ConvertFunctorArgsStrToIndex(
		const std::vector<izanagi::tool::CString>& tvFunctorArgSList,
		izanagi::S_PES_PASS_ANN& sParamAnn);

	BOOL ExportUsedParamAndSamplerIdxByPass();
	BOOL ExportUsedParamAndSamplerIdxByPass(CGpass pass);

	BOOL ExportPSProgram();
	BOOL ExportVSProgram();

	BOOL ExportStringBuffer();

	// ピクセルプログラムをコンパイル
	BOOL CompilePixelProgram(
		BOOL bIsAsm,
		LPCSTR lpszCompileCommand,
		LPCSTR lpszShaderFile,
		LPCSTR lpszObjDir);

	// 頂点プログラムをコンパイル
	BOOL CompileVertexProgram(
		BOOL bIsAsm,
		LPCSTR lpszCompileCommand,
		LPCSTR lpszShaderFile,
		LPCSTR lpszObjDir);

private:
	CGcontext m_pCgContext;
	CGeffect m_pCgEffect;

	izanagi::S_PES_HEADER m_PesHeader;

	std::vector<CGparameter> m_TexList;
	std::vector<CGparameter> m_SamplerList;
	std::vector<CGparameter> m_ParamList;

	izanagi::CFileOutputStream m_Out;

	std::vector<izanagi::tool::CString> m_CompiledPSList;

	struct SVSInfo {
		izanagi::tool::CString file;
		izanagi::E_POSTEFFECT_VTX_SHADER type;
	};

	std::vector<SVSInfo> m_CompiledVSList;
};

#endif	// #if !defined(__POSTEFFECT_CONVERTER_POSTEFFECT_CONVERTER_IMPL_H_)
