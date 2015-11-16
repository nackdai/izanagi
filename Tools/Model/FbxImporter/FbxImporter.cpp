#include "FbxImporter.h"
#include "FbxDataManager.h"

#include <map>

// NOTE
// http://www.gamedev.net/page/resources/_/technical/graphics-programming-and-theory/how-to-work-with-fbx-sdk-r3582


CFbxImporter::CFbxImporter()
{
}

IZ_BOOL CFbxImporter::Open(IZ_PCSTR pszName)
{
    IZ_BOOL ret = FbxDataManager::Instance().Open(pszName);
    return ret;
}

IZ_BOOL CFbxImporter::Close()
{
    FbxDataManager::Instance().Close();

    return IZ_TRUE;
}

//////////////////////////////////
// For geometry chunk.

void CFbxImporter::ExportGeometryCompleted()
{
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
    return 0;
}

// スキニング情報を取得.
void CFbxImporter::GetSkinList(std::vector<SSkin>& tvSkinList)
{
}

// 指定されているメッシュに含まれる三角形を取得.
IZ_UINT CFbxImporter::GetTriangles(std::vector<STri>& tvTriList)
{
    return 0;
}

IZ_UINT CFbxImporter::GetSkinIdxAffectToVtx(IZ_UINT nVtxIdx)
{
    // NOTE
    // 頂点インデックスとスキン情報のインデックスは一致する.
    return nVtxIdx;
}

IZ_UINT CFbxImporter::GetVtxSize()
{
    return 0;
}

IZ_UINT CFbxImporter::GetVtxFmt()
{
    return 0;
}

IZ_BOOL CFbxImporter::GetVertex(
    IZ_UINT nIdx,
    izanagi::math::SVector4& vec,
    izanagi::E_MSH_VTX_FMT_TYPE type)
{
    return IZ_FALSE;
}

void CFbxImporter::GetMaterialForMesh(
    IZ_UINT nMeshIdx,
    izanagi::S_MSH_MTRL& sMtrl)
{
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
    return 0;
}

IZ_PCSTR CFbxImporter::GetJointName(IZ_UINT nIdx)
{
    return nullptr;
}

IZ_INT CFbxImporter::GetJointParent(
    IZ_UINT nIdx,
    const std::vector<izanagi::S_SKL_JOINT>& tvJoint)
{
    return 0;
}

void CFbxImporter::GetJointInvMtx(
    IZ_UINT nIdx,
    izanagi::math::SMatrix44& mtx)
{
}

void CFbxImporter::GetJointTransform(
    IZ_UINT nIdx,
    const std::vector<izanagi::S_SKL_JOINT>& tvJoint,
    std::vector<SJointTransform>& tvTransform)
{
}

//////////////////////////////////
// For animation.

IZ_BOOL CFbxImporter::ReadBaseModel(IZ_PCSTR pszName)
{
    return IZ_FALSE;
}

IZ_UINT CFbxImporter::GetAnmSetNum()
{
    return 0;
}

IZ_BOOL CFbxImporter::BeginAnm(IZ_UINT nSetIdx)
{
    return IZ_FALSE;
}

IZ_BOOL CFbxImporter::EndAnm()
{
    return IZ_FALSE;
}

IZ_UINT CFbxImporter::GetAnmNodeNum()
{
    return 0;
}

IZ_UINT CFbxImporter::GetAnmChannelNum(IZ_UINT nNodeIdx)
{
    return 0;
}

IZ_BOOL CFbxImporter::GetAnmNode(
    IZ_UINT nNodeIdx,
    izanagi::S_ANM_NODE& sNode)
{
    return IZ_FALSE;
}

IZ_BOOL CFbxImporter::GetAnmChannel(
    IZ_UINT nNodeIdx,
    IZ_UINT nChannelIdx,
    izanagi::S_ANM_CHANNEL& sChannel)
{
    return IZ_FALSE;
}

IZ_BOOL CFbxImporter::GetAnmKey(
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

IZ_BOOL CFbxImporter::BeginMaterial()
{
    return IZ_FALSE;
}

IZ_BOOL CFbxImporter::EndMaterial()
{
    return IZ_FALSE;
}

IZ_UINT CFbxImporter::GetMaterialNum()
{
    return IZ_FALSE;
}

IZ_BOOL CFbxImporter::GetMaterial(
    IZ_UINT nMtrlIdx,
    izanagi::S_MTRL_MATERIAL& sMtrl)
{
    return IZ_FALSE;
}

void CFbxImporter::GetMaterialTexture(
    IZ_UINT nMtrlIdx,
    IZ_UINT nTexIdx,
    izanagi::S_MTRL_TEXTURE& sTex)
{
}

void CFbxImporter::GetMaterialShader(
    IZ_UINT nMtrlIdx,
    IZ_UINT nShaderIdx,
    izanagi::S_MTRL_SHADER& sShader)
{
}

void CFbxImporter::GetMaterialParam(
    IZ_UINT nMtrlIdx,
    IZ_UINT nParamIdx,
    izanagi::S_MTRL_PARAM& sParam)
{
}

void CFbxImporter::GetMaterialParamValue(
    IZ_UINT nMtrlIdx,
    IZ_UINT nParamIdx,
    std::vector<IZ_FLOAT>& tvValue)
{
}
