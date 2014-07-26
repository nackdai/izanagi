#include "PMDImporter.h"
#include "izToolKit.h"

CPmdImporter::CPmdImporter()
{
    m_CurMeshIdx = -1;
}

IZ_BOOL CPmdImporter::Open(IZ_PCSTR pszName)
{
    izanagi::CFileInputStream stream;
    VRETURN(stream.Open(pszName));

    // Check magic number
    SPmdHeader header;
    IZ_INPUT_READ_VRETURN(&stream, &header, 0, sizeof(header));

    stream.Close();

    IZ_BOOL ret = IZ_FALSE;

    if (header.magic[0] == 'P' && header.magic[1] == 'm'&& header.magic[2] == 'd')
    {
        ret = m_PmdLoader.Load(pszName);
    }
    else
    {
        ret = m_VmdLoader.Load(pszName);
    }

    return ret;
}

IZ_BOOL CPmdImporter::Close()
{
    m_PmdLoader.Clear();
    return IZ_TRUE;
}

//////////////////////////////////
// For geometry chunk.

void CPmdImporter::ExportGeometryCompleted()
{
    m_PmdLoader.ClearGeometryData();
}

void CPmdImporter::BeginMesh(IZ_UINT nIdx)
{
    IZ_ASSERT(!m_FaceIdxInMtrlList.empty());
    IZ_ASSERT(nIdx < m_FaceIdxInMtrlList.size());

    m_CurMeshIdx = nIdx;
}

void CPmdImporter::EndMesh()
{
    m_CurMeshIdx = -1;
}

IZ_UINT CPmdImporter::GetMeshNum()
{
    if (m_FaceIdxInMtrlList.empty())
    {
        IZ_UINT vtxPos = 0;

        for (IZ_UINT i = 0; i < m_PmdLoader.GetMtrlNum(); i++)
        {
            const SPmdMaterial& mtrl = m_PmdLoader.GetMtrl(i);

            // NOTE
            // １面あたり３頂点なので
            // 頂点位置を３で割れば、それは面の位置になる
            m_FaceIdxInMtrlList.push_back(vtxPos / 3);

            vtxPos += mtrl.faceVtxNum;
        }
    }

    // メッシュとマテリアルが１対１に結びつくので
    // メッシュ数 = マテリアル数 となる
    IZ_UINT ret = m_PmdLoader.GetMtrlNum();
    IZ_ASSERT(ret == m_FaceIdxInMtrlList.size());

    return ret;
}

void CPmdImporter::GetSkinList(std::vector<SSkin>& tvSkinList)
{
    IZ_UINT vtxNum = m_PmdLoader.GetVtxNum();

    for (IZ_UINT i = 0; i < vtxNum; i++)
    {
        const SPmdVertex& vtx = m_PmdLoader.GetVertex(i);

        SSkin skin;

        IZ_FLOAT weight[2] = 
        {
            IZ_MIN(1.0f, vtx.boneWeight * 0.01f),
            IZ_MIN(1.0f, (100 - vtx.boneWeight) * 0.01f),
        };

        skin.weight.push_back(weight[0]);
        skin.joint.push_back(vtx.boneIdx[0]);

        if (weight[1] > 0.0f)
        {
            skin.weight.push_back(weight[1]);
            skin.joint.push_back(vtx.boneIdx[1]);
        }

        tvSkinList.push_back(skin);
    }
}

IZ_UINT CPmdImporter::GetTriangles(std::vector<STri>& tvTriList)
{
    IZ_ASSERT(m_CurMeshIdx >= 0);

    // 指定されたメッシュセットの面の位置を取得
    const IZ_UINT facePos = m_FaceIdxInMtrlList[m_CurMeshIdx];

    // NOTE
    // １面あたり３頂点なので
    // 頂点位置を３で割れば、それは面の数になる
    const SPmdMaterial& mtrl = m_PmdLoader.GetMtrl(m_CurMeshIdx);
    const IZ_UINT faceNum = mtrl.faceVtxNum / 3;

    for (IZ_UINT i = 0; i < faceNum; i++)
    {
        IZ_UINT idx = facePos + i;
        const SPmdFace& face = m_PmdLoader.GetFace(idx);

        STri tri;
        {
            tri.vtx[0] = face.vtxIdx[0];
            tri.vtx[1] = face.vtxIdx[1];
            tri.vtx[2] = face.vtxIdx[2];
        }

        tvTriList.push_back(tri);
    }

    return mtrl.faceVtxNum;
}

