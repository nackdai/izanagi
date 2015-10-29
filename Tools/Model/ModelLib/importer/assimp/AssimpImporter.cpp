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
        | aiPostProcessSteps::aiProcess_JoinIdenticalVertices
        | aiPostProcessSteps::aiProcess_RemoveRedundantMaterials);

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

#if 0
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
#else
    auto mesh = m_scene->mMeshes[m_curMeshIdx];
    m_curMeshVtxNum = mesh->mNumVertices;
#endif
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
            m_mapVtxToSkin.insert(std::make_pair(vtxId, tvSkinList.size()));

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

#if 0
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
#else
    auto it = m_mapVtxToSkin.find(nVtxIdx);
    if (it != m_mapVtxToSkin.end()) {
        auto ret = it->second;
        return ret;
    }
#endif

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

    // NOTE
    // skinのサイズは外でやるのでここではやらない

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
    
    // NOTE
    // skinのフォーマットは外でやるのでここではやらない

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

    IZ_BOOL ret = IZ_FALSE;;

    if (type == izanagi::E_MSH_VTX_FMT_TYPE::E_MSH_VTX_FMT_TYPE_POS) {
        if (mesh->HasPositions()) {
            ret = IZ_TRUE;
            vec.Set(
                mesh->mVertices[nIdx].x,
                mesh->mVertices[nIdx].y,
                mesh->mVertices[nIdx].z);
        }
    }
    else if (type == izanagi::E_MSH_VTX_FMT_TYPE::E_MSH_VTX_FMT_TYPE_NORMAL) {
        if (mesh->HasNormals()) {
            ret = IZ_TRUE;
            vec.Set(
                mesh->mNormals[nIdx].x,
                mesh->mNormals[nIdx].y,
                mesh->mNormals[nIdx].z);
        }
    }
    else if (type == izanagi::E_MSH_VTX_FMT_TYPE::E_MSH_VTX_FMT_TYPE_UV) {
        if (mesh->HasTextureCoords(0)) {
            ret = IZ_TRUE;

            // TODO
            // multi textures.
            vec.Set(
                mesh->mTextureCoords[0][nIdx].x,
                mesh->mTextureCoords[0][nIdx].y,
                mesh->mTextureCoords[0][nIdx].z);
        }
    }
    else if (type == izanagi::E_MSH_VTX_FMT_TYPE::E_MSH_VTX_FMT_TYPE_COLOR) {
        if (mesh->HasVertexColors(0)) {
            ret = IZ_TRUE;

            // TODO
            // muiti channel colors.
            vec.Set(
                mesh->mColors[0][nIdx].r,
                mesh->mColors[0][nIdx].g,
                mesh->mColors[0][nIdx].b,
                mesh->mColors[0][nIdx].a);
        }
    }
    else {
        //IZ_ASSERT(IZ_FALSE);
    }

    return ret;
}

void AssimpImporter::GetMaterialForMesh(
    IZ_UINT nMeshIdx,
    izanagi::S_MSH_MTRL& sMtrl)
{
    IZ_ASSERT(nMeshIdx < m_scene->mNumMeshes);

    const auto mesh = m_scene->mMeshes[nMeshIdx];

    const auto mtrl = m_scene->mMaterials[mesh->mMaterialIndex];

    aiString str;
    auto result = mtrl->Get("?mat.name", 0, 0, str);

    IZ_ASSERT(result == aiReturn::aiReturn_SUCCESS);

    sMtrl.name.SetString(str.C_Str());
    sMtrl.nameKey = sMtrl.name.GetKeyValue();
}

//////////////////////////////////
// For joint chunk.

void AssimpImporter::ExportJointCompleted()
{
    // Nothing to do.
}

void AssimpImporter::getNode(aiNode* node, IZ_INT id)
{
    for (IZ_UINT i = 0; i < node->mNumChildren; i++) {
        auto child = node->mChildren[i];

        Node nodeInfo;
        nodeInfo.node = child;
        nodeInfo.parent = id;

        IZ_INT nodeId = m_nodes.size();

        m_nodes.insert(std::make_pair(nodeId, nodeInfo));

        getNode(child, nodeId);
    }
}

