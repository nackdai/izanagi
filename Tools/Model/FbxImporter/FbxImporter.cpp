#include "FbxImporter.h"
#include "FbxDataManager.h"

#include <map>

// NOTE
// http://www.gamedev.net/page/resources/_/technical/graphics-programming-and-theory/how-to-work-with-fbx-sdk-r3582


CFbxImporter::CFbxImporter()
{
}

IZ_BOOL CFbxImporter::Open(IZ_PCSTR pszName, IZ_BOOL isOpenForAnm)
{
    IZ_BOOL ret = IZ_FALSE;
    m_dataMgr = new FbxDataManager();
    
    if (isOpenForAnm) {
        ret = m_dataMgr->OpenForAnm(pszName);
    }
    else {
        ret = m_dataMgr->Open(pszName);
    }

    return ret;
}

IZ_BOOL CFbxImporter::Close()
{
    m_dataMgr->Close();
    delete m_dataMgr;

    if (m_dataMgrBase) {
        m_dataMgrBase->Close();
        delete m_dataMgrBase;
    }

    return IZ_TRUE;
}

//////////////////////////////////
// For geometry chunk.

void CFbxImporter::ExportGeometryCompleted()
{
    m_posVtx = 0;
}

void CFbxImporter::BeginMesh(IZ_UINT nIdx)
{
    m_curMeshIdx = nIdx;
}

void CFbxImporter::EndMesh()
{
}

IZ_UINT CFbxImporter::GetMeshNum()
{
    m_dataMgr->LoadMesh();
    IZ_UINT ret = m_dataMgr->GetMeshNum();
    return ret;
}

// スキニング情報を取得.
void CFbxImporter::GetSkinList(std::vector<SSkin>& tvSkinList)
{
    if (tvSkinList.size() > 0) {
        return;
    }

    IZ_UINT vtxNum = m_dataMgr->GetVtxNum();

    tvSkinList.resize(vtxNum);

    for (IZ_UINT i = 0; i < vtxNum; i++)
    {
        std::vector<IZ_FLOAT> weight;
        std::vector<IZ_UINT> joint;

        m_dataMgr->GetSkinData(i, weight, joint);

        IZ_ASSERT(weight.size() == joint.size());

        for (IZ_UINT n = 0; n < weight.size(); n++)
        {
            tvSkinList[i].Add(joint[n], weight[n]);
        }
    }
}

// 指定されているメッシュに含まれる三角形を取得.
IZ_UINT CFbxImporter::GetTriangles(std::vector<STri>& tvTriList)
{
    const MeshSubset& mesh = m_dataMgr->GetMesh(m_curMeshIdx);

    for each (const Face& face in mesh.faces)
    {
        STri tri;

        tri.vtx[0] = face.vtx[0];
        tri.vtx[1] = face.vtx[1];
        tri.vtx[2] = face.vtx[2];

        tvTriList.push_back(tri);
    }

    IZ_UINT vtxNum = mesh.vtxNum;

    return vtxNum;
}

IZ_UINT CFbxImporter::GetSkinIdxAffectToVtx(IZ_UINT nVtxIdx)
{
    // NOTE
    // 頂点インデックスとスキン情報のインデックスは一致する.
    return nVtxIdx;
}

IZ_UINT CFbxImporter::GetVtxSize()
{
    // NOTE
    // skinのサイズは外でやるのでここではやらない

    auto& meshSet = m_dataMgr->GetMesh(m_curMeshIdx);

    IZ_UINT ret = 0;

    FbxMesh* mesh = meshSet.fbxMesh;

    // ポジションが存在しないことはないはず.
    ret += izanagi::E_MSH_VTX_SIZE::E_MSH_VTX_SIZE_POS;

    if (mesh->GetElementNormalCount() > 0) {
        ret += izanagi::E_MSH_VTX_SIZE::E_MSH_VTX_SIZE_NORMAL;
    }

    if (mesh->GetElementUVCount() > 0) {
        ret += izanagi::E_MSH_VTX_SIZE::E_MSH_VTX_SIZE_UV;
    }

    if (mesh->GetElementVertexColorCount() > 0) {
        ret += izanagi::E_MSH_VTX_SIZE::E_MSH_VTX_SIZE_COLOR;
    }

    return ret;
}

IZ_UINT CFbxImporter::GetVtxFmt()
{
    auto& meshSet = m_dataMgr->GetMesh(m_curMeshIdx);

    IZ_UINT ret = 0;

    FbxMesh* mesh = meshSet.fbxMesh;

    // ポジションが存在しないことはないはず.
    ret |= 1 << izanagi::E_MSH_VTX_FMT_TYPE::E_MSH_VTX_FMT_TYPE_POS;

    if (mesh->GetElementNormalCount() > 0) {
        ret |= 1 << izanagi::E_MSH_VTX_FMT_TYPE::E_MSH_VTX_FMT_TYPE_NORMAL;
    }

    if (mesh->GetElementUVCount() > 0) {
        ret |= 1 << izanagi::E_MSH_VTX_FMT_TYPE::E_MSH_VTX_FMT_TYPE_UV;
    }

    if (mesh->GetElementVertexColorCount() > 0) {
        ret |= 1 << izanagi::E_MSH_VTX_FMT_TYPE::E_MSH_VTX_FMT_TYPE_COLOR;
    }

    // NOTE
    // skinのフォーマットは外でやるのでここではやらない

    return ret;
}

