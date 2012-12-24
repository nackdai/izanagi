#include "PMDLoader.h"

// ファイルロード
IZ_BOOL CPmdLoader::Load(IZ_PCSTR path)
{
    // Read all data.
    izanagi::CFileInputStream stream;
    VRETURN(stream.Open(path));

    // ファイルヘッダ
    IZ_INPUT_READ_VRETURN(&stream, &m_Header, 0, sizeof(m_Header));

    // 頂点チャンク
    VRETURN(LoadVtxChunk(&stream));

    // 面チャンク
    VRETURN(LoadFaceChunk(&stream));

    // マテリアルチャンク
    VRETURN(LoadMtrlChunk(&stream));

    // ボーンチャンク
    VRETURN(LoadBoneChunk(&stream));

    // IKチャンク
    VRETURN(LoadIkChunk(&stream));

    return IZ_TRUE;
}

// クリア
void CPmdLoader::Clear()
{
    ClearGeometryData();
    m_MtrlList.clear();
}

// ジオメトリに関するデータのみクリア
void CPmdLoader::ClearGeometryData()
{
    m_VtxList.clear();
    m_FaceList.clear();
}

// 関節に関するデータのみクリア
void CPmdLoader::ClearJointData()
{
    m_PoseList.clear();
    m_MtxList.clear();
}

// 頂点データ取得
const SPmdVertex& CPmdLoader::GetVertex(IZ_UINT idx)
{
    IZ_ASSERT(idx < m_VtxList.size());
    return m_VtxList[idx];
}

// 面データ取得
const SPmdFace& CPmdLoader::GetFace(IZ_UINT idx)
{
    IZ_ASSERT(idx < m_FaceList.size());
    return m_FaceList[idx];
}

// マテリアルデータ取得
const SPmdMaterial& CPmdLoader::GetMtrl(IZ_UINT idx)
{
    IZ_ASSERT(idx < m_MtrlList.size());
    return m_MtrlList[idx];
}

// ボーンデータ取得
const SPmdBone& CPmdLoader::GetBone(IZ_UINT idx)
{
    IZ_ASSERT(idx < m_BoneList.size());
    return m_BoneList[idx];
}