IZ_BOOL AssimpImporter::BeginJoint()
{
    Node nodeInfo;
    nodeInfo.node = m_scene->mRootNode;
    nodeInfo.parent = -1;

    m_nodes.insert(std::make_pair(0, nodeInfo));

    getNode(m_scene->mRootNode, 0);

    // マトリクスを格納.
    auto it = m_nodes.begin();

    for (; it != m_nodes.end(); it++)
    {
        Node& node = it->second;

        // NOTE
        // Assimpは左掛け、izanagiは右掛けなので転置する.
        for (IZ_UINT i = 0; i < 4; i++) {
            for (IZ_UINT n = 0; n < 4; n++) {
                node.mtx.m[i][n] = node.node->mTransformation[n][i];
            }
        }

        // TODO
        if (node.parent == -1) {
            node.mtx.m[0][0] = 1.0f;
            node.mtx.m[1][1] = 1.0f;
            node.mtx.m[2][2] = 1.0f;
            node.mtx.m[3][3] = 1.0f;
        }

        m_mtx.push_back(node.mtx);
    }

#if 0
    // 親子関係を解決したマトリクスを構築.
    it = m_nodes.begin();

    for (; it != m_nodes.end(); it++)
    {
        auto id = it->first;
        const Node& node = it->second;

        if (node.parent >= 0) {
            const auto& mtxParent = m_mtx[node.parent];
            izanagi::math::SMatrix44::Mul(m_mtx[id], m_mtx[id], mtxParent);
        }
    }
#else
    for (IZ_UINT i = 0; i < m_scene->mNumMeshes; i++) {
        const aiMesh* mesh = m_scene->mMeshes[i];

        for (IZ_UINT n = 0; n < mesh->mNumBones; n++) {
            aiBone* bone = mesh->mBones[n];

            m_bones.insert(std::make_pair(bone->mName.C_Str(), bone));
        }
    }
#endif

    return IZ_TRUE;
}

void AssimpImporter::EndJoint()
{
    m_nodes.clear();
}

IZ_UINT AssimpImporter::GetJointNum()
{
    return m_nodes.size();
}

IZ_PCSTR AssimpImporter::GetJointName(IZ_UINT nIdx)
{
    auto it = m_nodes.find(nIdx);

    IZ_PCSTR ret = IZ_NULL;

    if (it != m_nodes.end()) {
        const Node& node = it->second;
        ret = node.node->mName.C_Str();
    }

    return ret;
}

IZ_INT AssimpImporter::GetJointParent(
    IZ_UINT nIdx,
    const std::vector<izanagi::S_SKL_JOINT>& tvJoint)
{
    auto it = m_nodes.find(nIdx);

    IZ_INT ret = -1;

    if (it != m_nodes.end()) {
        const Node& node = it->second;

        auto parentId = node.parent;

        if (parentId >= 0) {
            auto itParent = m_nodes.find(parentId);
            IZ_ASSERT(itParent != m_nodes.end());

            const Node& parentNode = itParent->second;

            if (parentNode.node == node.node->mParent) {
                ret = parentId;
            }
            else {
                IZ_ASSERT(IZ_FALSE);
            }
        }
    }

    return ret;
}

void AssimpImporter::GetJointInvMtx(
    IZ_UINT nIdx,
    izanagi::math::SMatrix44& mtx)
{
#if 0
    IZ_ASSERT(nIdx < m_mtx.size());

    const auto& orgMtx = m_mtx[nIdx];

    izanagi::math::SMatrix44::Inverse(mtx, orgMtx);
#else
    auto it = m_nodes.find(nIdx);

    if (it != m_nodes.end()) {
        const Node& node = it->second;

        auto itBone = m_bones.find(node.node->mName.C_Str());
        if (itBone != m_bones.end()) {
            const aiBone* bone = itBone->second;

            // NOTE
            // Assimpは左掛け、izanagiは右掛けなので転置する.
            for (IZ_UINT i = 0; i < 4; i++) {
                for (IZ_UINT n = 0; n < 4; n++) {
                    mtx.m[i][n] = bone->mOffsetMatrix[n][i];
                }
            }
        }
        else {
            izanagi::math::SMatrix44::SetUnit(mtx);
        }
    }
    else {
        izanagi::math::SMatrix44::SetUnit(mtx);
    }
#endif
}

