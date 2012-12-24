#include "PMDLoader.h"

// �t�@�C�����[�h
IZ_BOOL CPmdLoader::Load(IZ_PCSTR path)
{
    // Read all data.
    izanagi::CFileInputStream stream;
    VRETURN(stream.Open(path));

    // �t�@�C���w�b�_
    IZ_INPUT_READ_VRETURN(&stream, &m_Header, 0, sizeof(m_Header));

    // ���_�`�����N
    VRETURN(LoadVtxChunk(&stream));

    // �ʃ`�����N
    VRETURN(LoadFaceChunk(&stream));

    // �}�e���A���`�����N
    VRETURN(LoadMtrlChunk(&stream));

    // �{�[���`�����N
    VRETURN(LoadBoneChunk(&stream));

    // IK�`�����N
    VRETURN(LoadIkChunk(&stream));

    return IZ_TRUE;
}

// �N���A
void CPmdLoader::Clear()
{
    ClearGeometryData();
    m_MtrlList.clear();
}

// �W�I���g���Ɋւ���f�[�^�̂݃N���A
void CPmdLoader::ClearGeometryData()
{
    m_VtxList.clear();
    m_FaceList.clear();
}

// �֐߂Ɋւ���f�[�^�̂݃N���A
void CPmdLoader::ClearJointData()
{
    m_PoseList.clear();
    m_MtxList.clear();
}

// ���_�f�[�^�擾
const SPmdVertex& CPmdLoader::GetVertex(IZ_UINT idx)
{
    IZ_ASSERT(idx < m_VtxList.size());
    return m_VtxList[idx];
}

// �ʃf�[�^�擾
const SPmdFace& CPmdLoader::GetFace(IZ_UINT idx)
{
    IZ_ASSERT(idx < m_FaceList.size());
    return m_FaceList[idx];
}

// �}�e���A���f�[�^�擾
const SPmdMaterial& CPmdLoader::GetMtrl(IZ_UINT idx)
{
    IZ_ASSERT(idx < m_MtrlList.size());
    return m_MtrlList[idx];
}

// �{�[���f�[�^�擾
const SPmdBone& CPmdLoader::GetBone(IZ_UINT idx)
{
    IZ_ASSERT(idx < m_BoneList.size());
    return m_BoneList[idx];
}

// �֐߂̎p�����v�Z
void CPmdLoader::ComputeJointPose()
{
    for (IZ_UINT i = 0; i < m_BoneList.size(); i++)
    {
        const SPmdBone& bone = m_BoneList[i];

        izanagi::S_SKL_JOINT_POSE pose;

        // �X�P�[���͏�ɂP
        pose.scale[0] = pose.scale[1] = pose.scale[2] = 1.0f;

        // ��]�͂��̎��_�ł͑��݂��Ȃ�
        izanagi::math::SQuat::SetIdentity(pose.quat);

        if (bone.parentIdx == 0xffff)
        {
            // �ʒu
            pose.trans[0] = bone.boneHeadPos[0];
            pose.trans[1] = bone.boneHeadPos[1];
            pose.trans[2] = bone.boneHeadPos[2];
        }
        else
        {
            const SPmdBone& parent = m_BoneList[bone.parentIdx];

            // �ʒu
            // �e����̍���
            pose.trans[0] = bone.boneHeadPos[0] - parent.boneHeadPos[0];
            pose.trans[1] = bone.boneHeadPos[1] - parent.boneHeadPos[1];
            pose.trans[2] = bone.boneHeadPos[2] - parent.boneHeadPos[2];
        }

        m_PoseList.push_back(pose);

        // �s��
        izanagi::math::SMatrix mtx;
        izanagi::math::SMatrix::GetTrans(
            mtx,
            izanagi::math::CVector(pose.trans[0], pose.trans[1], pose.trans[2]));

        m_MtxList.push_back(mtx);
    }

    // �s��̐e�q�֌W���\�z
    for (IZ_UINT i = 0; i < m_BoneList.size(); i++)
    {
        const SPmdBone& bone = m_BoneList[i];

        if (bone.parentIdx != 0xffff)
        {
            const izanagi::math::SMatrix& mtxParent = m_MtxList[bone.parentIdx];
            izanagi::math::SMatrix& mtx = m_MtxList[i];

            izanagi::math::SMatrix::Mul(mtx, mtx, mtxParent);
        }
    }
}

// �֐߂̎p�����擾.
const izanagi::S_SKL_JOINT_POSE& CPmdLoader::GetPose(IZ_UINT idx)
{
    IZ_ASSERT(idx < m_PoseList.size());
    return m_PoseList[idx];
}

// �֐߂̃}�g���N�X���擾.
const izanagi::math::SMatrix& CPmdLoader::GetMatrix(IZ_UINT idx)
{
    IZ_ASSERT(idx < m_MtxList.size());
    return m_MtxList[idx];
}

