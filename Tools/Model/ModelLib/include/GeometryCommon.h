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

/** �O�p�`���
 */
struct STri {
	IZ_UINT vtx[3];				///< �O�p�`���\�����钸�_�C���f�b�N�X
	std::set<IZ_UINT> joint;	///< �O�p�`���\�����钸�_�ɉe����^����i�X�L�j���O����j�֐߃C���f�b�N�X

	/** �O�p�`�ɉe����^����֐߃C���f�b�N�X�����ӂɌ��܂�L�[���v�Z����.
	 */
	IZ_UINT ComputeKey() const;

	/** �O�p�`�ɉe����^����֐ߐ����擾.
	 */ 
	IZ_UINT GetJointNum() const;

	/** �w�肳�ꂽ�֐߂��폜.
	 */
	void EraseJoint(IZ_UINT idx);
};

///////////////////////////////////////

/** �X�L�j���O���.
 * �P���_���Ƃɑ��݂���
 */
struct SSkin {
	std::vector<IZ_UINT> joint;		///< �e����^����֐߂̃C���f�b�N�X
	std::vector<IZ_FLOAT> weight;	///< �E�G�C�g�l

	/** �֐߂�o�^.
	 */
	void Add(IZ_UINT nJointIdx, IZ_FLOAT fWeight);

	/** �E�G�C�g�l�̍��v���P�ɂȂ�悤�ɐ��K������.
	 */
	void Normalize();

	/** �w�肳�ꂽ�֐߂��폜����.
	 */
	IZ_BOOL EraseJoint(IZ_UINT idx);
};

///////////////////////////////////////

/** �v���~�e�B�u�Z�b�g.
 * �����֐߂��Ƃɂ܂Ƃ߂�ꂽ�O�p�`�Q
 */
struct SPrimSet {
	IZ_UINT key;				///< �O�p�`�ɉe����^����֐߃C���f�b�N�X�����ӂɌ��܂�L�[
	std::vector<IZ_UINT> tri;	///< �֐߂���e�����󂯂�O�p�`�Q

	std::set<IZ_UINT> joint;	///< �O�p�`�ɉe����^����֐߃C���f�b�N�X

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

/** ���b�V�����.
 */
struct SMesh {
	IZ_UINT startTri;	///< ���b�V�����\������O�p�`�̊J�n�C���f�b�N�X
	IZ_UINT endTri;		///< ���b�V�����\������O�p�`�̏I���C���f�b�N�X

	std::vector<SPrimSet> subset;

	IZ_UINT fmt;		///< ���b�V���ɂ����钸�_�t�H�[�}�b�g
	IZ_UINT sizeVtx;	///< ���b�V���ɂ�����P���_������̃T�C�Y
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