namespace {
    // Break down matrix to trans, quartanion.
    void BreakDownMatrix(
        const izanagi::math::SMatrix44& mtx,
        izanagi::S_SKL_JOINT_POSE& pose)
    {
        // TODO
        pose.scale[0] = 1.0f;
        pose.scale[1] = 1.0f;
        pose.scale[2] = 1.0f;

        pose.trans[0] = mtx.v[3].x;
        pose.trans[1] = mtx.v[3].y;
        pose.trans[2] = mtx.v[3].z;

        izanagi::math::SQuat::QuatFromMatrix(
            pose.quat,
            mtx);
    }
}

void AssimpImporter::GetJointTransform(
    IZ_UINT nIdx,
    const std::vector<izanagi::S_SKL_JOINT>& tvJoint,
    std::vector<SJointTransform>& tvTransform)
{
    IZ_ASSERT(nIdx < GetJointNum());

    auto it = m_nodes.find(nIdx);
    IZ_ASSERT(it != m_nodes.end());

    const Node& node = it->second;

    izanagi::S_SKL_JOINT_POSE pose;
    BreakDownMatrix(node.mtx, pose);

    // For quat.
    if (pose.quat.x != 0.0f
        || pose.quat.y != 0.0f
        || pose.quat.z != 0.0f
        || pose.quat.w != 1.0f)
    {
        tvTransform.push_back(SJointTransform());
        SJointTransform& sTransform = tvTransform.back();

        sTransform.type = E_MDL_JOINT_TRANSFORM_QUATERNION;

        sTransform.param.push_back(pose.quat.x);
        sTransform.param.push_back(pose.quat.y);
        sTransform.param.push_back(pose.quat.z);
        sTransform.param.push_back(pose.quat.w);
    }

    // For trans.
    if (pose.trans[0] != 0.0f
        || pose.trans[1] != 0.0f
        || pose.trans[2] != 0.0f)
    {
        tvTransform.push_back(SJointTransform());
        SJointTransform& sTransform = tvTransform.back();

        sTransform.type = E_MDL_JOINT_TRANSFORM_TRANSLATE;

        sTransform.param.push_back(pose.trans[0]);
        sTransform.param.push_back(pose.trans[1]);
        sTransform.param.push_back(pose.trans[2]);
    }

    // TODO
    // scale
}

//////////////////////////////////
// For animation.

IZ_BOOL AssimpImporter::ReadBaseModel(IZ_PCSTR pszName)
{
    auto scene = m_importerForBase.ReadFile(
        std::string(pszName),
        aiPostProcessSteps::aiProcess_Triangulate
        | aiPostProcessSteps::aiProcess_SortByPType
        | aiPostProcessSteps::aiProcess_JoinIdenticalVertices
        | aiPostProcessSteps::aiProcess_RemoveRedundantMaterials);

    return (scene != nullptr);
}

IZ_UINT AssimpImporter::GetAnmSetNum()
{
    IZ_UINT ret = m_scene->mNumAnimations;

    return ret;
}

IZ_BOOL AssimpImporter::BeginAnm(IZ_UINT nSetIdx)
{
    m_curAnmIdx = nSetIdx;

    auto scene = m_importerForBase.GetScene();
    if (!scene) {
        scene = m_scene;
    }

    Node nodeInfo;
    nodeInfo.node = scene->mRootNode;
    nodeInfo.parent = -1;

    m_nodes.insert(std::make_pair(0, nodeInfo));

    getNode(scene->mRootNode, 0);

    return IZ_TRUE;
}

IZ_BOOL AssimpImporter::EndAnm()
{
    return IZ_TRUE;
}

IZ_UINT AssimpImporter::GetAnmNodeNum()
{
    IZ_ASSERT(m_curAnmIdx < m_scene->mNumAnimations);

    auto anm = m_scene->mAnimations[m_curAnmIdx];

    IZ_UINT ret = anm->mNumChannels;

    m_channels.resize(ret);

    return ret;
}