namespace {
    IZ_BOOL GetVertexData(
        fbxsdk::FbxLayerElement::EMappingMode mappingMode,
        fbxsdk::FbxLayerElement::EReferenceMode referenceMode,
        IZ_UINT vtxIdx, IZ_UINT vtxCounter,
        std::function<void(IZ_UINT)> funcDirect,
        std::function<void(IZ_UINT)> funcIndex)
    {
        IZ_BOOL ret = IZ_FALSE;

        switch (mappingMode)
        {
        case FbxGeometryElement::eByControlPoint:
            switch (referenceMode)
            {
            case FbxGeometryElement::eDirect:
            {
                funcDirect(vtxIdx);
                ret = IZ_TRUE;
            }
                break;

            case FbxGeometryElement::eIndexToDirect:
            {
                funcIndex(vtxIdx);
                ret = IZ_TRUE;
            }
                break;

            default:
                throw std::exception("Invalid Reference");
            }
            break;

        case FbxGeometryElement::eByPolygonVertex:
            // NOTE
            // 頂点の順番でアクセスする場合.
            switch (referenceMode)
            {
            case FbxGeometryElement::eDirect:
            {
                funcDirect(vtxCounter);
                ret = IZ_TRUE;
            }
                break;

            case FbxGeometryElement::eIndexToDirect:
            {
                funcIndex(vtxCounter);
                ret = IZ_TRUE;
            }
                break;

            default:
                throw std::exception("Invalid Reference");
            }
            break;
        }

        return ret;
    }
}

IZ_BOOL CFbxImporter::GetVertex(
    IZ_UINT nIdx,
    izanagi::math::SVector4& vec,
    izanagi::E_MSH_VTX_FMT_TYPE type)
{
    const VertexData& vtx = m_dataMgr->GetVertex(nIdx);

    auto mesh = m_dataMgr->GetMesh(0).fbxMesh;

    if (type == izanagi::E_MSH_VTX_FMT_TYPE::E_MSH_VTX_FMT_TYPE_POS) {
        vec.x = static_cast<IZ_FLOAT>(vtx.pos.mData[0]);
        vec.y = static_cast<IZ_FLOAT>(vtx.pos.mData[1]);
        vec.z = static_cast<IZ_FLOAT>(vtx.pos.mData[2]);
        vec.w = 1.0f;

        return IZ_TRUE;
    }

    if (type == izanagi::E_MSH_VTX_FMT_TYPE::E_MSH_VTX_FMT_TYPE_NORMAL) {
        if (mesh->GetElementNormalCount() > 0) {
#if 0
            // TODO
            const FbxGeometryElementNormal* vtxNormal = mesh->GetElementNormal();

            IZ_UINT numDirect = vtxNormal->GetDirectArray().GetCount();
            IZ_UINT numIndex = vtxNormal->GetIndexArray().GetCount();

            auto mappingMode = vtxNormal->GetMappingMode();
            auto referenceMode = vtxNormal->GetReferenceMode();

            GetVertexData(
                mappingMode, referenceMode,
                realVtxIdx, countInFbxMesh,
                [&](IZ_UINT idx) {
                const FbxVector4& nml = vtxNormal->GetDirectArray().GetAt(idx);
                vec.x = static_cast<float>(nml.mData[0]);
                vec.y = static_cast<float>(nml.mData[1]);
                vec.z = static_cast<float>(nml.mData[2]);
            },
                [&](IZ_UINT idx) {
                int index = vtxNormal->GetIndexArray().GetAt(idx);
                const FbxVector4& nml = vtxNormal->GetDirectArray().GetAt(index);
                vec.x = static_cast<float>(nml.mData[0]);
                vec.y = static_cast<float>(nml.mData[1]);
                vec.z = static_cast<float>(nml.mData[2]);
            });
#else
            vec.x = static_cast<IZ_FLOAT>(vtx.nml.mData[0]);
            vec.y = static_cast<IZ_FLOAT>(vtx.nml.mData[1]);
            vec.z = static_cast<IZ_FLOAT>(vtx.nml.mData[2]);
#endif
            return IZ_TRUE;
        }
    }

    if (type == izanagi::E_MSH_VTX_FMT_TYPE::E_MSH_VTX_FMT_TYPE_UV) {
        if (mesh->GetElementUVCount() > 0) {
            vec.x = static_cast<IZ_FLOAT>(vtx.uv.mData[0]);
            vec.y = static_cast<IZ_FLOAT>(vtx.uv.mData[1]);

            return IZ_TRUE;
        }
    }

    if (type == izanagi::E_MSH_VTX_FMT_TYPE::E_MSH_VTX_FMT_TYPE_COLOR) {
        if (mesh->GetElementVertexColorCount() > 0) {
#if 0
            const FbxGeometryElementVertexColor* vtxClr = mesh->GetElementVertexColor();

            IZ_UINT numDirect = vtxClr->GetDirectArray().GetCount();
            IZ_UINT numIndex = vtxClr->GetIndexArray().GetCount();

            auto mappingMode = vtxClr->GetMappingMode();
            auto referenceMode = vtxClr->GetReferenceMode();

            GetVertexData(
                mappingMode, referenceMode,
                realVtxIdx, countInFbxMesh,
                [&](IZ_UINT idx) {
                const FbxColor& clr = vtxClr->GetDirectArray().GetAt(idx);
                vec.x = static_cast<float>(clr.mRed);
                vec.y = static_cast<float>(clr.mGreen);
                vec.z = static_cast<float>(clr.mBlue);
                vec.w = static_cast<float>(clr.mAlpha);
            },
                [&](IZ_UINT idx) {
                int index = vtxClr->GetIndexArray().GetAt(idx);
                const FbxColor& clr = vtxClr->GetDirectArray().GetAt(index);
                vec.x = static_cast<float>(clr.mRed);
                vec.y = static_cast<float>(clr.mGreen);
                vec.z = static_cast<float>(clr.mBlue);
                vec.w = static_cast<float>(clr.mAlpha);
            });
#else
            vec.x = static_cast<IZ_FLOAT>(vtx.clr.mRed);
            vec.y = static_cast<IZ_FLOAT>(vtx.clr.mGreen);
            vec.z = static_cast<IZ_FLOAT>(vtx.clr.mBlue);
            vec.w = static_cast<IZ_FLOAT>(vtx.clr.mAlpha);
#endif       
            return IZ_TRUE;
        }
    }

    return IZ_FALSE;
}

