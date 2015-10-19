#include "AssimpImporter.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>

AssimpImporter::AssimpImporter()
{
}

IZ_BOOL AssimpImporter::Open(IZ_PCSTR pszName)
{
    m_scene = m_importer.ReadFile(
        std::string(pszName),
        aiPostProcessSteps::aiProcess_Triangulate
        | aiPostProcessSteps::aiProcess_SortByPType
        | aiPostProcessSteps::aiProcess_JoinIdenticalVertices);

    return (m_scene != nullptr);
}

IZ_BOOL AssimpImporter::Close()
{
    // TODO
    // Nothing to do?
    return IZ_TRUE;
}

//////////////////////////////////
// For geometry chunk.

// ジオメトリチャンクのエクスポートが完了したことを通知.
void AssimpImporter::ExportGeometryCompleted()
{
    // Nothing to do.
}

void AssimpImporter::BeginMesh(IZ_UINT nIdx)
{
    m_curMeshIdx = nIdx;

    IZ_ASSERT(m_curMeshIdx < m_scene->mNumMeshes);

    auto mesh = m_scene->mMeshes[m_curMeshIdx];

    std::vector<IZ_UINT> vtx;

    for (IZ_UINT i = 0; i < mesh->mNumFaces; i++) {
        const auto& face = mesh->mFaces[i];
        IZ_ASSERT(face.mNumIndices == 3);

        vtx.push_back(face.mIndices[0]);
        vtx.push_back(face.mIndices[1]);
        vtx.push_back(face.mIndices[2]);
    }

    // ソート.
    std::sort(vtx.begin(), vtx.end());

    // 隣接する重複を削除.
    vtx.erase(
        std::unique(vtx.begin(), vtx.end()),
        vtx.end());

    m_curMeshVtxNum = vtx.size();
}

void AssimpImporter::EndMesh()
{
    m_curMeshIdx = -1;
    m_curMeshVtxNum = 0;
}

IZ_UINT AssimpImporter::GetMeshNum()
{
    IZ_UINT ret = m_scene->mNumMeshes;
    return ret;
}

// 指定されているメッシュに関連するスキニング情報を取得.
void AssimpImporter::GetSkinList(std::vector<SSkin>& tvSkinList)
{
    // NOTE
    // １頂点ごとに存在するスキニング情報を取得.

    IZ_ASSERT(m_curMeshIdx < m_scene->mNumMeshes);

    auto mesh = m_scene->mMeshes[m_curMeshIdx];
    IZ_ASSERT(mesh->HasBones());

    tvSkinList.reserve(m_curMeshVtxNum);

    for (IZ_UINT vtxId = 0; vtxId < m_curMeshVtxNum; vtxId++) {
        SSkin skin;
        skin.vtxId = vtxId;

        // TODO
        for (IZ_UINT i = 0; i < mesh->mNumBones; i++) {
            const auto& bone = mesh->mBones[i];

            for (IZ_UINT n = 0; n < bone->mNumWeights; n++) {
                const auto& weight = bone->mWeights[n];

                if (weight.mVertexId == vtxId) {
                    skin.Add(i, weight.mWeight);
                }
            }
        }

        if (skin.joint.size() > 0) {
            tvSkinList.push_back(skin);
        }
    }
}

// 指定されているメッシュに含まれる三角形を取得.
IZ_UINT AssimpImporter::GetTriangles(std::vector<STri>& tvTriList)
{
    IZ_ASSERT(m_curMeshIdx < m_scene->mNumMeshes);

    auto mesh = m_scene->mMeshes[m_curMeshIdx];
    IZ_ASSERT(mesh->HasFaces());

    tvTriList.reserve(mesh->mNumFaces);

    for (IZ_UINT i = 0; i < mesh->mNumFaces; i++) {
        const auto& face = mesh->mFaces[i];

        // Check if this is a triangle.
        IZ_ASSERT(face.mNumIndices == 3);

        STri tri;
        tri.vtx[0] = face.mIndices[0];
        tri.vtx[1] = face.mIndices[1];
        tri.vtx[2] = face.mIndices[2];

        tvTriList.push_back(tri);
    }

    IZ_UINT ret = mesh->mNumFaces;
    
    return ret;
}

IZ_UINT AssimpImporter::GetSkinIdxAffectToVtx(IZ_UINT nVtxIdx)
{
    IZ_ASSERT(m_curMeshIdx < m_scene->mNumMeshes);
    IZ_ASSERT(nVtxIdx < m_curMeshVtxNum);

    auto mesh = m_scene->mMeshes[m_curMeshIdx];
    IZ_ASSERT(mesh->HasBones());

    IZ_UINT skinPos = 0;

    for (IZ_UINT vtxId = 0; vtxId < m_curMeshVtxNum; vtxId++) {
        IZ_BOOL hasSkin = IZ_FALSE;

        // TODO
        for (IZ_UINT i = 0; i < mesh->mNumBones; i++) {
            const auto& bone = mesh->mBones[i];

            for (IZ_UINT n = 0; n < bone->mNumWeights; n++) {
                const auto& weight = bone->mWeights[n];

                if (weight.mVertexId == vtxId) {
                    hasSkin = IZ_TRUE;
                    break;
                }
            }
        }

        if (hasSkin) {
            if (vtxId == nVtxIdx) {
                return skinPos;
            }

            skinPos++;
        }
    }

    // TODO
    return 0;
}