IZ_UINT AssimpImporter::GetAnmChannelNum(IZ_UINT nNodeIdx)
{
    IZ_ASSERT(m_curAnmIdx < m_scene->mNumAnimations);

    auto anm = m_scene->mAnimations[m_curAnmIdx];
    IZ_ASSERT(nNodeIdx < anm->mNumChannels);

    auto channel = anm->mChannels[nNodeIdx];

    AnmChannel& ch = m_channels[nNodeIdx];
    IZ_UINT pos = 0;

    IZ_UINT ret = 0;

    if (channel->mNumPositionKeys > 0) {
        IZ_BOOL isDiff = IZ_FALSE;

        auto prev = channel->mPositionKeys[0];

        for (IZ_UINT i = 1; i < channel->mNumPositionKeys; i++) {
            auto pos = channel->mPositionKeys[i];

            if (pos.mValue.x != prev.mValue.x
                || pos.mValue.y != prev.mValue.y
                || pos.mValue.z != prev.mValue.z)
            {
                isDiff = IZ_TRUE;
                break;
            }
        }

        if (isDiff) {
            ret++;
            ch.type[pos++] = izanagi::E_ANM_TRANSFORM_TYPE::E_ANM_TRANSFORM_TYPE_TRANSLATE;
        }
        else {
            // 強制書き換え...
            channel->mNumPositionKeys = 0;
        }
    }
    if (channel->mNumRotationKeys > 0) {
        IZ_BOOL isDiff = IZ_FALSE;

        auto prev = channel->mRotationKeys[0];

        for (IZ_UINT i = 1; i < channel->mNumRotationKeys; i++) {
            auto rot = channel->mRotationKeys[i];

            if (rot.mValue.x != prev.mValue.x
                || rot.mValue.y != rot.mValue.y
                || rot.mValue.z != rot.mValue.z
                || rot.mValue.w != rot.mValue.w)
            {
                isDiff = IZ_TRUE;
                break;
            }
        }

        if (isDiff) {
            ret++;
            ch.type[pos++] = izanagi::E_ANM_TRANSFORM_TYPE::E_ANM_TRANSFORM_TYPE_ROTATE;
        }
        else {
            // 強制書き換え...
            channel->mNumRotationKeys = 0;
        }
    }
    if (channel->mNumScalingKeys > 0) {
        IZ_BOOL isDiff = IZ_FALSE;

        auto prev = channel->mScalingKeys[0];

        for (IZ_UINT i = 1; i < channel->mNumScalingKeys; i++) {
            auto scale = channel->mScalingKeys[i];

            // TODO
#if 0
            if (scale.mValue.x != prev.mValue.x
                || scale.mValue.y != prev.mValue.y
                || scale.mValue.z != prev.mValue.z)
#else
            if (!izanagi::math::CMath::IsNearyEqual(scale.mValue.x, prev.mValue.x)
                || !izanagi::math::CMath::IsNearyEqual(scale.mValue.y, prev.mValue.y)
                || !izanagi::math::CMath::IsNearyEqual(scale.mValue.z, prev.mValue.z))
#endif
            {
                isDiff = IZ_TRUE;
                break;
            }
        }

        if (isDiff) {
            ret++;
            ch.type[pos++] = izanagi::E_ANM_TRANSFORM_TYPE::E_ANM_TRANSFORM_TYPE_SCALE;
        }
        else {
            // 強制書き換え...
            channel->mNumScalingKeys = 0;
        }
    }

    return ret;
}

IZ_BOOL AssimpImporter::GetAnmNode(
    IZ_UINT nNodeIdx,
    izanagi::S_ANM_NODE& sNode)
{
    auto scene = m_importer.GetScene();
    IZ_ASSERT(m_curAnmIdx < scene->mNumAnimations);

    auto anm = scene->mAnimations[m_curAnmIdx];
    IZ_ASSERT(nNodeIdx < anm->mNumChannels);

    auto channel = anm->mChannels[nNodeIdx];

    sNode.target.SetString(channel->mNodeName.C_Str());
    sNode.targetKey = sNode.target.GetKeyValue();

    IZ_BOOL ret = IZ_FALSE;

    auto it = m_nodes.begin();
    for (; it != m_nodes.end(); it++) {
        const Node& node = it->second;

        izanagi::S_ANM_NODE tmp;
        tmp.target.SetString(node.node->mName.C_Str());

        if (sNode.target == tmp.target) {
            sNode.targetIdx = it->first;

            ret = IZ_TRUE;
            break;
        }
    }

    IZ_ASSERT(ret);

    sNode.numChannels = GetAnmChannelNum(nNodeIdx);

    return ret;
}

