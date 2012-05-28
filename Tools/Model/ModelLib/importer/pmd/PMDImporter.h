#if !defined(__MODEL_LIB_PMD_IMPORTER_H__)
#define __MODEL_LIB_PMD_IMPORTER_H__

#include "Importer.h"
#include "PMDLoader.h"
#include <vector>

class CPmdImporter : public IImporter {
	friend class IImporter;

protected:
	CPmdImporter();
	~CPmdImporter() { Close(); }

	NO_COPIABLE(CPmdImporter);

public:
	IZ_BOOL Open(IZ_PCSTR pszName);
	IZ_BOOL Close();

	//////////////////////////////////
	// For geometry chunk.

	void ExportGeometryCompleted();

	IZ_UINT GetMeshNum();

	// メッシュに関する処理を開始.
	void BeginMesh(IZ_UINT nIdx);

	// メッシュに関する処理を終了.
	void EndMesh();

	// BeginMeshで指定されたメッシュに含まれスキニング情報を取得.
	void GetSkinList(std::vector<SSkin>& tvSkinList);

	// BeginMeshで指定されたメッシュに含まれる三角形を取得.
	IZ_UINT GetTriangles(std::vector<STri>& tvTriList);

	// 指定された頂点に影響を与えるスキニング情報へのインデックスを取得.
	IZ_UINT GetSkinIdxAffectToVtx(IZ_UINT nVtxIdx);

	// １頂点あたりのサイズを取得.
	// ただし、スキニングに関するサイズは含まない
	IZ_UINT GetVtxSize();

	// 頂点フォーマットを取得.
	// ただし、スキニングに関するフォーマットは含まない
	IZ_UINT GetVtxFmt();

	// 指定された頂点における指定フォーマットのデータを取得.
	IZ_BOOL GetVertex(
		IZ_UINT nIdx,
		izanagi::SVector& vec,
		izanagi::E_MSH_VTX_FMT_TYPE type);

	void GetMaterialForMesh(
		IZ_UINT nIdx,
		izanagi::S_MSH_MTRL& sMtrl);

	//////////////////////////////////
	// For joint chunk.

	// 関節データの出力完了を通知.
	void ExportJointCompleted();

	// 関節に関する処理を開始.
	IZ_BOOL BeginJoint();

	// 関節に関する処理を終了.
	void EndJoint();

	// 関節数を取得.
	IZ_UINT GetJointNum();

	// 指定された関節の名前を取得.
	IZ_PCSTR GetJointName(IZ_UINT nIdx);

	// 親関節へのインデックスを取得.	
	IZ_INT GetJointParent(
		IZ_UINT nIdx,
		const std::vector<izanagi::S_SKL_JOINT>& tvJoint);

	// 指定された関節の逆マトリクスを取得.	
	void GetJointInvMtx(
		IZ_UINT nIdx,
		izanagi::SMatrix& mtx);
		
	// 関節の姿勢を取得.
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

private:
	CPmdLoader m_Loader;

	IZ_INT m_CurMeshIdx;

	// マテリアルにバインドされている面のインデックスのリスト
	// マテリアル[0]から順に対応する面の先頭のインデックスが保持される
	std::vector<IZ_UINT> m_FaceIdxInMtrlList;
};

#endif	// #if !defined(__MODEL_LIB_PMD_IMPORTER_H__)