// �{�[��������C���f�b�N�X���擾.
IZ_INT CPmdLoader::GetIdxFromBoneName(const char* name) const
{
    for (IZ_UINT i = 0; i < m_BoneList.size(); i++)
    {
        const SPmdBone& bone = m_BoneList[i];

        size_t nameLen_0 = ::strlen(bone.boneName);
        size_t nameLen_1 = ::strlen(name);

        if (nameLen_0 == nameLen_1
            && (memcmp(bone.boneName, name, nameLen_0) == 0))
        {
            return i;
        }
    }

    // TODO
    //IZ_ASSERT(IZ_FALSE);
    return -1;
}

// ���_�`�����N���[�h
IZ_BOOL CPmdLoader::LoadVtxChunk(izanagi::IInputStream* stream)
{
    // ���_�`�����N�w�b�_
    IZ_INPUT_READ_VRETURN(
        stream,
        &m_VtxChunkHeader,
        0,
        sizeof(m_VtxChunkHeader));

    // ���_���
    m_VtxList.resize(m_VtxChunkHeader.vtxNum);
    for (IZ_UINT i = 0; i < m_VtxChunkHeader.vtxNum; i++)
    {
        SPmdVertex& vtx = m_VtxList[i];
        IZ_INPUT_READ_VRETURN(stream, &vtx, 0, sizeof(vtx));
    }

    return IZ_TRUE;
}

// �ʃ`�����N���[�h
IZ_BOOL CPmdLoader::LoadFaceChunk(izanagi::IInputStream* stream)
{
    // �ʃ`�����N�w�b�_
    IZ_INPUT_READ_VRETURN(
        stream,
        &m_FaceChunkHeader,
        0,
        sizeof(m_FaceChunkHeader));

    // NOTE
    // �P�ʂ�����R���_�Ȃ̂ŁA�����_���͂R�Ŋ���؂�Ȃ��Ă͂����Ȃ�
    VRETURN((m_FaceChunkHeader.vtxNum % 3) == 0);

    IZ_UINT faceNum = m_FaceChunkHeader.vtxNum / 3;

    // �ʏ��
    m_FaceList.resize(faceNum);
    for (IZ_UINT i = 0; i < faceNum; i++)
    {
        SPmdFace& face = m_FaceList[i];
        IZ_INPUT_READ_VRETURN(stream, &face, 0, sizeof(face));
    }

    return IZ_TRUE;
}

// �}�e���A���`�����N���[�h
IZ_BOOL CPmdLoader::LoadMtrlChunk(izanagi::IInputStream* stream)
{
    // �}�e���A���`�����N�w�b�_
    IZ_INPUT_READ_VRETURN(
        stream,
        &m_MtrlChunkHeader,
        0,
        sizeof(m_MtrlChunkHeader));

    // �}�e���A�����
    m_MtrlList.resize(m_MtrlChunkHeader.mtrlNum);
    for (IZ_UINT i = 0; i < m_MtrlChunkHeader.mtrlNum; i++)
    {
        SPmdMaterial& mtrl = m_MtrlList[i];
        IZ_INPUT_READ_VRETURN(stream, &mtrl, 0, sizeof(mtrl));
    }

    return IZ_TRUE;
}

// �{�[���`�����N���[�h
IZ_BOOL CPmdLoader::LoadBoneChunk(izanagi::IInputStream* stream)
{
    // �{�[���`�����N�w�b�_
    IZ_INPUT_READ_VRETURN(
        stream,
        &m_BoneChunkHeader,
        0,
        sizeof(m_BoneChunkHeader));

    // �{�[�����
    m_BoneList.resize(m_BoneChunkHeader.boneNum);
    for (IZ_UINT i = 0; i < m_BoneChunkHeader.boneNum; i++)
    {
        SPmdBone& bone = m_BoneList[i];
        IZ_INPUT_READ_VRETURN(stream, &bone, 0, sizeof(bone));
    }

    return IZ_TRUE;
}

// IK�`�����N���[�h
IZ_BOOL CPmdLoader::LoadIkChunk(izanagi::IInputStream* stream)
{
    // IK�`�����N�w�b�_
    IZ_INPUT_READ_VRETURN(
        stream,
        &m_IkChunkHeader,
        0,
        sizeof(m_IkChunkHeader));

    // IK���
    m_IkList.resize(m_IkChunkHeader.ikNum);
    for (IZ_UINT i = 0; i < m_IkChunkHeader.ikNum; i++)
    {
        SPmdIk& ik = m_IkList[i];

        IZ_INPUT_READ_VRETURN(stream, &ik.param, 0, sizeof(ik.param));

        if (ik.param.chainNum > 0)
        {
            ik.childBoneIdx.resize(ik.param.chainNum);
            IZ_INPUT_READ_VRETURN(
                stream,
                &ik.childBoneIdx[0],
                0,
                sizeof(WORD) * ik.param.chainNum);
        }
    }

    return IZ_TRUE;
}