IZ_BOOL AssimpImporter::GetAnmChannel(
    IZ_UINT nNodeIdx,
    IZ_UINT nChannelIdx,
    izanagi::S_ANM_CHANNEL& sChannel)
{
    auto scene = m_importer.GetScene();
    IZ_ASSERT(m_curAnmIdx < scene->mNumAnimations);

    auto anm = scene->mAnimations[m_curAnmIdx];
    IZ_ASSERT(nNodeIdx < anm->mNumChannels);

    auto channel = anm->mChannels[nNodeIdx];

    const AnmChannel& ch = m_channels[nNodeIdx];

    // NOTE
    // Rotation -> Translate -> Scale

    // Rotation
    if (ch.type[nChannelIdx] == izanagi::E_ANM_TRANSFORM_TYPE::E_ANM_TRANSFORM_TYPE_ROTATE) {
        sChannel.numKeys = channel->mNumRotationKeys;
        sChannel.interp = izanagi::E_ANM_INTERP_TYPE::E_ANM_INTERP_TYPE_SLERP;
        sChannel.type = izanagi::E_ANM_TRANSFORM_TYPE::E_ANM_TRANSFORM_TYPE_QUATERNION_XYZW;

        // TODO
        // Not used.
        //sChannel.stride

        return IZ_TRUE;
    }

    // Scale
    if (ch.type[nChannelIdx] == izanagi::E_ANM_TRANSFORM_TYPE::E_ANM_TRANSFORM_TYPE_SCALE) {
        sChannel.numKeys = channel->mNumScalingKeys;
        sChannel.interp = izanagi::E_ANM_INTERP_TYPE::E_ANM_INTERP_TYPE_LINEAR;
        sChannel.type = izanagi::E_ANM_TRANSFORM_TYPE::E_ANM_TRANSFORM_TYPE_SCALE_XYZ;

        // TODO
        // Not used.
        //sChannel.stride

        return IZ_TRUE;
    }

    // Translate
    if (ch.type[nChannelIdx] == izanagi::E_ANM_TRANSFORM_TYPE::E_ANM_TRANSFORM_TYPE_TRANSLATE) {
        sChannel.numKeys = channel->mNumPositionKeys;
        sChannel.interp = izanagi::E_ANM_INTERP_TYPE::E_ANM_INTERP_TYPE_LINEAR;
        sChannel.type = izanagi::E_ANM_TRANSFORM_TYPE::E_ANM_TRANSFORM_TYPE_TRANSLATE_XYZ;

        // TODO
        // Not used.
        //sChannel.stride

        return IZ_TRUE;
    }

    return IZ_FALSE;
}

IZ_BOOL AssimpImporter::GetAnmKey(
    IZ_UINT nNodeIdx,
    IZ_UINT nChannelIdx,
    IZ_UINT nKeyIdx,
    izanagi::S_ANM_KEY& sKey,
    std::vector<IZ_FLOAT>& tvValue)
{
    auto scene = m_importer.GetScene();
    IZ_ASSERT(m_curAnmIdx < scene->mNumAnimations);

    auto anm = scene->mAnimations[m_curAnmIdx];
    IZ_ASSERT(nNodeIdx < anm->mNumChannels);

    auto channel = anm->mChannels[nNodeIdx];

    const AnmChannel& ch = m_channels[nNodeIdx];

    // NOTE
    // Rotation -> Translate -> Scale

    // Rotation
    if (ch.type[nChannelIdx] == izanagi::E_ANM_TRANSFORM_TYPE::E_ANM_TRANSFORM_TYPE_ROTATE) {
        IZ_ASSERT(nKeyIdx < channel->mNumRotationKeys);
        const auto& quat = channel->mRotationKeys[nKeyIdx];

        sKey.keyTime = quat.mTime;
        sKey.numParams = 4;

        tvValue.push_back(quat.mValue.x);
        tvValue.push_back(quat.mValue.y);
        tvValue.push_back(quat.mValue.z);
        tvValue.push_back(quat.mValue.w);

        IZ_PRINTF("[%d]-[%d]-[%d]\n", nNodeIdx, nChannelIdx, nKeyIdx);
        IZ_PRINTF(" %f %f %f %f\n", quat.mValue.x, quat.mValue.y, quat.mValue.z, quat.mValue.w);

        return IZ_TRUE;
    }

    // Scale
    if (ch.type[nChannelIdx] == izanagi::E_ANM_TRANSFORM_TYPE::E_ANM_TRANSFORM_TYPE_SCALE) {
        IZ_ASSERT(nKeyIdx < channel->mNumScalingKeys);

        const auto& scale = channel->mScalingKeys[nKeyIdx];

        sKey.keyTime = scale.mTime;
        sKey.numParams = 3;

        tvValue.push_back(scale.mValue.x);
        tvValue.push_back(scale.mValue.y);
        tvValue.push_back(scale.mValue.z);

        return IZ_TRUE;
    }

    // Translate
    if (ch.type[nChannelIdx] == izanagi::E_ANM_TRANSFORM_TYPE::E_ANM_TRANSFORM_TYPE_TRANSLATE) {
        IZ_ASSERT(nKeyIdx < channel->mNumPositionKeys);
        const auto& pos = channel->mPositionKeys[nKeyIdx];

        sKey.keyTime = pos.mTime;
        sKey.numParams = 3;

        tvValue.push_back(pos.mValue.x);
        tvValue.push_back(pos.mValue.y);
        tvValue.push_back(pos.mValue.z);

        return IZ_TRUE;
    }

    return IZ_FALSE;
}

