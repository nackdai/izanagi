#include "XFileImporter.h"
#include "XFileGeometry.h"
#include "XFileJoint.h"
#include "XFileAnimation.h"
#include "XFileMaterial.h"

CXFileImporter::CXFileImporter()
{
	m_BaseMdlParser = IZ_NULL;
}

IZ_BOOL CXFileImporter::IsTxtFormat(IZ_PCSTR pszName)
{
	return CXFileParser::IsTxtFormat(pszName);
}

IZ_BOOL CXFileImporter::Open(IZ_PCSTR pszName)
{
	IZ_BOOL ret = m_Parser.Parse(pszName);
	if (ret) {
		CXFileGeometry::GetInstance().Init(&m_Parser);
	}
	return ret;
}

IZ_BOOL CXFileImporter::Close()
{
	CXFileAnimation::DeleteInstance();
	CXFileGeometry::DeleteInstance();
	CXFileJoint::DeleteInstance();
	CXFileMaterial::DeleteInstance();

	return IZ_TRUE;
}

//////////////////////////////////
// For geometry chunk.

void CXFileImporter::ExportGeometryCompleted()
{
	CXFileGeometry::GetInstance().Clear();
}

void CXFileImporter::BeginMesh(IZ_UINT nIdx)
{
	m_nMeshIdx = nIdx;
}

void CXFileImporter::EndMesh()
{
}

IZ_UINT CXFileImporter::GetMeshNum()
{
	IZ_UINT ret = CXFileGeometry::GetInstance().GetMeshNum();
	return ret;
}

void CXFileImporter::GetSkinList(std::vector<SSkin>& tvSkinList)
{
	tvSkinList.clear();

	CXFileGeometry::GetInstance().GetSkinList(
		&m_Parser,
		m_nMeshIdx,
		tvSkinList);
}

IZ_UINT CXFileImporter::GetTriangles(std::vector<STri>& tvTriList)
{
	IZ_UINT ret = CXFileGeometry::GetInstance().GetTriangles(
					&m_Parser,
					m_nMeshIdx,
					tvTriList);
	return ret;
}

IZ_UINT CXFileImporter::GetSkinIdxAffectToVtx(IZ_UINT nVtxIdx)
{
	IZ_UINT ret = CXFileGeometry::GetInstance().GetSkinIdx(
					&m_Parser,
					m_nMeshIdx,
					nVtxIdx);
	return ret;
}

IZ_UINT CXFileImporter::GetVtxSize()
{
	IZ_UINT ret = CXFileGeometry::GetInstance().GetVtxSize(m_nMeshIdx);
	return ret;
}

IZ_UINT CXFileImporter::GetVtxFmt()
{
	IZ_UINT ret = CXFileGeometry::GetInstance().GetVtxFmt(m_nMeshIdx);
	return ret;
}

IZ_BOOL CXFileImporter::GetVertex(
	IZ_UINT nIdx,
	izanagi::SVector& vec,
	izanagi::E_MSH_VTX_FMT_TYPE type)
{
	IZ_BOOL ret = CXFileGeometry::GetInstance().GetVertex(
					m_nMeshIdx,
					nIdx,
					vec,
					type);
	return ret;
}

void CXFileImporter::GetMaterialForMesh(
	IZ_UINT nIdx,
	izanagi::S_MSH_MTRL& sMtrl)
{
	CXFileGeometry::GetInstance().GetMaterial(nIdx, sMtrl);
}

//////////////////////////////////
// For joint chunk.

void CXFileImporter::ExportJointCompleted()
{
	CXFileJoint::GetInstance().Clear();
}

IZ_BOOL CXFileImporter::BeginJoint()
{
	IZ_BOOL ret = CXFileJoint::GetInstance().Init(&m_Parser);
	return ret;
}

void CXFileImporter::EndJoint()
{
	CXFileJoint::GetInstance().Clear();
}

IZ_UINT CXFileImporter::GetJointNum()
{
	IZ_UINT ret = CXFileJoint::GetInstance().GetJointNum();
	return ret;
}

IZ_PCSTR CXFileImporter::GetJointName(IZ_UINT nIdx)
{
	IZ_PCSTR ret = CXFileJoint::GetInstance().GetJointName(nIdx);
	return ret;
}

IZ_INT CXFileImporter::GetJointParent(
	IZ_UINT nIdx,
	const std::vector<izanagi::S_SKL_JOINT>& tvJoint)
{
	IZ_INT ret = CXFileJoint::GetInstance().GetJointParent(nIdx);
	return ret;
}

void CXFileImporter::GetJointInvMtx(
	IZ_UINT nIdx,
	izanagi::SMatrix& mtx)
{
	CXFileJoint::GetInstance().GetJointInvMtx(nIdx,	mtx);
}

void CXFileImporter::GetJointTransform(
	IZ_UINT nIdx,
	const std::vector<izanagi::S_SKL_JOINT>& tvJoint,
	std::vector<SJointTransform>& tvTransform)
{
	CXFileJoint::GetInstance().GetJointTransform(nIdx, tvTransform);
}

//////////////////////////////////
// For animation.