IZ_UINT CPmdImporter::GetSkinIdxAffectToVtx(IZ_UINT nVtxIdx)
{
    IZ_ASSERT(nVtxIdx < m_PmdLoader.GetVtxNum());
    return nVtxIdx;
}

IZ_UINT CPmdImporter::GetVtxSize()
{
    IZ_UINT ret = izanagi::E_MSH_VTX_SIZE_POS;
    ret += izanagi::E_MSH_VTX_SIZE_NORMAL;
    ret += izanagi::E_MSH_VTX_SIZE_UV;
    return ret;
}

IZ_UINT CPmdImporter::GetVtxFmt()
{
    IZ_UINT ret = 1 << izanagi::E_MSH_VTX_FMT_TYPE_POS;
    ret |= 1 << izanagi::E_MSH_VTX_FMT_TYPE_NORMAL;
    ret |= 1 << izanagi::E_MSH_VTX_FMT_TYPE_UV;
    return ret;
}

IZ_BOOL CPmdImporter::GetVertex(
    IZ_UINT nIdx,
    izanagi::math::SVector4& vec,
    izanagi::E_MSH_VTX_FMT_TYPE type)
{
    // NOTE
    // スキンニングについては別であつかうので
    // ここでは考慮しない

    const SPmdVertex& vtx = m_PmdLoader.GetVertex(nIdx);

    IZ_BOOL ret = IZ_TRUE;

    switch (type)
    {
    case izanagi::E_MSH_VTX_FMT_TYPE_POS:
        vec.Set(vtx.pos[0], vtx.pos[1], vtx.pos[2], 1.0f);
        break;
    case izanagi::E_MSH_VTX_FMT_TYPE_NORMAL:
        vec.Set(vtx.normal[0], vtx.normal[1], vtx.normal[2], 0.0f);
        break;
    case izanagi::E_MSH_VTX_FMT_TYPE_UV:
        vec.Set(vtx.uv[0], vtx.uv[1], 0.0f, 0.0f);
        break;
    default:
        ret = IZ_FALSE;
        break;
    }

    return ret;
}

void CPmdImporter::GetMaterialForMesh(
    IZ_UINT nIdx,
    izanagi::S_MSH_MTRL& sMtrl)
{
    // NOTE
    // PMDのマテリアルは名前を持たないので
    // 適当に名前を付ける

    izanagi::tool::CString str;
    str.format("material_%d\0", nIdx);

    sMtrl.name.SetString(str.c_str());
    sMtrl.nameKey = sMtrl.name.GetKeyValue();
}

//////////////////////////////////
// For joint chunk.

void CPmdImporter::ExportJointCompleted()
{
}

IZ_BOOL CPmdImporter::BeginJoint()
{
    // 関節の姿勢を計算
    m_PmdLoader.ComputeJointPose();

    return IZ_TRUE;
}

void CPmdImporter::EndJoint()
{
    // Nothing is done...
}

IZ_UINT CPmdImporter::GetJointNum()
{
    IZ_UINT ret = m_PmdLoader.GetBoneNum();
    return ret;
}

IZ_PCSTR CPmdImporter::GetJointName(IZ_UINT nIdx)
{
    // TODO
#if 1
    static izanagi::tool::CString str;
    str.format("Joint_%d\0", nIdx);
    return str.c_str();
#else
    const SPmdBone& bone = m_PmdLoader.GetBone(nIdx);
    return bone.boneName;
#endif
}

IZ_INT CPmdImporter::GetJointParent(
    IZ_UINT nIdx,
    const std::vector<izanagi::S_SKL_JOINT>& tvJoint)
{
    const SPmdBone& bone = m_PmdLoader.GetBone(nIdx);
    IZ_INT ret = (bone.parentIdx == 0xFFFF ? -1 : bone.parentIdx);
    return ret;
}

void CPmdImporter::GetJointInvMtx(
    IZ_UINT nIdx,
    izanagi::math::SMatrix& mtx)
{
    const izanagi::math::SMatrix& mtxJoint = m_PmdLoader.GetMatrix(nIdx);
    izanagi::math::SMatrix::Inverse(mtx, mtxJoint);
}

