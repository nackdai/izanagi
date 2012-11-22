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
	// �t�@�C�����[�h
	IZ_BOOL Load(IZ_PCSTR path);

	// �N���A
	void Clear();

	// �W�I���g���Ɋւ���f�[�^�̂݃N���A
	void ClearGeometryData();

	// �֐߂Ɋւ���f�[�^�̂݃N���A
	void ClearJointData();

	// ���_�f�[�^�擾
	const SPmdVertex& GetVertex(IZ_UINT idx);

	// �ʃf�[�^�擾
	const SPmdFace& GetFace(IZ_UINT idx);

	// �}�e���A���f�[�^�擾
	const SPmdMaterial& GetMtrl(IZ_UINT idx);

	// �{�[���f�[�^�擾
	const SPmdBone& GetBone(IZ_UINT idx);

	// �֐߂̎p�����v�Z
	void ComputeJointPose();

	// �֐߂̎p�����擾.
	const izanagi::S_SKL_JOINT_POSE& GetPose(IZ_UINT idx);

	// �֐߂̃}�g���N�X���擾.
	const izanagi::SMatrix& GetMatrix(IZ_UINT idx);

	// �{�[��������C���f�b�N�X���擾.
	IZ_INT GetIdxFromBoneName(const char* name) const;

private:
	// ���_�`�����N���[�h
	IZ_BOOL LoadVtxChunk(izanagi::IInputStream* stream);

	// �ʃ`�����N���[�h
	IZ_BOOL LoadFaceChunk(izanagi::IInputStream* stream);

	// �}�e���A���`�����N���[�h
	IZ_BOOL LoadMtrlChunk(izanagi::IInputStream* stream);

	// �{�[���`�����N���[�h
	IZ_BOOL LoadBoneChunk(izanagi::IInputStream* stream);

	// IK�`�����N���[�h
	IZ_BOOL LoadIkChunk(izanagi::IInputStream* stream);

public:
	// ���_���擾
	IZ_UINT GetVtxNum() const
	{
		return m_VtxChunkHeader.vtxNum;
	}

	// �ʐ��擾
	IZ_UINT GetFaceNum() const
	{
		IZ_UINT ret = m_FaceChunkHeader.vtxNum;
		return ret / 3;
	}

	// �}�e���A�����擾
	IZ_UINT GetMtrlNum() const
	{
		return m_MtrlChunkHeader.mtrlNum;
	}

	// �{�[�����擾
	IZ_UINT GetBoneNum() const
	{
		return m_BoneChunkHeader.boneNum;
	}

private:
	// �t�@�C���w�b�_
	SPmdHeader m_Header;

	// ���_
	SPmdVtxChunkHeader m_VtxChunkHeader;
	std::vector<SPmdVertex> m_VtxList;

	// ��
	SPmdFaceChunkHeader m_FaceChunkHeader;
	std::vector<SPmdFace> m_FaceList;

	// �}�e���A��
	SPmdMtrlChunkHeader m_MtrlChunkHeader;
	std::vector<SPmdMaterial> m_MtrlList;

	// �{�[��
	SPmdBoneChunkHeader m_BoneChunkHeader;
	std::vector<SPmdBone> m_BoneList;

	// �֐߂̎p��
	std::vector<izanagi::S_SKL_JOINT_POSE> m_PoseList;
	std::vector<izanagi::SMatrix> m_MtxList;

	// IK
	SPmdIkChunkHeader m_IkChunkHeader;
	std::vector<SPmdIk> m_IkList;
};

#endif	// #if !defined(__MODEL_LIB_PMD_LOADER_H__)