IZ_BOOL CXFileImporter::ReadBaseModel(IZ_PCSTR pszName)
{
	VRETURN(m_BaseMdlParser == IZ_NULL);

	m_BaseMdlParser = new CXFileParser;
	VRETURN(m_BaseMdlParser->Parse(pszName));
	
	return IZ_TRUE;
}

IZ_UINT CXFileImporter::GetAnmSetNum()
{
	size_t ret = m_Parser.GetAnmSetList().size();
	return static_cast<IZ_UINT>(ret);
}

IZ_BOOL CXFileImporter::BeginAnm(IZ_UINT nSetIdx)
{
	VRETURN(nSetIdx < GetAnmSetNum());
	VRETURN(m_BaseMdlParser != IZ_NULL);

	IZ_BOOL ret = CXFileAnimation::GetInstance().Begin(
					m_Parser,
					m_BaseMdlParser->GetNodeList(),
					nSetIdx);

	return ret;
}

IZ_BOOL CXFileImporter::EndAnm()
{
	SAFE_DELETE(m_BaseMdlParser);
	return IZ_TRUE;
}

IZ_UINT CXFileImporter::GetAnmNodeNum()
{
	IZ_UINT ret = CXFileAnimation::GetInstance().GetAnmNodeNum();
	return ret;
}

IZ_UINT CXFileImporter::GetAnmChannelNum(IZ_UINT nNodeIdx)
{
	IZ_UINT ret = CXFileAnimation::GetInstance().GetAnmChannelNum(nNodeIdx);
	return ret;
}

IZ_BOOL CXFileImporter::GetAnmNode(
	IZ_UINT nNodeIdx,
	izanagi::S_ANM_NODE& sNode)
{
	IZ_BOOL ret = CXFileAnimation::GetInstance().GetAnmNode(nNodeIdx, sNode);
	return ret;
}

IZ_BOOL CXFileImporter::GetAnmChannel(
	IZ_UINT nNodeIdx,
	IZ_UINT nChannelIdx,
	izanagi::S_ANM_CHANNEL& sChannel)
{
	IZ_BOOL ret = CXFileAnimation::GetInstance().GetAnmChannel(
					nNodeIdx,
					nChannelIdx,
					sChannel);
	return ret;
}

IZ_BOOL CXFileImporter::GetAnmKey(
	IZ_UINT nNodeIdx,
	IZ_UINT nChannelIdx,
	IZ_UINT nKeyIdx,
	izanagi::S_ANM_KEY& sKey,
	std::vector<IZ_FLOAT>& tvValue)
{
	IZ_BOOL ret = CXFileAnimation::GetInstance().GetAnmKey(
					nNodeIdx,
					nChannelIdx,
					nKeyIdx,
					sKey,
					tvValue);
	return ret;
}

//////////////////////////////////
// For material.

void CXFileImporter::SetNameForInvalidMaterial(IZ_PCSTR pszName)
{
	CXFileMaterial::GetInstance().SetNameForInvalidMaterial(pszName);
}

IZ_BOOL CXFileImporter::BeginMaterial()
{
	IZ_BOOL ret = CXFileMaterial::GetInstance().Begin(&m_Parser);
	return ret;
}

IZ_BOOL CXFileImporter::EndMaterial()
{
	// Nothing is done.
	return IZ_TRUE;
}

IZ_UINT CXFileImporter::GetMaterialNum()
{
	IZ_UINT ret = CXFileMaterial::GetInstance().GetMaterialNum();
	return ret;
}

IZ_BOOL CXFileImporter::GetMaterial(
	IZ_UINT nMtrlIdx,
	izanagi::S_MTRL_MATERIAL& sMtrl)
{
	IZ_BOOL ret = CXFileMaterial::GetInstance().GetMaterial(nMtrlIdx, sMtrl);
	return ret;
}

void CXFileImporter::GetMaterialTexture(
	IZ_UINT nMtrlIdx,
	IZ_UINT nTexIdx,
	izanagi::S_MTRL_TEXTURE& sTex)
{
	CXFileMaterial::GetInstance().GetMaterialTexture(
		nMtrlIdx,
		nTexIdx,
		sTex);
}

void CXFileImporter::GetMaterialShader(
	IZ_UINT nMtrlIdx,
	IZ_UINT nShaderIdx,
	izanagi::S_MTRL_SHADER& sShader)
{
	CXFileMaterial::GetInstance().GetMaterialShader(
		nMtrlIdx,
		nShaderIdx,
		sShader);
}

void CXFileImporter::GetMaterialParam(
	IZ_UINT nMtrlIdx,
	IZ_UINT nParamIdx,
	izanagi::S_MTRL_PARAM& sParam)
{
	CXFileMaterial::GetInstance().GetMaterialParam(
		nMtrlIdx,
		nParamIdx,
		sParam);
}

void CXFileImporter::GetMaterialParamValue(
	IZ_UINT nMtrlIdx,
	IZ_UINT nParamIdx,
	std::vector<IZ_FLOAT>& tvValue)
{
	CXFileMaterial::GetInstance().GetMaterialParamValue(
		nMtrlIdx,
		nParamIdx,
		tvValue);
}