void CFbxImporter::GetMaterialForMesh(
    IZ_UINT nMeshIdx,
    izanagi::S_MSH_MTRL& sMtrl)
{
    const MeshSubset& mesh = m_dataMgr->GetMesh(nMeshIdx);

    sMtrl.name.SetString(mesh.mtrl->GetName());
    sMtrl.nameKey = sMtrl.name.GetKeyValue();
}

//////////////////////////////////
// For joint chunk.

void CFbxImporter::ExportJointCompleted()
{
}

IZ_BOOL CFbxImporter::BeginJoint()
{
    return IZ_TRUE;
}

void CFbxImporter::EndJoint()
{
}

IZ_UINT CFbxImporter::GetJointNum()
{
    IZ_UINT ret = m_dataMgr->GetNodeNum();
    return ret;
}

IZ_PCSTR CFbxImporter::GetJointName(IZ_UINT nIdx)
{
    FbxNode* node = m_dataMgr->GetNode(nIdx);
    return node->GetName();
}

IZ_INT CFbxImporter::GetJointParent(
    IZ_UINT nIdx,
    const std::vector<izanagi::S_SKL_JOINT>& tvJoint)
{
    FbxNode* node = m_dataMgr->GetNode(nIdx);

    const FbxNode* parent = node->GetParent();
    if (parent == NULL) {
        return -1;
    }

    IZ_INT ret = m_dataMgr->GetNodeIndex(parent);

    return ret;
}

void CFbxImporter::GetJointInvMtx(
    IZ_UINT nIdx,
    izanagi::math::SMatrix44& mtx)
{
    FbxNode* node = m_dataMgr->GetNode(nIdx);
    FbxCluster* cluster = m_dataMgr->GetClusterByNode(node);

    if (cluster) {
        FbxAMatrix& mtxGlobal = node->EvaluateGlobalTransform();
        FbxAMatrix& mtxLocal = node->EvaluateLocalTransform();

        FbxVector4 trans = node->LclTranslation;
        FbxVector4 rot = node->LclRotation;
        FbxVector4 scale = node->LclScaling;

        FbxAMatrix tmp(trans, rot, scale);

        FbxAMatrix mtxTransformLink;
        cluster->GetTransformLinkMatrix(mtxTransformLink);

        FbxAMatrix mtxTransform;
        cluster->GetTransformMatrix(mtxTransform);

        FbxAMatrix globalBindposeInverseMatrix = mtxTransformLink.Inverse();

        for (IZ_UINT i = 0; i < 4; i++) {
            for (IZ_UINT n = 0; n < 4; n++) {
                mtx.m[i][n] = static_cast<IZ_FLOAT>(globalBindposeInverseMatrix.Get(i, n));
            }
        }
    }
    else {
        izanagi::math::SMatrix44::SetUnit(mtx);
    }
}

void CFbxImporter::GetJointTransform(
    IZ_UINT nIdx,
    const std::vector<izanagi::S_SKL_JOINT>& tvJoint,
    std::vector<SJointTransform>& tvTransform)
{
    FbxNode* node = m_dataMgr->GetNode(nIdx);

    FbxAMatrix& mtxLocal = node->EvaluateLocalTransform();

    const FbxVector4 trans = mtxLocal.GetT();
    const FbxQuaternion quat = mtxLocal.GetQ();
    const FbxVector4 scale = mtxLocal.GetS();

    // For quat.
    if (quat.mData[0] != 0.0f
        || quat.mData[1] != 0.0f
        || quat.mData[2] != 0.0f
        || quat.mData[3] != 1.0f)
    {
        tvTransform.push_back(SJointTransform());
        SJointTransform& sTransform = tvTransform.back();

        sTransform.type = E_MDL_JOINT_TRANSFORM_QUATERNION;

        sTransform.param.push_back(static_cast<IZ_FLOAT>(quat.mData[0]));
        sTransform.param.push_back(static_cast<IZ_FLOAT>(quat.mData[1]));
        sTransform.param.push_back(static_cast<IZ_FLOAT>(quat.mData[2]));
        sTransform.param.push_back(static_cast<IZ_FLOAT>(quat.mData[3]));
    }

    // For trans.
    if (trans.mData[0] != 0.0f
        || trans.mData[1] != 0.0f
        || trans.mData[2] != 0.0f)
    {
        tvTransform.push_back(SJointTransform());
        SJointTransform& sTransform = tvTransform.back();

        sTransform.type = E_MDL_JOINT_TRANSFORM_TRANSLATE;

        sTransform.param.push_back(static_cast<IZ_FLOAT>(trans.mData[0]));
        sTransform.param.push_back(static_cast<IZ_FLOAT>(trans.mData[1]));
        sTransform.param.push_back(static_cast<IZ_FLOAT>(trans.mData[2]));
    }

    // TODO
    // scale
}

//////////////////////////////////
// For animation.

