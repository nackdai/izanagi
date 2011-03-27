#if !defined(__MODEL_LIB_XFILE_MATERIAL_H__)
#define __MODEL_LIB_XFILE_MATERIAL_H__

#include <vector>
#include "XFileHelper.h"
#include "XFileParser.h"

class CXFileMaterial {
	static CXFileMaterial s_cInstance;

public:
	static CXFileMaterial& GetInstance() { return s_cInstance; }

private:
	CXFileMaterial();
	~CXFileMaterial() {}

public:
	void SetNameForInvalidMaterial(IZ_PCSTR pszName);

	IZ_BOOL Begin(const CXFileParser* pParser);

	IZ_UINT GetMaterialNum();

	IZ_BOOL GetMaterial(
		IZ_UINT nMtrlIdx,
		izanagi::S_MTRL_MATERIAL& sMtrl);

	void GetMaterialTexture(
		IZ_UINT nMtrlIdx,
		IZ_UINT nTexIdx,
		izanagi::S_MTRL_TEXTURE& sTex);

	void GetMaterialShader(
		IZ_UINT nMtrlIdx,
		IZ_UINT nShaderIdx,
		izanagi::S_MTRL_SHADER& sShader);

	void GetMaterialParam(
		IZ_UINT nMtrlIdx,
		IZ_UINT nParamIdx,
		izanagi::S_MTRL_PARAM& sParam);

	void GetMaterialParamValue(
		IZ_UINT nMtrlIdx,
		IZ_UINT nParamIdx,
		std::vector<IZ_FLOAT>& tvValue);

private:
	std::vector<SXFileMaterial> m_MtrlList;

	std::string m_NameForInvalidMtrl;
	IZ_UINT m_nInvalidMtrlCount;
};

#endif	// #if !defined(__MODEL_LIB_XFILE_MATERIAL_H__)
