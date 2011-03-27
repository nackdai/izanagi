#if !defined(__MODEL_LIB_IMPORTER_H__)
#define __MODEL_LIB_IMPORTER_H__

#include "GeometryCommon.h"
#include "izIo.h"
#include "izSceneGraph.h"

#include <vector>

class IImporter {
public:
	template <typename _T>
	static IImporter* CreateImporter()
	{
		IImporter* ret = new _T;
		IZ_ASSERT(ret != IZ_NULL);
		return ret;
	}

protected:
	IImporter() {}
	virtual ~IImporter() {}

public:
	PURE_VIRTUAL(IZ_BOOL Open(IZ_PCSTR pszName));
	PURE_VIRTUAL(IZ_BOOL Close());

	//////////////////////////////////
	// For geometry chunk.

	PURE_VIRTUAL(void ExportGeometryCompleted());
	
	PURE_VIRTUAL(IZ_UINT GetMeshNum());

	PURE_VIRTUAL(void BeginMesh(IZ_UINT nIdx));

	PURE_VIRTUAL(void EndMesh());

	PURE_VIRTUAL(void GetSkinList(std::vector<SSkin>& tvSkinList));

	PURE_VIRTUAL(IZ_UINT GetTriangles(std::vector<STri>& tvTriList));

	PURE_VIRTUAL(IZ_UINT GetSkinIdx(IZ_UINT nVtxIdx));

	PURE_VIRTUAL(IZ_UINT GetVtxSize());

	PURE_VIRTUAL(IZ_UINT GetVtxFmt());

	PURE_VIRTUAL(
		IZ_BOOL GetVertex(
			IZ_UINT nIdx,
			izanagi::SVector& vec,
			izanagi::E_MSH_VTX_FMT_TYPE type));

	//////////////////////////////////
	// For joint chunk.

	PURE_VIRTUAL(void ExportJointCompleted());

	PURE_VIRTUAL(IZ_BOOL BeginJoint());

	PURE_VIRTUAL(void EndJoint());

	PURE_VIRTUAL(IZ_UINT GetJointNum());

	PURE_VIRTUAL(IZ_PCSTR GetJointName(IZ_UINT nIdx));

	PURE_VIRTUAL(
		IZ_INT GetJointParent(
			IZ_UINT nIdx,
			const std::vector<izanagi::S_SKL_JOINT>& tvJoint));

	PURE_VIRTUAL(
		void GetJointInvMtx(
			IZ_UINT nIdx,
			izanagi::SMatrix& mtx));
		
	PURE_VIRTUAL(
		void GetJointTransform(
			IZ_UINT nIdx,
			const std::vector<izanagi::S_SKL_JOINT>& tvJoint,
			std::vector<SJointTransform>& tvTransform));

	//////////////////////////////////
	// For animation.

	PURE_VIRTUAL(IZ_BOOL ReadBaseModel(IZ_PCSTR pszName));

	PURE_VIRTUAL(IZ_UINT GetAnmSetNum());

	PURE_VIRTUAL(IZ_BOOL BeginAnm(IZ_UINT nSetIdx));

	PURE_VIRTUAL(IZ_BOOL EndAnm());

	PURE_VIRTUAL(IZ_UINT GetAnmNodeNum());

	PURE_VIRTUAL(
		IZ_UINT GetAnmChannelNum(IZ_UINT nNodeIdx));

	PURE_VIRTUAL(
		IZ_BOOL GetAnmNode(
			IZ_UINT nNodeIdx,
			izanagi::S_ANM_NODE& sNode));

	PURE_VIRTUAL(
		IZ_BOOL GetAnmChannel(
			IZ_UINT nNodeIdx,
			IZ_UINT nChannelIdx,
			izanagi::S_ANM_CHANNEL& sChannel));
		
	PURE_VIRTUAL(
		IZ_BOOL GetAnmKey(
			IZ_UINT nNodeIdx,
			IZ_UINT nChannelIdx,
			IZ_UINT nKeyIdx,
			izanagi::S_ANM_KEY& sKey,
			std::vector<IZ_FLOAT>& tvValue));

	//////////////////////////////////
	// For material.

	PURE_VIRTUAL(IZ_BOOL BeginMaterial());

	PURE_VIRTUAL(IZ_BOOL EndMaterial());

	PURE_VIRTUAL(IZ_UINT GetMaterialNum());

	PURE_VIRTUAL(
		IZ_BOOL GetMaterial(
			IZ_UINT nMtrlIdx,
			izanagi::S_MTRL_MATERIAL& sMtrl));

	PURE_VIRTUAL(
		void GetMaterialTexture(
			IZ_UINT nMtrlIdx,
			IZ_UINT nTexIdx,
			izanagi::S_MTRL_TEXTURE& sTex));

	PURE_VIRTUAL(
		void GetMaterialShader(
			IZ_UINT nMtrlIdx,
			IZ_UINT nShaderIdx,
			izanagi::S_MTRL_SHADER& sShader));

	PURE_VIRTUAL(
		void GetMaterialParam(
			IZ_UINT nMtrlIdx,
			IZ_UINT nParamIdx,
			izanagi::S_MTRL_PARAM& sParam));

	PURE_VIRTUAL(
		void GetMaterialParamValue(
			IZ_UINT nMtrlIdx,
			IZ_UINT nParamIdx,
			std::vector<IZ_FLOAT>& tvValue));
};

#endif	// #if !defined(__MODEL_LIB_IMPORTER_H__)