void CPmdImporter::GetJointTransform(
    IZ_UINT nIdx,
    const std::vector<izanagi::S_SKL_JOINT>& tvJoint,
    std::vector<SJointTransform>& tvTransform)
{
    const izanagi::S_SKL_JOINT_POSE& pose = m_PmdLoader.GetPose(nIdx);

    // NOTE
    // translateのみを考えればいい

    SJointTransform transform;
    transform.type = E_MDL_JOINT_TRANSFORM_TRANSLATE;
    transform.param.push_back(pose.trans[0]);
    transform.param.push_back(pose.trans[1]);
    transform.param.push_back(pose.trans[2]);

    tvTransform.push_back(transform);
}

//////////////////////////////////
// For animation.

IZ_BOOL CPmdImporter::ReadBaseModel(IZ_PCSTR pszName)
{
    IZ_BOOL ret = m_PmdLoader.Load(pszName);
    return ret;
}

IZ_UINT CPmdImporter::GetAnmSetNum()
{
    // 常に１
    return 1;
}

IZ_BOOL CPmdImporter::BeginAnm(IZ_UINT nSetIdx)
{
    // NOTE
    // 含まれるモーション数は常に１なので無視する
    return m_VmdLoader.MakeFrameOrder(m_PmdLoader);
}

IZ_BOOL CPmdImporter::EndAnm()
{
    return IZ_TRUE;
}

IZ_UINT CPmdImporter::GetAnmNodeNum()
{
    IZ_UINT ret = m_PmdLoader.GetBoneNum();
    return ret;
}

IZ_UINT CPmdImporter::GetAnmChannelNum(IZ_UINT nNodeIdx)
{
    IZ_ASSERT(nNodeIdx < m_PmdLoader.GetBoneNum());

    IZ_UINT ret = 0;

    if (m_VmdLoader.IsValidTrans(nNodeIdx))
    {
        ret += 1;
    }
    if (m_VmdLoader.IsValidRotate(nNodeIdx))
    {
        ret += 1;
    }

    return ret;
}

IZ_BOOL CPmdImporter::GetAnmNode(
    IZ_UINT nNodeIdx,
    izanagi::S_ANM_NODE& sNode)
{
    const SPmdBone& bone = m_PmdLoader.GetBone(nNodeIdx);

    sNode.numChannels = GetAnmChannelNum(nNodeIdx);
    sNode.targetIdx = nNodeIdx;

    // TODO
    izanagi::tool::CString str;
    str.format("node_%d", nNodeIdx);

    sNode.target.SetString(str.c_str());
    sNode.targetKey = sNode.target.GetKeyValue();

    return IZ_TRUE;
}

IZ_BOOL CPmdImporter::GetAnmChannel(
    IZ_UINT nNodeIdx,
    IZ_UINT nChannelIdx,
    izanagi::S_ANM_CHANNEL& sChannel)
{
    // TODO
    // 面倒なのでチャンネルは常にtransとquatの２つ
    VRETURN(nChannelIdx < 2);

    // NOTE
    // There is only bezier interpolation in VMD.

    // NOTE
    // VMD has four points always for bezier.
    // -> A point has two params, (x,y).

    IZ_BOOL isValidTrans = m_VmdLoader.IsValidTrans(nNodeIdx);
    IZ_BOOL isValidRotate = m_VmdLoader.IsValidRotate(nNodeIdx);

    if (isValidTrans && nChannelIdx == 0)
    {
        // trans parameter is 1 per XYZ.
        sChannel.stride = 4 * 2 * 3 + 3;

        sChannel.type = izanagi::E_ANM_TRANSFORM_TYPE_TRANSLATE_XYZ;
        sChannel.interp = izanagi::E_ANM_INTERP_TYPE_BEZIER | izanagi::E_ANM_INTERP_TYPE_USER_CUSTOM;
    }
    else
    {
        // quat parameters are 4.
        sChannel.stride = 4 * 2 + 4;

        sChannel.type = izanagi::E_ANM_TRANSFORM_TYPE_QUATERNION_XYZW;
        sChannel.interp = izanagi::E_ANM_INTERP_TYPE_SLERP | izanagi::E_ANM_INTERP_TYPE_USER_CUSTOM;
    }

    sChannel.numKeys = static_cast<IZ_UINT16>(m_VmdLoader.GetFrameInfo(nNodeIdx).size());

    return IZ_TRUE;
}