IZ_BOOL CFbxImporter::ReadBaseModel(IZ_PCSTR pszName)
{
#if 0
    m_dataMgrBase = new FbxDataManager();
    IZ_BOOL ret = m_dataMgrBase->Open(pszName);

    return ret;
#else
    return IZ_TRUE;
#endif
}

IZ_UINT CFbxImporter::GetAnmSetNum()
{
    // NOTE
    // １しか許さない.
    return 1;
}

IZ_BOOL CFbxImporter::BeginAnm(IZ_UINT nSetIdx)
{
    IZ_UINT m_curAnmIdx = nSetIdx;
    return IZ_TRUE;
}

IZ_BOOL CFbxImporter::EndAnm()
{
    return IZ_TRUE;
}

IZ_UINT CFbxImporter::GetAnmNodeNum()
{
    IZ_UINT ret = m_dataMgr->GetNodeNum();
    return ret;
}

IZ_UINT CFbxImporter::GetAnmChannelNum(IZ_UINT nNodeIdx)
{
    static const fbxsdk::FbxVector4 vOne(1.0f, 1.0f, 1.0f, 0.0f);
    static const fbxsdk::FbxQuaternion qZero(0.0f, 0.0f, 0.0f, 1.0f);

    IZ_UINT num = 0;

    auto node = m_dataMgr->GetNode(nNodeIdx);

    auto start = m_dataMgr->GetAnmStartFrame();
    auto stop = m_dataMgr->GetAnmStopFrame();

    fbxsdk::FbxAMatrix prevMtx;
    prevMtx.SetIdentity();

    AnmChannel channel;
    channel.nodeIdx = nNodeIdx;

    for (IZ_INT f = start; f < stop; f++) {
        FbxTime time;
        time.Set(FbxTime::GetOneFrameValue(FbxTime::eFrames60) * f);

        auto mtx = node->EvaluateLocalTransform(time);

        if (mtx != prevMtx) {
            auto t = mtx.GetT();    // translate.
            auto s = mtx.GetS();    // scale.
            auto q = mtx.GetQ();    // quaternion.
            auto r = mtx.GetR();    // rotation.

            IZ_UINT pos = 0;

            // NOTE
            // Rotation -> Scale -> Translate

            // Rotate.
            if (!(q == qZero)) {
                num++;
                channel.type[pos++] = ParamType::Rotate;
            }
            
            // Scale.
            if (!(s == vOne)) {
                num++;
                channel.type[pos++] = ParamType::Scale;
            }

            // Trans.
            if (!t.IsZero(3)) {
                num++;
                channel.type[pos++] = ParamType::Tranlate;
            }

            break;
        }

        prevMtx = mtx;
    }

    m_channels.push_back(channel);

    return num;
}

IZ_BOOL CFbxImporter::GetAnmNode(
    IZ_UINT nNodeIdx,
    izanagi::S_ANM_NODE& sNode)
{
    auto node = m_dataMgr->GetNode(nNodeIdx);

    sNode.targetIdx = nNodeIdx;
    sNode.target.SetString(node->GetName());
    sNode.targetKey = sNode.target.GetKeyValue();

    sNode.numChannels = GetAnmChannelNum(nNodeIdx);

    // NOTE
    // channelIdx は外部で設定される.

    return IZ_TRUE;
}

IZ_BOOL CFbxImporter::GetAnmChannel(
    IZ_UINT nNodeIdx,
    IZ_UINT nChannelIdx,
    izanagi::S_ANM_CHANNEL& sChannel)
{
    IZ_ASSERT(nNodeIdx < m_channels.size());

    auto node = m_dataMgr->GetNode(nNodeIdx);

    auto& channel = m_channels[nNodeIdx];
    IZ_ASSERT(channel.nodeIdx == nNodeIdx);

    // TODO
    // Not used.
    //sChannel.stride

    // NOTE
    // keyIdx は外部で設定される.


    if (!channel.isChecked)
    {
        auto start = m_dataMgr->GetAnmStartFrame();
        auto stop = m_dataMgr->GetAnmStopFrame();

        fbxsdk::FbxVector4 prevT;
        fbxsdk::FbxVector4 prevS(1.0f, 1.0f, 1.0f, 0.0f);
        fbxsdk::FbxQuaternion prevQ;

        for (IZ_INT f = start; f < stop; f++) {
            FbxTime time;
            time.Set(FbxTime::GetOneFrameValue(FbxTime::eFrames60) * f);

            auto mtx = node->EvaluateLocalTransform(time);

            auto t = mtx.GetT();    // translate.
            auto s = mtx.GetS();    // scale.
            auto q = mtx.GetQ();    // quaternion.
            auto r = mtx.GetR();    // rotation.

            // NOTE
            // Rotation -> Scale -> Translate

            // Rotate.
            if (!(q == prevQ)) {
                AnmKey key;
                key.key = f - start;

                key.value[0] = q.mData[0];
                key.value[1] = q.mData[1];
                key.value[2] = q.mData[2];
                key.value[3] = q.mData[3];

                channel.keys[ParamType::Rotate].push_back(key);
            }
            prevQ = q;

            // Scale.
            if (!(s == prevS)) {
                AnmKey key;
                key.key = f - start;

                key.value[0] = s.mData[0];
                key.value[1] = s.mData[1];
                key.value[2] = s.mData[2];
                key.value[3] = s.mData[3];

                channel.keys[ParamType::Scale].push_back(key);
            }
            prevS = s;

            // Trans.
            if (!(t == prevT)) {
                AnmKey key;
                key.key = f - start;

                key.value[0] = t.mData[0];
                key.value[1] = t.mData[1];
                key.value[2] = t.mData[2];
                key.value[3] = t.mData[3];

                channel.keys[ParamType::Tranlate].push_back(key);
            }
            prevT = t;
        }

        // NOTE
        // キーデータが１つだと補間できないので、あえて増やす.
        for (IZ_UINT i = 0; i < ParamType::Num; i++) {
            if (channel.keys[i].size() == 1) {
                auto key = channel.keys[i][0];
                key.key = stop - 1;
                channel.keys[i].push_back(key);
            }
        }
    }

    channel.isChecked = IZ_TRUE;

    auto type = channel.type[nChannelIdx];
    
    // NOTE
    // Rotation -> Scale -> Translate

    // Rotation
    if (type == ParamType::Rotate) {
        sChannel.numKeys = channel.keys[type].size();
        sChannel.interp = izanagi::E_ANM_INTERP_TYPE::E_ANM_INTERP_TYPE_SLERP;
        sChannel.type = izanagi::E_ANM_TRANSFORM_TYPE::E_ANM_TRANSFORM_TYPE_QUATERNION_XYZW;
        return IZ_TRUE;
    }

    // Scale
    if (type == ParamType::Scale) {
        sChannel.numKeys = channel.keys[type].size();
        sChannel.interp = izanagi::E_ANM_INTERP_TYPE::E_ANM_INTERP_TYPE_LINEAR;
        sChannel.type = izanagi::E_ANM_TRANSFORM_TYPE::E_ANM_TRANSFORM_TYPE_SCALE_XYZ;
        return IZ_TRUE;
    }

    // Translate
    if (type == ParamType::Tranlate) {
        sChannel.numKeys = channel.keys[type].size();
        sChannel.interp = izanagi::E_ANM_INTERP_TYPE::E_ANM_INTERP_TYPE_LINEAR;
        sChannel.type = izanagi::E_ANM_TRANSFORM_TYPE::E_ANM_TRANSFORM_TYPE_TRANSLATE_XYZ;
        return IZ_TRUE;
    }

    return IZ_FALSE;
}