IZ_UINT AssimpImporter::GetVtxSize()
{
    IZ_ASSERT(m_curMeshIdx < m_scene->mNumMeshes);

    auto mesh = m_scene->mMeshes[m_curMeshIdx];
    
    IZ_UINT ret = 0;

    if (mesh->HasPositions()) {
        ret += izanagi::E_MSH_VTX_SIZE::E_MSH_VTX_SIZE_POS;
    }
    if (mesh->GetNumUVChannels() > 0) {
        auto numChannels = mesh->GetNumUVChannels();
        ret += izanagi::E_MSH_VTX_SIZE::E_MSH_VTX_SIZE_UV * numChannels;
    }
    if (mesh->GetNumColorChannels() > 0) {
        auto numChannels = mesh->GetNumColorChannels();
        ret += izanagi::E_MSH_VTX_SIZE::E_MSH_VTX_SIZE_COLOR * numChannels;
    }
    if (mesh->HasNormals()) {
        ret += izanagi::E_MSH_VTX_SIZE::E_MSH_VTX_SIZE_NORMAL;
    }
    if (mesh->HasBones()) {
        ret += izanagi::E_MSH_VTX_SIZE::E_MSH_VTX_SIZE_BLENDINDICES;
        ret += izanagi::E_MSH_VTX_SIZE::E_MSH_VTX_SIZE_BLENDWEIGHT;
    }

    return ret;
}

IZ_UINT AssimpImporter::GetVtxFmt()
{
    IZ_ASSERT(m_curMeshIdx < m_scene->mNumMeshes);

    auto mesh = m_scene->mMeshes[m_curMeshIdx];

    IZ_UINT ret = 0;

    if (mesh->HasPositions()) {
        ret |= 1 << izanagi::E_MSH_VTX_FMT_TYPE::E_MSH_VTX_FMT_TYPE_POS;
    }
    if (mesh->GetNumUVChannels() > 0) {
        ret |= 1 << izanagi::E_MSH_VTX_FMT_TYPE::E_MSH_VTX_FMT_TYPE_UV;
    }
    if (mesh->GetNumColorChannels() > 0) {
        ret |= 1 << izanagi::E_MSH_VTX_FMT_TYPE::E_MSH_VTX_FMT_TYPE_COLOR;
    }
    if (mesh->HasNormals()) {
        ret |= 1 << izanagi::E_MSH_VTX_FMT_TYPE::E_MSH_VTX_FMT_TYPE_NORMAL;
    }
    if (mesh->HasBones()) {
        ret |= 1 << izanagi::E_MSH_VTX_FMT_TYPE::E_MSH_VTX_FMT_TYPE_BLENDINDICES;
        ret |= 1 << izanagi::E_MSH_VTX_FMT_TYPE::E_MSH_VTX_FMT_TYPE_BLENDWEIGHT;
    }

    return ret;
}

IZ_BOOL AssimpImporter::GetVertex(
    IZ_UINT nIdx,
    izanagi::math::SVector4& vec,
    izanagi::E_MSH_VTX_FMT_TYPE type)
{
    // NOTE
    // スキンウエイト関係のパラメータは別処理される.

    IZ_ASSERT(m_curMeshIdx < m_scene->mNumMeshes);
    IZ_ASSERT(nIdx < m_curMeshVtxNum);

    auto mesh = m_scene->mMeshes[m_curMeshIdx];

    IZ_BOOL ret = IZ_TRUE;

    if (type == izanagi::E_MSH_VTX_FMT_TYPE::E_MSH_VTX_FMT_TYPE_POS) {
        vec.Set(
            mesh->mVertices[nIdx].x,
            mesh->mVertices[nIdx].y,
            mesh->mVertices[nIdx].z);
    }
    else if (type == izanagi::E_MSH_VTX_FMT_TYPE::E_MSH_VTX_FMT_TYPE_NORMAL) {
        vec.Set(
            mesh->mNormals[nIdx].x,
            mesh->mNormals[nIdx].y,
            mesh->mNormals[nIdx].z);
    }
    else if (type == izanagi::E_MSH_VTX_FMT_TYPE::E_MSH_VTX_FMT_TYPE_UV) {
        // TODO
        // multi textures.
        vec.Set(
            mesh->mTextureCoords[0][nIdx].x,
            mesh->mTextureCoords[0][nIdx].y,
            mesh->mTextureCoords[0][nIdx].z);
    }
    else if (type == izanagi::E_MSH_VTX_FMT_TYPE::E_MSH_VTX_FMT_TYPE_COLOR) {
        // TODO
        // muiti channel colors.
        vec.Set(
            mesh->mColors[0][nIdx].r,
            mesh->mColors[0][nIdx].g,
            mesh->mColors[0][nIdx].b,
            mesh->mColors[0][nIdx].a);
    }
    else {
        IZ_ASSERT(IZ_FALSE);
        ret = IZ_FALSE;
    }

    return ret;
}