namespace
{
    inline void _SetValue(
        const char params[],
        IZ_UINT idx,
        IZ_FLOAT attn,
        std::vector<IZ_FLOAT>& value)
    {
        value.push_back(0.0f);
        value.push_back(params[idx + 4 * 0] * attn);
        value.push_back(params[idx + 4 * 1] * attn);
        value.push_back(1.0f);

        value.push_back(0.0f);
        value.push_back(params[idx + 4 * 2] * attn);
        value.push_back(params[idx + 4 * 3] * attn);
        value.push_back(1.0f);
    }
}

IZ_BOOL CPmdImporter::GetAnmKey(
    IZ_UINT nNodeIdx,
    IZ_UINT nChannelIdx,
    IZ_UINT nKeyIdx,
    izanagi::S_ANM_KEY& sKey,
    std::vector<IZ_FLOAT>& tvValue)
{
    // TODO
    // 面倒なのでチャンネルは常にtransとquatの２つ
    VRETURN(nChannelIdx < 2);

    const SVmdFrame* frame = m_VmdLoader.GetFrameInfo(nNodeIdx, nKeyIdx);
    VRETURN(frame != IZ_NULL);

    sKey.stride = (nChannelIdx == 0 ? 3 : 1);
    sKey.keyTime = static_cast<IZ_FLOAT>(frame->frameNo);

    // NOTE
    // VMD has four points always for bezier.
    // -> A point has two params, (x,y).
    sKey.numParams = sKey.stride * 4 * 2;

    static const IZ_FLOAT div = 1.0f / 127.0f;

    IZ_BOOL isValidTrans = m_VmdLoader.IsValidTrans(nNodeIdx);
    IZ_BOOL isValidRotate = m_VmdLoader.IsValidRotate(nNodeIdx);

    if (isValidTrans && nChannelIdx == 0)
    {
        // trans

        sKey.numParams += 3;

        // X
        tvValue.push_back(frame->trans[0]);
        _SetValue(
            frame->interpParam,
            0, div,
            tvValue);

        // Y
        tvValue.push_back(frame->trans[1]);
        _SetValue(
            frame->interpParam,
            1, div,
            tvValue);

        // Z
        tvValue.push_back(frame->trans[2]);
        _SetValue(
            frame->interpParam,
            2, div,
            tvValue);
    }
    else
    {
        // quat

        sKey.numParams += 4;
        
        tvValue.push_back(frame->quat[0]);
        tvValue.push_back(frame->quat[1]);
        tvValue.push_back(frame->quat[2]);
        tvValue.push_back(frame->quat[3]);

        _SetValue(
            frame->interpParam,
            3, div,
            tvValue);
    }

    return IZ_TRUE;
}

//////////////////////////////////
// For material.

IZ_BOOL CPmdImporter::BeginMaterial()
{
    return IZ_TRUE;
}

IZ_BOOL CPmdImporter::EndMaterial()
{
    return IZ_TRUE;
}

IZ_UINT CPmdImporter::GetMaterialNum()
{
    return m_PmdLoader.GetMtrlNum();
}

#define PMD_MTRL_PARAM_NUM  (3)

IZ_BOOL CPmdImporter::GetMaterial(
    IZ_UINT nMtrlIdx,
    izanagi::S_MTRL_MATERIAL& sMtrl)
{
    IZ_ASSERT(nMtrlIdx < GetMaterialNum());

    const SPmdMaterial& pmdMtrl = m_PmdLoader.GetMtrl(nMtrlIdx);

    // 名前
    izanagi::tool::CString strName;
    strName.format("material_%d", nMtrlIdx);
    sMtrl.name.SetString(strName.c_str());

    sMtrl.keyMaterial = sMtrl.name.GetKeyValue();

    // テクスチャのトーン用のテクスチャが必ず１枚ある
    IZ_BOOL hasTex = (::strlen(pmdMtrl.texFileName) > 0);
    sMtrl.numTex = (hasTex ? 2 : 1);

    // パラメータ数は固定
    // diffuse, specular, ambient
    sMtrl.numParam = PMD_MTRL_PARAM_NUM;

    // パラメータのデータサイズはそれぞれで float * 4
    sMtrl.paramBytes = 3 * sizeof(IZ_FLOAT) * 4;

    return IZ_TRUE;
}

