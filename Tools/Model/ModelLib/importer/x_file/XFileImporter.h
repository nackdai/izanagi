#if !defined(__MODEL_LIB_XFILE_IMPORTER_H__)
#define __MODEL_LIB_XFILE_IMPORTER_H__

#include "Importer.h"
#include "XFileParser.h"

class CXFileImporter : public IImporter {
	friend class IImporter;

protected:
	CXFileImporter();
	~CXFileImporter() { Close(); }

	NO_COPIABLE(CXFileImporter);

public:
	static IZ_BOOL IsTxtFormat(IZ_PCSTR pszName);

	IZ_BOOL Open(IZ_PCSTR pszName);
	IZ_BOOL Close();

	//////////////////////////////////
	// For geometry chunk.

	void ExportGeometryCompleted();

	void BeginMesh(IZ_UINT nIdx);

	void EndMesh();
	
	IZ_UINT GetMeshNum();

	void GetSkinList(std::vector<SSkin>& tvSkinList);

	IZ_UINT GetTriangles(std::vector<STri>& tvTriList);

	IZ_UINT GetSkinIdx(IZ_UINT nVtxIdx);

	IZ_UINT GetVtxSize();

	IZ_UINT GetVtxFmt();

	IZ_BOOL GetVertex(
		IZ_UINT nIdx,
		izanagi::SVector& vec,
		izanagi::E_MSH_VTX_FMT_TYPE type);

	void GetMaterialForMesh(
		IZ_UINT nIdx,
		izanagi::S_MSH_MTRL& sMtrl);

	//////////////////////////////////
	// For joint chunk.

	void ExportJointCompleted();

	IZ_BOOL BeginJoint();

	void EndJoint();

	IZ_UINT GetJointNum();

	IZ_PCSTR GetJointName(IZ_UINT nIdx);

	IZ_INT GetJointParent(
		IZ_UINT nIdx,
		const std::vector<izanagi::S_SKL_JOINT>& tvJoint);

	void GetJointInvMtx(
		IZ_UINT nIdx,
		izanagi::SMatrix& mtx);
		
	void GetJointTransform(
		IZ_UINT nIdx,
		const std::vector<izanagi::S_SKL_JOINT>& tvJoint,
		std::vector<SJointTransform>& tvTransform);

	//////////////////////////////////
	// For animation.

	IZ_BOOL ReadBaseModel(IZ_PCSTR pszName);

	IZ_UINT GetAnmSetNum();

	IZ_BOOL BeginAnm(IZ_UINT nSetIdx);

	IZ_BOOL EndAnm();

	IZ_UINT GetAnmNodeNum();

	IZ_UINT GetAnmChannelNum(IZ_UINT nNodeIdx);

	IZ_BOOL GetAnmNode(
		IZ_UINT nNodeIdx,
		izanagi::S_ANM_NODE& sNode);

	IZ_BOOL GetAnmChannel(
		IZ_UINT nNodeIdx,
		IZ_UINT nChannelIdx,
		izanagi::S_ANM_CHANNEL& sChannel);
	
	IZ_BOOL GetAnmKey(
		IZ_UINT nNodeIdx,
		IZ_UINT nChannelIdx,
		IZ_UINT nKeyIdx,
		izanagi::S_ANM_KEY& sKey,
		std::vector<IZ_FLOAT>& tvValue);

	//////////////////////////////////
	// For material.

	void SetNameForInvalidMaterial(IZ_PCSTR pszName);

	IZ_BOOL BeginMaterial();

	IZ_BOOL EndMaterial();

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

protected:
	CXFileParser m_Parser;
	IZ_UINT m_nMeshIdx;

	CXFileParser* m_BaseMdlParser;
};

#endif	// #if !defined(__MODEL_LIB_XFILE_IMPORTER_H__)
