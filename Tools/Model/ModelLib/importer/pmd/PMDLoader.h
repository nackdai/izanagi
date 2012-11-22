#if !defined(__MODEL_LIB_PMD_LOADER_H__)
#define __MODEL_LIB_PMD_LOADER_H__

#include "izDefs.h"
#include "PMDFormat.h"
#include "izToolKit.h"
#include "izSceneGraph.h"
#include <vector>

class CPmdLoader {
public:
	CPmdLoader() {}
	~CPmdLoader() {}

	NO_COPIABLE(CPmdLoader);

public:
	// ファイルロード
	IZ_BOOL Load(IZ_PCSTR path);

	// クリア
	void Clear();

	// ジオメトリに関するデータのみクリア
	void ClearGeometryData();

	// 関節に関するデータのみクリア
	void ClearJointData();

	// 頂点データ取得
	const SPmdVertex& GetVertex(IZ_UINT idx);

	// 面データ取得
	const SPmdFace& GetFace(IZ_UINT idx);

	// マテリアルデータ取得
	const SPmdMaterial& GetMtrl(IZ_UINT idx);

	// ボーンデータ取得
	const SPmdBone& GetBone(IZ_UINT idx);

	// 関節の姿勢を計算
	void ComputeJointPose();

	// 関節の姿勢を取得.
	const izanagi::S_SKL_JOINT_POSE& GetPose(IZ_UINT idx);

	// 関節のマトリクスを取得.
	const izanagi::SMatrix& GetMatrix(IZ_UINT idx);

	// ボーン名からインデックスを取得.
	IZ_INT GetIdxFromBoneName(const char* name) const;

private:
	// 頂点チャンクロード
	IZ_BOOL LoadVtxChunk(izanagi::IInputStream* stream);

	// 面チャンクロード
	IZ_BOOL LoadFaceChunk(izanagi::IInputStream* stream);

	// マテリアルチャンクロード
	IZ_BOOL LoadMtrlChunk(izanagi::IInputStream* stream);

	// ボーンチャンクロード
	IZ_BOOL LoadBoneChunk(izanagi::IInputStream* stream);

	// IKチャンクロード
	IZ_BOOL LoadIkChunk(izanagi::IInputStream* stream);

public:
	// 頂点数取得
	IZ_UINT GetVtxNum() const
	{
		return m_VtxChunkHeader.vtxNum;
	}

	// 面数取得
	IZ_UINT GetFaceNum() const
	{
		IZ_UINT ret = m_FaceChunkHeader.vtxNum;
		return ret / 3;
	}

	// マテリアル数取得
	IZ_UINT GetMtrlNum() const
	{
		return m_MtrlChunkHeader.mtrlNum;
	}

	// ボーン数取得
	IZ_UINT GetBoneNum() const
	{
		return m_BoneChunkHeader.boneNum;
	}

private:
	// ファイルヘッダ
	SPmdHeader m_Header;

	// 頂点
	SPmdVtxChunkHeader m_VtxChunkHeader;
	std::vector<SPmdVertex> m_VtxList;

	// 面
	SPmdFaceChunkHeader m_FaceChunkHeader;
	std::vector<SPmdFace> m_FaceList;

	// マテリアル
	SPmdMtrlChunkHeader m_MtrlChunkHeader;
	std::vector<SPmdMaterial> m_MtrlList;

	// ボーン
	SPmdBoneChunkHeader m_BoneChunkHeader;
	std::vector<SPmdBone> m_BoneList;

	// 関節の姿勢
	std::vector<izanagi::S_SKL_JOINT_POSE> m_PoseList;
	std::vector<izanagi::SMatrix> m_MtxList;

	// IK
	SPmdIkChunkHeader m_IkChunkHeader;
	std::vector<SPmdIk> m_IkList;
};

#endif	// #if !defined(__MODEL_LIB_PMD_LOADER_H__)