void CPmdImporter::GetMaterialTexture(
    IZ_UINT nMtrlIdx,
    IZ_UINT nTexIdx,
    izanagi::S_MTRL_TEXTURE& sTex)
{
    IZ_ASSERT(nMtrlIdx < GetMaterialNum());

    const SPmdMaterial& pmdMtrl = m_PmdLoader.GetMtrl(nMtrlIdx);

    IZ_BOOL hasTex = (::strlen(pmdMtrl.texFileName) > 0);
    IZ_UINT numTex = (hasTex ? 2 : 1);

    IZ_ASSERT(nTexIdx < numTex);

    if (nTexIdx == 0)
    {
        // ０番目はトーン用テクスチャ
        izanagi::tool::CString strTex;
        strTex.format("toon%02d.bmp", pmdMtrl.idxToon);
        sTex.name.SetString(strTex.c_str());
    }
    else
    {
        sTex.name.SetString(pmdMtrl.texFileName);
    }

    sTex.key = sTex.name.GetKeyValue();

    sTex.type.flags = 0;
}

void CPmdImporter::GetMaterialShader(
    IZ_UINT nMtrlIdx,
    IZ_UINT nShaderIdx,
    izanagi::S_MTRL_SHADER& sShader)
{
    IZ_ASSERT(nMtrlIdx < GetMaterialNum());

    const SPmdMaterial& pmdMtrl = m_PmdLoader.GetMtrl(nMtrlIdx);

    sShader.name.SetString("DefaultShader");
    sShader.key = sShader.name.GetKeyValue();
}

void CPmdImporter::GetMaterialParam(
    IZ_UINT nMtrlIdx,
    IZ_UINT nParamIdx,
    izanagi::S_MTRL_PARAM& sParam)
{
    IZ_ASSERT(nMtrlIdx < GetMaterialNum());

    const SPmdMaterial& pmdMtrl = m_PmdLoader.GetMtrl(nMtrlIdx);

    // パラメータ数は固定
    // diffuse, specular, ambient
    IZ_ASSERT(nParamIdx < PMD_MTRL_PARAM_NUM);

    switch (nParamIdx)
    {
    case 0:
        sParam.name.SetString("diffuse");
        break;
    case 1:
        sParam.name.SetString("specular");
        break;
    case 2:
        sParam.name.SetString("ambient");
        break;
    }

    sParam.key = sParam.name.GetKeyValue();

    sParam.type = izanagi::E_MTRL_PARAM_TYPE_VECTOR;

    sParam.elements = 1;
    sParam.bytes = sizeof(IZ_FLOAT) * 4;
}

void CPmdImporter::GetMaterialParamValue(
    IZ_UINT nMtrlIdx,
    IZ_UINT nParamIdx,
    std::vector<IZ_FLOAT>& tvValue)
{IZ_ASSERT(nMtrlIdx < GetMaterialNum());

    const SPmdMaterial& pmdMtrl = m_PmdLoader.GetMtrl(nMtrlIdx);

    // パラメータ数は固定
    // diffuse, specular, ambient
    IZ_ASSERT(nParamIdx < PMD_MTRL_PARAM_NUM);

    switch (nParamIdx)
    {
    case 0:
        tvValue.push_back(pmdMtrl.diffuse[0]);
        tvValue.push_back(pmdMtrl.diffuse[1]);
        tvValue.push_back(pmdMtrl.diffuse[2]);
        tvValue.push_back(1.0f);
        break;
    case 1:
        tvValue.push_back(pmdMtrl.specular[0]);
        tvValue.push_back(pmdMtrl.specular[1]);
        tvValue.push_back(pmdMtrl.specular[2]);
        tvValue.push_back(pmdMtrl.specularity);
        break;
    case 2:
        tvValue.push_back(pmdMtrl.ambient[0]);
        tvValue.push_back(pmdMtrl.ambient[1]);
        tvValue.push_back(pmdMtrl.ambient[2]);
        tvValue.push_back(1.0f);
        break;
    }
}
