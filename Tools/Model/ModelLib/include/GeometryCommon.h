#if !defined(__MODEL_LIB_COMMON_DEFS_H__)
#define __MODEL_LIB_COMMON_DEFS_H__

#include "izDefs.h"
#include "izMath.h"
#include "izSceneGraph.h"

#include <set>
#include <vector>

///////////////////////////////////////

enum E_MDL_JOINT_TRANSFORM {
	E_MDL_JOINT_TRANSFORM_TRANSLATE = 0,
	E_MDL_JOINT_TRANSFORM_QUATERNION,
	E_MDL_JOINT_TRANSFORM_SCALE,

	E_MDL_JOINT_TRANSFORM_AXIS_ROT,
	E_MDL_JOINT_TRANSFORM_ROTATE,
	E_MDL_JOINT_TRANSFORM_SKEW,

	E_MDL_JOINT_TRANSFORM_NUM,
	E_MDL_JOINT_TRANSFORM_FORCE_INT32 = 0x7fffffff,
};

///////////////////////////////////////

/** 三角形情報
 */
struct STri {
	IZ_UINT vtx[3];				///< 三角形を構成する頂点インデックス
	std::set<IZ_UINT> joint;	///< 三角形を構成する頂点に影響を与える（スキニングする）関節インデックス

	/** 三角形に影響を与える関節インデックスから一意に決まるキーを計算する.
	 */
	IZ_UINT ComputeKey() const;

	/** 三角形に影響を与える関節数を取得.
	 */ 
	IZ_UINT GetJointNum() const;

	/** 指定された関節を削除.
	 */
	void EraseJoint(IZ_UINT idx);
};

///////////////////////////////////////

/** スキニング情報.
 * １頂点ごとに存在する
 */
struct SSkin {
	std::vector<IZ_UINT> joint;		///< 影響を与える関節のインデックス
	std::vector<IZ_FLOAT> weight;	///< ウエイト値

	/** 関節を登録.
	 */
	void Add(IZ_UINT nJointIdx, IZ_FLOAT fWeight);

	/** ウエイト値の合計が１になるように正規化する.
	 */
	void Normalize();

	/** 指定された関節を削除する.
	 */
	IZ_BOOL EraseJoint(IZ_UINT idx);
};

///////////////////////////////////////

/** プリミティブセット.
 * 所属関節ごとにまとめられた三角形群
 */
struct SPrimSet {
	IZ_UINT key;				///< 三角形に影響を与える関節インデックスから一意に決まるキー
	std::vector<IZ_UINT> tri;	///< 関節から影響を受ける三角形群

	std::set<IZ_UINT> joint;	///< 三角形に影響を与える関節インデックス

	IZ_UINT idxVB;
	IZ_UINT16 minIdx;
	IZ_UINT16 maxIdx;

	// For std::find
	bool operator==(IZ_UINT rhs)
	{
		return (key == rhs);
	}

	bool operator==(const SPrimSet& rhs);

#if 0
private:
	static std::vector<STri>* ptrTriList;

public:
	static void SetTriList(std::vector<STri>* pTriList) { ptrTriList = pTriList; }
	static std::vector<STri>* GetTriList()
	{
		IZ_ASSERT(ptrTriList != IZ_NULL);
		return ptrTriList;
	}
#endif
};

///////////////////////////////////////

/** メッシュ情報.
 */
struct SMesh {
	IZ_UINT startTri;	///< メッシュを構成する三角形の開始インデックス
	IZ_UINT endTri;		///< メッシュを構成する三角形の終了インデックス

	std::vector<SPrimSet> subset;

	IZ_UINT fmt;		///< メッシュにおける頂点フォーマット
	IZ_UINT sizeVtx;	///< メッシュにおける１頂点あたりのサイズ
};

///////////////////////////////////////

struct SVtxAdditional {
	std::vector<izanagi::SVector> nml;
	std::vector<izanagi::SVector> tangent;

	IZ_BOOL HasNormal() const
	{
		return !nml.empty();
	}

	IZ_BOOL HasTangent() const
	{
		return !tangent.empty();
	}

	void FixNormal()
	{
		if (nml.empty()) {
			return;
		}

		izanagi::SVector v;
		izanagi::SVector::SetZero(v);

		for (size_t i = 0; i < nml.size(); i++) {
			v.x += nml[i].x;
			v.y += nml[i].y;
			v.z += nml[i].z;
		}

		IZ_FLOAT div = 1.0f / nml.size();
		v.x *= div;
		v.y *= div;
		v.z *= div;

		nml.clear();
		nml.push_back(v);
	}

	void GetNormal(izanagi::SVector& v) const
	{
		// NOTE
		// Need to call "FixNormal" before call this function...
		IZ_ASSERT(nml.size() == 1);
		izanagi::SVector::Copy(v, nml[0]);
	}

	void FixTangent()
	{
		if (tangent.empty()) {
			return;
		}

		izanagi::SVector v;
		izanagi::SVector::SetZero(v);

		for (size_t i = 0; i < tangent.size(); i++) {
			v.x += tangent[i].x;
			v.y += tangent[i].y;
			v.z += tangent[i].z;
		}

		IZ_FLOAT div = 1.0f / tangent.size();
		v.x *= div;
		v.y *= div;
		v.z *= div;
	}

	void GetTangent(izanagi::SVector& v) const
	{
		// NOTE
		// Need to call "FixTangent" before call this function...
		IZ_ASSERT(tangent.size() == 1);
		izanagi::SVector::Copy(v, tangent[0]);
	}
};

///////////////////////////////////////

struct SJointTransform {
	IZ_PCSTR name;
	E_MDL_JOINT_TRANSFORM type;
	std::vector<IZ_FLOAT> param;
};

#endif	// #if !defined(__MODEL_LIB_COMMON_DEFS_H__)