// 関節の姿勢を計算
void CPmdLoader::ComputeJointPose()
{
    for (IZ_UINT i = 0; i < m_BoneList.size(); i++)
    {
        const SPmdBone& bone = m_BoneList[i];

        izanagi::S_SKL_JOINT_POSE pose;

        // スケールは常に１
        pose.scale[0] = pose.scale[1] = pose.scale[2] = 1.0f;

        // 回転はこの時点では存在しない
        izanagi::math::SQuat::SetIdentity(pose.quat);

        if (bone.parentIdx == 0xffff)
        {
            // 位置
            pose.trans[0] = bone.boneHeadPos[0];
            pose.trans[1] = bone.boneHeadPos[1];
            pose.trans[2] = bone.boneHeadPos[2];
        }
        else
        {
            const SPmdBone& parent = m_BoneList[bone.parentIdx];

            // 位置
            // 親からの差分
            pose.trans[0] = bone.boneHeadPos[0] - parent.boneHeadPos[0];
            pose.trans[1] = bone.boneHeadPos[1] - parent.boneHeadPos[1];
            pose.trans[2] = bone.boneHeadPos[2] - parent.boneHeadPos[2];
        }

        m_PoseList.push_back(pose);

        // 行列
        izanagi::math::SMatrix mtx;
        izanagi::math::SMatrix::GetTrans(
            mtx,
            izanagi::math::CVector(pose.trans[0], pose.trans[1], pose.trans[2]));

        m_MtxList.push_back(mtx);
    }

    // 行列の親子関係を構築
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

// 関節の姿勢を取得.
const izanagi::S_SKL_JOINT_POSE& CPmdLoader::GetPose(IZ_UINT idx)
{
    IZ_ASSERT(idx < m_PoseList.size());
    return m_PoseList[idx];
}

// 関節のマトリクスを取得.
const izanagi::math::SMatrix& CPmdLoader::GetMatrix(IZ_UINT idx)
{
    IZ_ASSERT(idx < m_MtxList.size());
    return m_MtxList[idx];
}

// ボーン名からインデックスを取得.
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

// 頂点チャンクロード
IZ_BOOL CPmdLoader::LoadVtxChunk(izanagi::IInputStream* stream)
{
    // 頂点チャンクヘッダ
    IZ_INPUT_READ_VRETURN(
        stream,
        &m_VtxChunkHeader,
        0,
        sizeof(m_VtxChunkHeader));

    // 頂点情報
    m_VtxList.resize(m_VtxChunkHeader.vtxNum);
    for (IZ_UINT i = 0; i < m_VtxChunkHeader.vtxNum; i++)
    {
        SPmdVertex& vtx = m_VtxList[i];
        IZ_INPUT_READ_VRETURN(stream, &vtx, 0, sizeof(vtx));
    }

    return IZ_TRUE;
}

// 面チャンクロード
IZ_BOOL CPmdLoader::LoadFaceChunk(izanagi::IInputStream* stream)
{
    // 面チャンクヘッダ
    IZ_INPUT_READ_VRETURN(
        stream,
        &m_FaceChunkHeader,
        0,
        sizeof(m_FaceChunkHeader));

    // NOTE
    // １面あたり３頂点なので、総頂点数は３で割り切れなくてはいけない
    VRETURN((m_FaceChunkHeader.vtxNum % 3) == 0);

    IZ_UINT faceNum = m_FaceChunkHeader.vtxNum / 3;

    // 面情報
    m_FaceList.resize(faceNum);
    for (IZ_UINT i = 0; i < faceNum; i++)
    {
        SPmdFace& face = m_FaceList[i];
        IZ_INPUT_READ_VRETURN(stream, &face, 0, sizeof(face));
    }

    return IZ_TRUE;
}

// マテリアルチャンクロード
IZ_BOOL CPmdLoader::LoadMtrlChunk(izanagi::IInputStream* stream)
{
    // マテリアルチャンクヘッダ
    IZ_INPUT_READ_VRETURN(
        stream,
        &m_MtrlChunkHeader,
        0,
        sizeof(m_MtrlChunkHeader));

    // マテリアル情報
    m_MtrlList.resize(m_MtrlChunkHeader.mtrlNum);
    for (IZ_UINT i = 0; i < m_MtrlChunkHeader.mtrlNum; i++)
    {
        SPmdMaterial& mtrl = m_MtrlList[i];
        IZ_INPUT_READ_VRETURN(stream, &mtrl, 0, sizeof(mtrl));
    }

    return IZ_TRUE;
}

// ボーンチャンクロード
IZ_BOOL CPmdLoader::LoadBoneChunk(izanagi::IInputStream* stream)
{
    // ボーンチャンクヘッダ
    IZ_INPUT_READ_VRETURN(
        stream,
        &m_BoneChunkHeader,
        0,
        sizeof(m_BoneChunkHeader));

    // ボーン情報
    m_BoneList.resize(m_BoneChunkHeader.boneNum);
    for (IZ_UINT i = 0; i < m_BoneChunkHeader.boneNum; i++)
    {
        SPmdBone& bone = m_BoneList[i];
        IZ_INPUT_READ_VRETURN(stream, &bone, 0, sizeof(bone));
    }

    return IZ_TRUE;
}

// IKチャンクロード
IZ_BOOL CPmdLoader::LoadIkChunk(izanagi::IInputStream* stream)
{
    // IKチャンクヘッダ
    IZ_INPUT_READ_VRETURN(
        stream,
        &m_IkChunkHeader,
        0,
        sizeof(m_IkChunkHeader));

    // IK情報
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