IZ_BOOL CFbxImporter::GetAnmKey(
    IZ_UINT nNodeIdx,
    IZ_UINT nChannelIdx,
    IZ_UINT nKeyIdx,
    izanagi::S_ANM_KEY& sKey,
    std::vector<IZ_FLOAT>& tvValue)
{
    IZ_ASSERT(nNodeIdx < m_channels.size());

    auto node = m_dataMgr->GetNode(nNodeIdx);

    auto& channel = m_channels[nNodeIdx];
    IZ_ASSERT(channel.nodeIdx == nNodeIdx);

    auto type = channel.type[nChannelIdx];

    // TODO
    // 60FPS固定.
    static const IZ_FLOAT frame = 1.0f / 60.0f;

    IZ_ASSERT(nKeyIdx < channel.keys[type].size());
    const auto& key = channel.keys[type][nKeyIdx];

    if (nKeyIdx > 0) {
        IZ_ASSERT(key.key > channel.keys[type][nKeyIdx - 1].key);
    }

    sKey.keyTime = key.key * frame;

    // NOTE
    // Rotation -> Scale -> Translate

    // Rotation
    if (type == ParamType::Rotate) {
        sKey.numParams = 4;

        tvValue.push_back(key.value[0]);
        tvValue.push_back(key.value[1]);
        tvValue.push_back(key.value[2]);
        tvValue.push_back(key.value[3]);

        return IZ_TRUE;
    }

    // Scale
    if (type == ParamType::Scale) {
        sKey.numParams = 3;

        tvValue.push_back(key.value[0]);
        tvValue.push_back(key.value[1]);
        tvValue.push_back(key.value[2]);

        return IZ_TRUE;
    }

    // Translate
    if (type == ParamType::Tranlate) {
        sKey.numParams = 3;

        tvValue.push_back(key.value[0]);
        tvValue.push_back(key.value[1]);
        tvValue.push_back(key.value[2]);

        return IZ_TRUE;
    }

    return IZ_FALSE;
}

//////////////////////////////////
// For material.

IZ_BOOL CFbxImporter::BeginMaterial()
{
    return IZ_TRUE;
}

IZ_BOOL CFbxImporter::EndMaterial()
{
    return IZ_TRUE;
}

IZ_UINT CFbxImporter::GetMaterialNum()
{
    m_dataMgr->LoadMaterial();
    IZ_UINT ret = m_dataMgr->GetMaterialNum();
    return ret;
}

static const char* FbxMtrlParamNames[] = {
    fbxsdk::FbxSurfaceMaterial::sEmissive,
    fbxsdk::FbxSurfaceMaterial::sEmissiveFactor,

    fbxsdk::FbxSurfaceMaterial::sAmbient,
    fbxsdk::FbxSurfaceMaterial::sAmbientFactor,

    fbxsdk::FbxSurfaceMaterial::sDiffuse,
    fbxsdk::FbxSurfaceMaterial::sDiffuseFactor,

    fbxsdk::FbxSurfaceMaterial::sSpecular,
    fbxsdk::FbxSurfaceMaterial::sSpecularFactor,
    fbxsdk::FbxSurfaceMaterial::sShininess,

    fbxsdk::FbxSurfaceMaterial::sBump,
    fbxsdk::FbxSurfaceMaterial::sNormalMap,
    fbxsdk::FbxSurfaceMaterial::sBumpFactor,

    fbxsdk::FbxSurfaceMaterial::sTransparentColor,
    fbxsdk::FbxSurfaceMaterial::sTransparencyFactor,

    fbxsdk::FbxSurfaceMaterial::sReflection,
    fbxsdk::FbxSurfaceMaterial::sReflectionFactor,

    fbxsdk::FbxSurfaceMaterial::sDisplacementColor,
    fbxsdk::FbxSurfaceMaterial::sDisplacementFactor,

    fbxsdk::FbxSurfaceMaterial::sVectorDisplacementColor,
    fbxsdk::FbxSurfaceMaterial::sVectorDisplacementFactor,
};