void AssimpImporter::GetMaterialForMesh(
    IZ_UINT nMeshIdx,
    izanagi::S_MSH_MTRL& sMtrl)
{
    IZ_ASSERT(nMeshIdx < m_scene->mNumMeshes);

#if 0
    izanagi::tool::CString str;
    str.format("material_%d\0", nMeshIdx);

    sMtrl.name.SetString(str.c_str());
    sMtrl.nameKey = sMtrl.name.GetKeyValue();
#else
    const auto mesh = m_scene->mMeshes[nMeshIdx];

    // TODO
    // プロパティの中身を確認する.
    const auto mtrl = m_scene->mMaterials[mesh->mMaterialIndex];

    for (IZ_UINT i = 0; i < mtrl->mNumProperties; i++) {
        const auto prop = mtrl->mProperties[i];

        // TODO
    }
#endif
}

//////////////////////////////////
// For joint chunk.

void AssimpImporter::ExportJointCompleted()
{
}

IZ_BOOL AssimpImporter::BeginJoint()
{
    return IZ_TRUE;
}

void AssimpImporter::EndJoint()
{
}

IZ_UINT AssimpImporter::GetJointNum()
{
    return 0;
}

IZ_PCSTR AssimpImporter::GetJointName(IZ_UINT nIdx)
{
    return NULL;
}

IZ_INT AssimpImporter::GetJointParent(
    IZ_UINT nIdx,
    const std::vector<izanagi::S_SKL_JOINT>& tvJoint)
{
    return 0;
}

void AssimpImporter::GetJointInvMtx(
    IZ_UINT nIdx,
    izanagi::math::SMatrix44& mtx)
{
}

void AssimpImporter::GetJointTransform(
    IZ_UINT nIdx,
    const std::vector<izanagi::S_SKL_JOINT>& tvJoint,
    std::vector<SJointTransform>& tvTransform)
{
}

//////////////////////////////////
// For animation.

IZ_BOOL AssimpImporter::ReadBaseModel(IZ_PCSTR pszName)
{
    return IZ_FALSE;
}

IZ_UINT AssimpImporter::GetAnmSetNum()
{
    return 0;
}

IZ_BOOL AssimpImporter::BeginAnm(IZ_UINT nSetIdx)
{
    return IZ_FALSE;
}

IZ_BOOL AssimpImporter::EndAnm()
{
    return IZ_FALSE;
}

IZ_UINT AssimpImporter::GetAnmNodeNum()
{
    return 0;
}

IZ_UINT AssimpImporter::GetAnmChannelNum(IZ_UINT nNodeIdx)
{
    return 0;
}

IZ_BOOL AssimpImporter::GetAnmNode(
    IZ_UINT nNodeIdx,
    izanagi::S_ANM_NODE& sNode)
{
    return IZ_FALSE;
}

IZ_BOOL AssimpImporter::GetAnmChannel(
    IZ_UINT nNodeIdx,
    IZ_UINT nChannelIdx,
    izanagi::S_ANM_CHANNEL& sChannel)
{
    return IZ_FALSE;
}

IZ_BOOL AssimpImporter::GetAnmKey(
    IZ_UINT nNodeIdx,
    IZ_UINT nChannelIdx,
    IZ_UINT nKeyIdx,
    izanagi::S_ANM_KEY& sKey,
    std::vector<IZ_FLOAT>& tvValue)
{
    return IZ_FALSE;
}

//////////////////////////////////
// For material.

IZ_BOOL AssimpImporter::BeginMaterial()
{
    return IZ_FALSE;
}

IZ_BOOL AssimpImporter::EndMaterial()
{
    return IZ_FALSE;
}

IZ_UINT AssimpImporter::GetMaterialNum()
{
    IZ_UINT ret = m_scene->mNumMaterials;
    return ret;
}

IZ_BOOL AssimpImporter::GetMaterial(
    IZ_UINT nMtrlIdx,
    izanagi::S_MTRL_MATERIAL& sMtrl)
{
    return IZ_FALSE;
}

void AssimpImporter::GetMaterialTexture(
    IZ_UINT nMtrlIdx,
    IZ_UINT nTexIdx,
    izanagi::S_MTRL_TEXTURE& sTex)
{
}

void AssimpImporter::GetMaterialShader(
    IZ_UINT nMtrlIdx,
    IZ_UINT nShaderIdx,
    izanagi::S_MTRL_SHADER& sShader)
{
}

void AssimpImporter::GetMaterialParam(
    IZ_UINT nMtrlIdx,
    IZ_UINT nParamIdx,
    izanagi::S_MTRL_PARAM& sParam)
{
}

void AssimpImporter::GetMaterialParamValue(
    IZ_UINT nMtrlIdx,
    IZ_UINT nParamIdx,
    std::vector<IZ_FLOAT>& tvValue)
{
}