//////////////////////////////////
// For material.

IZ_BOOL AssimpImporter::BeginMaterial()
{
    return IZ_TRUE;
}

IZ_BOOL AssimpImporter::EndMaterial()
{
    return IZ_TRUE;
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
    IZ_ASSERT(nMtrlIdx < m_scene->mNumMaterials);

    const auto mtrl = m_scene->mMaterials[nMtrlIdx];

    // name
    {
        aiString str;
        auto result = mtrl->Get("?mat.name", 0, 0, str);

        IZ_ASSERT(result == aiReturn::aiReturn_SUCCESS);

        sMtrl.name.SetString(str.C_Str());
        sMtrl.keyMaterial = sMtrl.name.GetKeyValue();
    }

    sMtrl.numTex = 0;
    for (IZ_UINT i = 0; i < AI_TEXTURE_TYPE_MAX; i++) {
        sMtrl.numTex += mtrl->GetTextureCount((aiTextureType)i);
    }

    sMtrl.numParam = 0;
    sMtrl.paramBytes = 0;

    IZ_CHAR buf[32] = { 0 };

    std::vector<aiMaterialProperty*> props;

    for (IZ_UINT i = 0; i < mtrl->mNumProperties; i++) {
        auto prop = mtrl->mProperties[i];

        if (prop->mType != aiPropertyTypeInfo::aiPTI_String) {
            ::sprintf_s(buf, "%s\0", prop->mKey.C_Str());

            const char* name = nullptr;

            for (IZ_UINT n = 0; n < strlen(buf); n++) {
                if (buf[n] == '.') {
                    name = &buf[n + 1];
                    break;
                }
            }

            IZ_ASSERT(name);

            // TODO
            if (strcmp(name, "diffuse") == 0) {
                sMtrl.paramBytes += prop->mDataLength;
                props.push_back(prop);
            }
            else if (strcmp(name, "specular") == 0) {
                sMtrl.paramBytes += prop->mDataLength;
                props.push_back(prop);
            }
            else if (strcmp(name, "ambient") == 0) {
                sMtrl.paramBytes += prop->mDataLength;
                props.push_back(prop);
            }
        }
    }

    if (props.size() > 0) {
        sMtrl.numParam = props.size();
        m_props.insert(std::make_pair(nMtrlIdx, props));
    }

    return IZ_TRUE;
}

void AssimpImporter::GetMaterialTexture(
    IZ_UINT nMtrlIdx,
    IZ_UINT nTexIdx,
    izanagi::S_MTRL_TEXTURE& sTex)
{
    IZ_ASSERT(nMtrlIdx < m_scene->mNumMaterials);

    const auto mtrl = m_scene->mMaterials[nMtrlIdx];

    sTex.type.flags = 0;

    IZ_UINT texPos = 0;

    for (IZ_UINT i = 0; i < AI_TEXTURE_TYPE_MAX; i++) {
        aiTextureType type = (aiTextureType)i;

        auto cnt = mtrl->GetTextureCount(type);

        if (texPos <= nTexIdx && nTexIdx < texPos + cnt) {
            aiString path;

            auto idx = nTexIdx - texPos;

            mtrl->GetTexture(
                type,
                idx,
                &path);

            sTex.name.SetString(path.C_Str());
            sTex.key = sTex.name.GetKeyValue();

            switch (type) {
            case aiTextureType::aiTextureType_DIFFUSE:
                // Nothing to do.
                break;
            case aiTextureType::aiTextureType_NORMALS:
                sTex.type.isNormal = IZ_TRUE;
                break;
            case aiTextureType::aiTextureType_SPECULAR:
                sTex.type.isSpecular = IZ_TRUE;
                break;
            case aiTextureType::aiTextureType_OPACITY:
                sTex.type.isTranslucent = IZ_TRUE;
                break;
            case aiTextureType::aiTextureType_REFLECTION:
                sTex.type.isEnvironment = IZ_TRUE;
                break;
            default:
                IZ_ASSERT(IZ_FALSE);
                break;
            }

            break;
        }
        
        texPos += cnt;
    }
}