void CFbxImporter::getLambertParams(
    void* mtrl, 
    std::vector<CFbxImporter::MaterialParam>& list)
{
    fbxsdk::FbxSurfaceLambert* lambert = (fbxsdk::FbxSurfaceLambert*)mtrl;

    if (lambert->Diffuse.IsValid()) {
        MaterialParam diffuse;
        diffuse.fbxMtrl = mtrl;
        diffuse.name = "diffuse";
        diffuse.values.push_back(lambert->Diffuse.Get().mData[0]);
        diffuse.values.push_back(lambert->Diffuse.Get().mData[1]);
        diffuse.values.push_back(lambert->Diffuse.Get().mData[2]);
        diffuse.values.push_back(1.0f);
        list.push_back(diffuse);
    }

    if (lambert->Ambient.IsValid()) {
        MaterialParam ambient;
        ambient.fbxMtrl = mtrl;
        ambient.name = "ambient";
        ambient.values.push_back(lambert->Ambient.Get().mData[0]);
        ambient.values.push_back(lambert->Ambient.Get().mData[1]);
        ambient.values.push_back(lambert->Ambient.Get().mData[2]);
        ambient.values.push_back(1.0f);
        list.push_back(ambient);
    }

    if (lambert->Emissive.IsValid()) {
        MaterialParam emmisive;
        emmisive.fbxMtrl = mtrl;
        emmisive.name = "emmisive";
        emmisive.values.push_back(lambert->Emissive.Get().mData[0]);
        emmisive.values.push_back(lambert->Emissive.Get().mData[1]);
        emmisive.values.push_back(lambert->Emissive.Get().mData[2]);
        emmisive.values.push_back(1.0f);
        list.push_back(emmisive);
    }
}

void CFbxImporter::getPhongParams(
    void* mtrl, 
    std::vector<CFbxImporter::MaterialParam>& list)
{
    getLambertParams(mtrl, list);

    fbxsdk::FbxSurfacePhong* phong = (fbxsdk::FbxSurfacePhong*)mtrl;

    if (phong->Specular.IsValid()) {
        MaterialParam specular;
        specular.fbxMtrl = mtrl;
        specular.name = "specular";
        specular.values.push_back(phong->Specular.Get().mData[0]);
        specular.values.push_back(phong->Specular.Get().mData[1]);
        specular.values.push_back(phong->Specular.Get().mData[2]);
        specular.values.push_back(1.0f);
        list.push_back(specular);
    }

    if (phong->Shininess.IsValid()) {
        MaterialParam shiness;
        shiness.fbxMtrl = mtrl;
        shiness.name = "shiness";
        shiness.values.push_back(phong->Shininess.Get());
        list.push_back(shiness);
    }
}

IZ_BOOL CFbxImporter::GetMaterial(
    IZ_UINT nMtrlIdx,
    izanagi::S_MTRL_MATERIAL& sMtrl)
{
    auto* fbxMtrl = m_dataMgr->GetMaterial(nMtrlIdx);

    sMtrl.name.SetString(fbxMtrl->GetName());
    sMtrl.keyMaterial = sMtrl.name.GetKeyValue();

    sMtrl.numTex = 0;

    sMtrl.numParam = 0;
    sMtrl.paramBytes = 0;

    IZ_BOOL ret = IZ_FALSE;

    // TODO
    // cgfx.
    auto implementation = GetImplementation(fbxMtrl, FBXSDK_IMPLEMENTATION_CGFX);

    if (implementation != nullptr) {
        ret = GetFbxMatrialByImplmentation(nMtrlIdx, sMtrl);
    }
    else {
        ret = GetFbxMatrial(nMtrlIdx, sMtrl);
    }

    return ret;
}

