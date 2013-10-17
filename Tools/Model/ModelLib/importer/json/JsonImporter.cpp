#include "JsonImporter.h"

CJsonImporter::CJsonImporter()
{
    m_FP = NULL;
    m_FileStream = NULL;
}

IZ_BOOL CJsonImporter::Open(IZ_PCSTR pszName)
{
    VRETURN(m_FileStream == NULL);
    VRETURN(m_FP == NULL);
    
    ::fopen_s(&m_FP, pszName, "rt");
    VRETURN(m_FP != NULL);

    m_FileStream = new rapidjson::FileStream(m_FP);
    
    IZ_BOOL ret = !m_Document.ParseStream<0>(*m_FileStream).HasParseError();
    IZ_ASSERT(ret);

    VRETURN(m_Geometry.Read(m_Document));

    return ret;
}

IZ_BOOL CJsonImporter::Close()
{
    SAFE_DELETE(m_FileStream);
    
    if (m_FP != NULL) {
        ::fclose(m_FP);
        m_FP = NULL;
    }

    return IZ_TRUE;
}

//////////////////////////////////
// For geometry chunk.

void CJsonImporter::ExportGeometryCompleted()
{
}

void CJsonImporter::BeginMesh(IZ_UINT nIdx)
{
}

void CJsonImporter::EndMesh()
{
}

IZ_UINT CJsonImporter::GetMeshNum()
{
    return 0;
}

// 指定されているメッシュに関連するスキニング情報を取得.
void CJsonImporter::GetSkinList(std::vector<SSkin>& tvSkinList)
{

}

// 指定されているメッシュに含まれる三角形を取得.
IZ_UINT CJsonImporter::GetTriangles(std::vector<STri>& tvTriList)
{
    return 0;
}

IZ_UINT CJsonImporter::GetSkinIdxAffectToVtx(IZ_UINT nVtxIdx)
{
    return 0;
}

IZ_UINT CJsonImporter::GetVtxSize()
{
    return 0;
}

IZ_UINT CJsonImporter::GetVtxFmt()
{
    return 0;
}

IZ_BOOL CJsonImporter::GetVertex(
    IZ_UINT nIdx,
    izanagi::math::SVector& vec,
    izanagi::E_MSH_VTX_FMT_TYPE type)
{
    return IZ_FALSE;
}

void CJsonImporter::GetMaterialForMesh(
    IZ_UINT nIdx,
    izanagi::S_MSH_MTRL& sMtrl)
{
}

//////////////////////////////////
// For joint chunk.

void CJsonImporter::ExportJointCompleted()
{
}

IZ_BOOL CJsonImporter::BeginJoint()
{
    return IZ_FALSE;
}

void CJsonImporter::EndJoint()
{
}

IZ_UINT CJsonImporter::GetJointNum()
{
    return 0;
}

IZ_PCSTR CJsonImporter::GetJointName(IZ_UINT nIdx)
{
    return NULL;
}

IZ_INT CJsonImporter::GetJointParent(
    IZ_UINT nIdx,
    const std::vector<izanagi::S_SKL_JOINT>& tvJoint)
{
    return 0;
}

void CJsonImporter::GetJointInvMtx(
    IZ_UINT nIdx,
    izanagi::math::SMatrix& mtx)
{
}

void CJsonImporter::GetJointTransform(
    IZ_UINT nIdx,
    const std::vector<izanagi::S_SKL_JOINT>& tvJoint,
    std::vector<SJointTransform>& tvTransform)
{
}

//////////////////////////////////
// For animation.

IZ_BOOL CJsonImporter::ReadBaseModel(IZ_PCSTR pszName)
{
    return IZ_FALSE;
}

IZ_UINT CJsonImporter::GetAnmSetNum()
{
    return 0;
}

IZ_BOOL CJsonImporter::BeginAnm(IZ_UINT nSetIdx)
{
    return IZ_FALSE;
}

IZ_BOOL CJsonImporter::EndAnm()
{
    return IZ_FALSE;
}

IZ_UINT CJsonImporter::GetAnmNodeNum()
{
    return 0;
}

IZ_UINT CJsonImporter::GetAnmChannelNum(IZ_UINT nNodeIdx)
{
    return 0;
}

IZ_BOOL CJsonImporter::GetAnmNode(
    IZ_UINT nNodeIdx,
    izanagi::S_ANM_NODE& sNode)
{
    return IZ_FALSE;
}

IZ_BOOL CJsonImporter::GetAnmChannel(
    IZ_UINT nNodeIdx,
    IZ_UINT nChannelIdx,
    izanagi::S_ANM_CHANNEL& sChannel)
{
    return IZ_FALSE;
}

IZ_BOOL CJsonImporter::GetAnmKey(
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

IZ_BOOL CJsonImporter::BeginMaterial()
{
    return IZ_FALSE;
}

IZ_BOOL CJsonImporter::EndMaterial()
{
    return IZ_FALSE;
}

IZ_UINT CJsonImporter::GetMaterialNum()
{
    return IZ_FALSE;
}

IZ_BOOL CJsonImporter::GetMaterial(
    IZ_UINT nMtrlIdx,
    izanagi::S_MTRL_MATERIAL& sMtrl)
{
    return IZ_FALSE;
}

void CJsonImporter::GetMaterialTexture(
    IZ_UINT nMtrlIdx,
    IZ_UINT nTexIdx,
    izanagi::S_MTRL_TEXTURE& sTex)
{
}

void CJsonImporter::GetMaterialShader(
    IZ_UINT nMtrlIdx,
    IZ_UINT nShaderIdx,
    izanagi::S_MTRL_SHADER& sShader)
{
}

void CJsonImporter::GetMaterialParam(
    IZ_UINT nMtrlIdx,
    IZ_UINT nParamIdx,
    izanagi::S_MTRL_PARAM& sParam)
{
}

void CJsonImporter::GetMaterialParamValue(
    IZ_UINT nMtrlIdx,
    IZ_UINT nParamIdx,
    std::vector<IZ_FLOAT>& tvValue)
{
}