void AssimpImporter::GetMaterialShader(
    IZ_UINT nMtrlIdx,
    IZ_UINT nShaderIdx,
    izanagi::S_MTRL_SHADER& sShader)
{
    // NOTE
    // Maybe, assimp doesn't have shader name.

    sShader.name.SetString("DefaultShader");
    sShader.key = sShader.name.GetKeyValue();
}

void AssimpImporter::GetMaterialParam(
    IZ_UINT nMtrlIdx,
    IZ_UINT nParamIdx,
    izanagi::S_MTRL_PARAM& sParam)
{
    auto it = m_props.find(nMtrlIdx);

    if (it != m_props.end()) {
        auto& props = it->second;
        IZ_ASSERT(nParamIdx < props.size());

        IZ_CHAR buf[32] = { 0 };

        auto prop = props[nParamIdx];

        ::sprintf_s(buf, "%s\0", prop->mKey.C_Str());

        const char* name = nullptr;

        for (IZ_UINT n = 0; n < strlen(buf); n++) {
            if (buf[n] == '.') {
                name = &buf[n + 1];
                break;
            }
        }

        IZ_ASSERT(name);

        sParam.name.SetString(name);
        sParam.key = sParam.name.GetKeyValue();

        if (prop->mType == aiPropertyTypeInfo::aiPTI_Float) {
            // TODO
            if (prop->mDataLength == 16
                || prop->mDataLength == 32
                || prop->mDataLength == 48)
            {
                sParam.elements = prop->mDataLength / sizeof(izanagi::math::SVector4);
                sParam.type = izanagi::E_MTRL_PARAM_TYPE::E_MTRL_PARAM_TYPE_VECTOR;
            }
            else if (prop->mDataLength == 64) {
                sParam.elements = 1;
                sParam.type = izanagi::E_MTRL_PARAM_TYPE::E_MTRL_PARAM_TYPE_MATRIX;
            }
            else {
                sParam.elements = prop->mDataLength / sizeof(IZ_FLOAT);
                sParam.type = izanagi::E_MTRL_PARAM_TYPE::E_MTRL_PARAM_TYPE_FLOAT;
            }

            sParam.bytes = prop->mDataLength;
        }
        else if (prop->mType == aiPropertyTypeInfo::aiPTI_Integer) {
            sParam.elements = prop->mDataLength / sizeof(IZ_UINT);
            sParam.type = izanagi::E_MTRL_PARAM_TYPE::E_MTRL_PARAM_TYPE_UINT;
            sParam.bytes = prop->mDataLength;
        }
        else {
            IZ_ASSERT(IZ_FALSE);
        }
    }
}

void AssimpImporter::GetMaterialParamValue(
    IZ_UINT nMtrlIdx,
    IZ_UINT nParamIdx,
    std::vector<IZ_FLOAT>& tvValue)
{
    auto it = m_props.find(nMtrlIdx);

    if (it != m_props.end()) {
        auto& props = it->second;
        IZ_ASSERT(nParamIdx < props.size());

        auto prop = props[nParamIdx];
        IZ_ASSERT(prop->mType == aiPropertyTypeInfo::aiPTI_Float);

        IZ_UINT elements = prop->mDataLength / sizeof(IZ_FLOAT);

        IZ_FLOAT* data = (IZ_FLOAT*)prop->mData;

        for (IZ_UINT i = 0; i < elements; i++) {
            IZ_FLOAT value = data[i];
            tvValue.push_back(value);
        }
    }
}