IZ_BOOL CFbxImporter::GetFbxMatrial(
    IZ_UINT nMtrlIdx,
    izanagi::S_MTRL_MATERIAL& sMtrl)
{
    auto* fbxMtrl = m_dataMgr->GetMaterial(nMtrlIdx);

    std::vector<MaterialTex> texList;

    // for tex.
    for (IZ_UINT i = 0; i < COUNTOF(FbxMtrlParamNames); i++)
    {
        std::string name(FbxMtrlParamNames[i]);

        fbxsdk::FbxProperty prop = fbxMtrl->FindProperty(name.c_str());
        if (!prop.IsValid()) {
            continue;
        }

        // NOTE
        // http://stackoverflow.com/questions/19634369/read-texture-filename-from-fbx-with-fbx-sdk-c
        // http://marupeke296.com/FBX_No7_TextureMaterial.html

        // プロパティが持っているレイヤードテクスチャの枚数をチェック.
        int layerNum = prop.GetSrcObjectCount<fbxsdk::FbxLayeredTexture>();

        if (layerNum > 0) {
            // TODO
        }
        else {
            int textureCount = prop.GetSrcObjectCount<fbxsdk::FbxTexture>();

            for (int n = 0; n < textureCount; n++)
            {
                fbxsdk::FbxTexture* texture = FbxCast<fbxsdk::FbxTexture>(prop.GetSrcObject<fbxsdk::FbxTexture>(n));

                MaterialTex tex;
                {
                    tex.fbxMtrl = fbxMtrl;
                    tex.paramName = name.c_str();
                    tex.texName = ((fbxsdk::FbxFileTexture*)texture)->GetRelativeFileName();
                    tex.type.flags = 0;
                }

                if (name == fbxsdk::FbxSurfaceMaterial::sSpecular) {
                    tex.type.isSpecular = IZ_TRUE;
                }
                else if (name == fbxsdk::FbxSurfaceMaterial::sNormalMap) {
                    tex.type.isNormal = IZ_TRUE;
                }
                else if (name == fbxsdk::FbxSurfaceMaterial::sTransparentColor) {
                    tex.type.isTranslucent = IZ_TRUE;
                }

                // TODO
                // 他の場合...

                texList.push_back(tex);
            }
        }
    }

    if (texList.size() > 0) {
        if (s_EnvParam.idxEnableTex >= 0
            && s_EnvParam.idxEnableTex < texList.size())
        {
            // 指定されたテクスチャ以外は無視する.
            IZ_UINT pos = 0;
            for (auto it = texList.begin(); it != texList.end(); it++, pos++) {
                if (s_EnvParam.idxEnableTex != pos) {
                    it = texList.erase(it);
                }
            }
        }

        m_mtrlTex.insert(std::make_pair(nMtrlIdx, texList));
        sMtrl.numTex += texList.size();
    }

    std::vector<MaterialParam> paramList;

    // for param.
    if (fbxMtrl->GetClassId().Is(fbxsdk::FbxSurfacePhong::ClassId)) {
        getPhongParams(fbxMtrl, paramList);
    }
    else if (fbxMtrl->GetClassId().Is(fbxsdk::FbxSurfaceLambert::ClassId)) {
        getLambertParams(fbxMtrl, paramList);
    }
    else {
        IZ_ASSERT(IZ_FALSE);
    }

    if (paramList.size()) {
        m_mtrlParam.insert(std::make_pair(nMtrlIdx, paramList));

        for (IZ_UINT i = 0; i < paramList.size(); i++)
        {
            const MaterialParam& param = paramList[i];

            sMtrl.paramBytes += sizeof(IZ_FLOAT) * param.values.size();
            sMtrl.numParam++;
        }
    }
    
    return IZ_TRUE;
}

IZ_BOOL CFbxImporter::GetFbxMatrialByImplmentation(
    IZ_UINT nMtrlIdx,
    izanagi::S_MTRL_MATERIAL& sMtrl)
{
    // NOTE
    // http://www.programmershare.com/3142984/

    auto* fbxMtrl = m_dataMgr->GetMaterial(nMtrlIdx);

    // TODO
    // cgfx.
    auto implementation = GetImplementation(fbxMtrl, FBXSDK_IMPLEMENTATION_CGFX);
    VRETURN(implementation != nullptr);

    auto rootTable = implementation->GetRootTable();
    auto entryCount = rootTable->GetEntryCount();

    std::vector<MaterialTex> texList;
    std::vector<MaterialParam> paramList;

    for (int i = 0; i < entryCount; ++i)
    {
        auto entry = rootTable->GetEntry(i);

        auto fbxProperty = fbxMtrl->FindPropertyHierarchical(entry.GetSource());
        if (!fbxProperty.IsValid()) {
            fbxProperty = fbxMtrl->RootProperty.FindHierarchical(entry.GetSource());
        }

        auto propName = fbxProperty.GetNameAsCStr();

        auto textureCount = fbxProperty.GetSrcObjectCount<FbxTexture>();

        if (textureCount > 0)
        {
            // for tex.

            std::string src = entry.GetSource();

            auto num = fbxProperty.GetSrcObjectCount<FbxFileTexture>();

            for (int n = 0; n < num; n++)
            {
                auto texFile = fbxProperty.GetSrcObject<FbxFileTexture>(n);
                std::string texName = texFile->GetFileName();
                texName = texName.substr(texName.find_last_of('/') + 1);

                auto texture = fbxProperty.GetSrcObject<FbxTexture>(n);

                MaterialTex tex;
                {
                    tex.fbxMtrl = fbxMtrl;
                    tex.paramName = propName;
                    tex.texName = texName;
                    tex.type.flags = 0;
                }

                if (src == "Maya|DiffuseTexture") {
                    // Nothing.
                }
                else if (src == "Maya|NormalTexture") {
                    tex.type.isNormal = IZ_TRUE;
                }
                else if (src == "Maya|SpecularTexture") {
                    tex.type.isSpecular = IZ_TRUE;
                }
                else if (src == "Maya|FalloffTexture") {
                    // TODO
                }
                else if (src == "Maya|ReflectionMapTexture") {
                    // TODO
                }

                texList.push_back(tex);
            }
        }
        else {
            // for param.

            auto dataType = fbxProperty.GetPropertyDataType().GetType();

            MaterialParam param;
            param.fbxMtrl = fbxMtrl;
            param.name = propName;

            if (dataType == fbxsdk::eFbxBool) {
                bool v = fbxProperty.Get<bool>();
                param.values.push_back(v);
            }
            else if (dataType == fbxsdk::eFbxInt || dataType == fbxsdk::eFbxEnum) {
                int v = fbxProperty.Get<int>();
                param.values.push_back(v);
            }
            else if (dataType == fbxsdk::eFbxUInt) {
                unsigned int v = fbxProperty.Get<unsigned int>();
                param.values.push_back(v);
            }
            else if (dataType == fbxsdk::eFbxFloat) {
                float v = fbxProperty.Get<float>();
                param.values.push_back(v);
            }
            else if (dataType == fbxsdk::eFbxDouble) {
                float v = fbxProperty.Get<double>();
                param.values.push_back(v);
            }
            else if (dataType == fbxsdk::eFbxDouble2) {
                FbxDouble2 v = fbxProperty.Get<FbxDouble2>();
                param.values.push_back(v.mData[0]);
                param.values.push_back(v.mData[1]);
            }
            else if (dataType == fbxsdk::eFbxDouble3) {
                FbxDouble3 v = fbxProperty.Get<FbxDouble3>();
                param.values.push_back(v.mData[0]);
                param.values.push_back(v.mData[1]);
                param.values.push_back(v.mData[2]);
            }
            else if (dataType == fbxsdk::eFbxDouble4) {
                FbxDouble4 v = fbxProperty.Get<FbxDouble4>();
                param.values.push_back(v.mData[0]);
                param.values.push_back(v.mData[1]);
                param.values.push_back(v.mData[2]);
                param.values.push_back(v.mData[3]);
            }
            else if (dataType == fbxsdk::eFbxDouble4x4) {
                FbxDouble4x4 v = fbxProperty.Get<FbxDouble4x4>();

                for (int i = 0; i < 4; i++) {
                    for (int n = 0; n < 4; n++) {
                        param.values.push_back(v.mData[i].mData[n]);
                    }
                }
            }
            else {
                IZ_ASSERT(IZ_FALSE);
            }

            if (param.values.size() > 0) {
                paramList.push_back(param);
            }
        }
    }

    if (texList.size() > 0) {
        if (s_EnvParam.idxEnableTex >= 0
            && s_EnvParam.idxEnableTex < texList.size())
        {
            // 指定されたテクスチャ以外は無視する.
            IZ_UINT pos = 0;
            for (auto it = texList.begin(); it != texList.end(); it++, pos++) {
                if (s_EnvParam.idxEnableTex != pos) {
                    it = texList.erase(it);
                }
            }
        }

        m_mtrlTex.insert(std::make_pair(nMtrlIdx, texList));
        sMtrl.numTex += texList.size();
    }

    if (paramList.size() > 0) {
        m_mtrlParam.insert(std::make_pair(nMtrlIdx, paramList));

        for (IZ_UINT i = 0; i < paramList.size(); i++)
        {
            const MaterialParam& param = paramList[i];

            sMtrl.paramBytes += sizeof(IZ_FLOAT)* param.values.size();
            sMtrl.numParam++;
        }
    }

    return IZ_TRUE;
}

