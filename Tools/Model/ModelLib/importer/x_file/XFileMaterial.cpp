#include "XFileMaterial.h"

CXFileMaterial* CXFileMaterial::s_pInstance = IZ_NULL;

CXFileMaterial& CXFileMaterial::GetInstance()
{
	if (s_pInstance == IZ_NULL) {
		s_pInstance = new CXFileMaterial();
	}
	return *s_pInstance;
}

void CXFileMaterial::DeleteInstance()
{
	SAFE_DELETE(s_pInstance);
}

CXFileMaterial::CXFileMaterial()
{
	m_nInvalidMtrlCount = 0;
}

void CXFileMaterial::SetNameForInvalidMaterial(IZ_PCSTR pszName)
{
	if (pszName != IZ_NULL) {
		m_NameForInvalidMtrl = pszName;
	}
}

IZ_BOOL CXFileMaterial::Begin(const CXFileParser* pParser)
{
	const std::vector<SXFileNode*>& tvNodeList = pParser->GetNodeList();
	
	for (size_t i = 0; i < tvNodeList.size(); i++) {
		const SXFileNode* pNode = tvNodeList[i];

		for (size_t m = 0; m < pNode->meshes.size(); m++) {
			const SXFileMesh* pMesh = pNode->meshes[m];

			for (size_t nMtrlIdx = 0; nMtrlIdx < pMesh->mtrls.size(); nMtrlIdx++) {
				const SXFileMaterial& sMtrl = pMesh->mtrls[nMtrlIdx];

				m_MtrlList.push_back(sMtrl);
			}
		}
	}

	return IZ_TRUE;
}

IZ_UINT CXFileMaterial::GetMaterialNum()
{
	size_t ret = m_MtrlList.size();
	return static_cast<IZ_UINT>(ret);
}

IZ_BOOL CXFileMaterial::GetMaterial(
	IZ_UINT nMtrlIdx,
	izanagi::S_MTRL_MATERIAL& sMtrl)
{
	IZ_ASSERT(nMtrlIdx < GetMaterialNum());

	const SXFileMaterial& sXfileMtrl = m_MtrlList[nMtrlIdx];

	if (sXfileMtrl.name.empty()) {
		// Material' name is invalid, so create new name.

		// Check whether base name is not empty.
		VRETURN(!m_NameForInvalidMtrl.empty());

		izChar tmp[16];
		sprintf_s(tmp, sizeof(tmp), "%d\0", m_nInvalidMtrlCount);

		std::string name(m_NameForInvalidMtrl);
		name += tmp;

		sMtrl.name.SetString(name.c_str());

		m_nInvalidMtrlCount++;
	}
	else {
		sMtrl.name.SetString(sXfileMtrl.name.c_str());
	}

	sMtrl.keyMaterial = sMtrl.name.GetKeyValue();

	sMtrl.numTex = static_cast<IZ_UINT16>(sXfileMtrl.textures.size());

	// NOTE
	// XFile's material does not have any shader,
	// so mtrl has a shader which is default.

	sMtrl.numShader = 1;

	// NOTE
	// XFile's material has fixed count of parameters,
	// so mtrl has same count of parameters.

	sMtrl.numParam = 4;

	for (IZ_UINT i = 0; i < sMtrl.numParam; i++) {
		izanagi::S_MTRL_PARAM sParam;
		GetMaterialParam(nMtrlIdx, i, sParam);

		sMtrl.paramBytes += sParam.bytes;
	}

	return IZ_TRUE;
}

void CXFileMaterial::GetMaterialTexture(
	IZ_UINT nMtrlIdx,
	IZ_UINT nTexIdx,
	izanagi::S_MTRL_TEXTURE& sTex)
{
	IZ_ASSERT(nMtrlIdx < GetMaterialNum());

	const SXFileMaterial& sXfileMtrl = m_MtrlList[nMtrlIdx];

	IZ_ASSERT(nTexIdx < sXfileMtrl.textures.size());

	const SXFileTexture& sXfileTex = sXfileMtrl.textures[nTexIdx];

	sTex.name.SetString(sXfileTex.name.c_str());
	sTex.key = sTex.name.GetKeyValue();

	// NOTE
	// XFile's texture is only albedo or normal map.
	sTex.type.flags = 0;
	sTex.type.isNormal = sXfileTex.isNormalMap;
}

void CXFileMaterial::GetMaterialShader(
	IZ_UINT nMtrlIdx,
	IZ_UINT nShaderIdx,
	izanagi::S_MTRL_SHADER& sShader)
{
	IZ_ASSERT(nMtrlIdx < GetMaterialNum());

	const SXFileMaterial& sXfileMtrl = m_MtrlList[nMtrlIdx];

	// NOTE
	// XFile's material does not have any shader.
	// so mtrl has a shader which has default name.

	sShader.name.SetString("DefaultShader");
	sShader.key = sShader.name.GetKeyValue();
}

void CXFileMaterial::GetMaterialParam(
	IZ_UINT nMtrlIdx,
	IZ_UINT nParamIdx,
	izanagi::S_MTRL_PARAM& sParam)
{
	IZ_ASSERT(nMtrlIdx < GetMaterialNum());

	const SXFileMaterial& sXfileMtrl = m_MtrlList[nMtrlIdx];

	switch (nParamIdx) {
	case 0:
		// diffuse
		sParam.name.SetString("diffuse");
		sParam.elements = 3;
		break;
	case 1:
		// specularExp
		sParam.name.SetString("specularExp");
		sParam.elements = 1;
		break;
	case 2:
		// specular
		sParam.name.SetString("specular");
		sParam.elements = 3;
		break;
	case 3:
		// emission
		sParam.name.SetString("emission");
		sParam.elements = 3;
		break;
	default:
		IZ_ASSERT(IZ_FALSE);
		break;
	}
	
	sParam.key = sParam.name.GetKeyValue();

	sParam.type = izanagi::E_MTRL_PARAM_TYPE_FLOAT;
	sParam.bytes = static_cast<IZ_UINT16>(sizeof(IZ_FLOAT) * sParam.elements);
}

void CXFileMaterial::GetMaterialParamValue(
	IZ_UINT nMtrlIdx,
	IZ_UINT nParamIdx,
	std::vector<IZ_FLOAT>& tvValue)
{
	IZ_ASSERT(nMtrlIdx < GetMaterialNum());

	const SXFileMaterial& sXfileMtrl = m_MtrlList[nMtrlIdx];

	switch (nParamIdx) {
	case 0:
		// diffuse
		tvValue.push_back(sXfileMtrl.diffuse.r);
		tvValue.push_back(sXfileMtrl.diffuse.g);
		tvValue.push_back(sXfileMtrl.diffuse.b);
		break;
	case 1:
		// specularExp
		tvValue.push_back(sXfileMtrl.specularExp);
		break;
	case 2:
		// specular
		tvValue.push_back(sXfileMtrl.specular.r);
		tvValue.push_back(sXfileMtrl.specular.g);
		tvValue.push_back(sXfileMtrl.specular.b);
		break;
	case 3:
		// emission
		tvValue.push_back(sXfileMtrl.emisssion.r);
		tvValue.push_back(sXfileMtrl.emisssion.g);
		tvValue.push_back(sXfileMtrl.emisssion.b);
		break;
	default:
		IZ_ASSERT(IZ_FALSE);
		break;
	}
}