void CFbxImporter::GetMaterialTexture(
    IZ_UINT nMtrlIdx,
    IZ_UINT nTexIdx,
    izanagi::S_MTRL_TEXTURE& sTex)
{
    const auto& tex = m_mtrlTex[nMtrlIdx][nTexIdx];

    sTex.name.SetString(tex.texName.c_str());
    sTex.key = sTex.name.GetKeyValue();

    sTex.type = tex.type;
}

void CFbxImporter::GetMaterialShader(
    IZ_UINT nMtrlIdx,
    IZ_UINT nShaderIdx,
    izanagi::S_MTRL_SHADER& sShader)
{
    // NOTE
    // マテリアルごとに１つのみぽい.
    IZ_ASSERT(nShaderIdx == 0);

    auto* fbxMtrl = m_dataMgr->GetMaterial(nMtrlIdx);

    auto implementation = GetImplementation(fbxMtrl, FBXSDK_IMPLEMENTATION_CGFX);

    if (implementation != nullptr) {
        sShader.name.SetString(implementation->GetName());
    }
    else {
        FbxString shading = fbxMtrl->ShadingModel.Get();

        sShader.name.SetString((const char*)shading);
    }

    if (!s_EnvParam.nameShader.empty()) {
        // 指定されたシェーダ名で強制上書き
        sShader.name.SetString(s_EnvParam.nameShader.c_str());
    }

    sShader.key = sShader.name.GetKeyValue();
}

void CFbxImporter::GetMaterialParam(
    IZ_UINT nMtrlIdx,
    IZ_UINT nParamIdx,
    izanagi::S_MTRL_PARAM& sParam)
{
    const auto& param = m_mtrlParam[nMtrlIdx][nParamIdx];

    sParam.name.SetString(param.name.c_str());
    sParam.key = sParam.name.GetKeyValue();

    if (param.values.size() == 4)
    {
        sParam.elements = param.values.size();
        sParam.type = izanagi::E_MTRL_PARAM_TYPE::E_MTRL_PARAM_TYPE_VECTOR;
    }
    else if (param.values.size() == 16) {
        sParam.elements = param.values.size();
        sParam.type = izanagi::E_MTRL_PARAM_TYPE::E_MTRL_PARAM_TYPE_MATRIX;
    }
    else {
        sParam.elements = param.values.size();
        sParam.type = izanagi::E_MTRL_PARAM_TYPE::E_MTRL_PARAM_TYPE_FLOAT;
    }

    sParam.bytes = sizeof(IZ_FLOAT) * sParam.elements;
}

void CFbxImporter::GetMaterialParamValue(
    IZ_UINT nMtrlIdx,
    IZ_UINT nParamIdx,
    std::vector<IZ_FLOAT>& tvValue)
{
    const auto& param = m_mtrlParam[nMtrlIdx][nParamIdx];

    std::copy(
        param.values.begin(),
        param.values.end(),
        std::back_inserter(tvValue));
}
