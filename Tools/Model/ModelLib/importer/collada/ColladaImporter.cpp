#include <dom/domConstants.h>
#include "ColladaImporter.h"
#include "ColladaGeometry.h"
#include "ColladaJoint.h"
#include "ColladaAnimation.h"
#include "ColladaMaterial.h"
#include "izToolKit.h"

CColladaImporter::CColladaImporter()
{
	m_pDAE = IZ_NULL;
	m_pRoot = IZ_NULL;

	m_pGeometry = IZ_NULL;

	m_pBaseDAEForANm = IZ_NULL;
	m_pBaseRootForAnm = IZ_NULL;
}

IZ_BOOL CColladaImporter::Open(IZ_PCSTR pszName)
{
	IZ_ASSERT(m_pRoot == IZ_NULL);

#if 0
	m_pDAE = new DAE();
	int result = m_pDAE->load(pszName);
	VRETURN(result == DAE_OK);
#else
	m_pDAE = new DAE();
    m_pRoot = m_pDAE->open141(pszName);
	VRETURN(m_pRoot != IZ_NULL);
#endif

	return IZ_TRUE;
}

IZ_BOOL CColladaImporter::Close()
{
	SAFE_DELETE(m_pDAE);
	m_pRoot = IZ_NULL;

	m_pGeometry = IZ_NULL;

	SAFE_DELETE(m_pBaseDAEForANm);
	m_pBaseRootForAnm = IZ_NULL;

	CColladaAnimation::DeleteInstance();
	CColladaGeometry::DeleteInstance();
	CColladaJoint::DeleteInstance();
	CColladaMaterial::DeleteInstance();

	return IZ_TRUE;
}

//////////////////////////////////
// For geometry chunk.

void CColladaImporter::ExportGeometryCompleted()
{
	CColladaGeometry::GetInstance().Clear();
}

namespace {
	template <typename _T>
	inline _T* _GetElementFromLibrary(
		domCOLLADA* pDom,
		daeString strLibrary,
		daeString strElement)
	{
		IZ_ASSERT(pDom != IZ_NULL);

		domElement* pLib = pDom->getDescendant(strLibrary);
		IZ_ASSERT(pLib != IZ_NULL);

		_T* ret = (_T*)pLib->getDescendant(strElement);
		IZ_ASSERT(ret != IZ_NULL);

		return ret;
	}
}	// namespace

domGeometry* CColladaImporter::GetGeometry()
{
	if (m_pGeometry == IZ_NULL) {
		IZ_ASSERT(m_pRoot != IZ_NULL);

		// Find from visual_scene.		I
		domVisual_scene* pScene = _GetElementFromLibrary<domVisual_scene>(
									m_pRoot,
									"library_visual_scenes",
									"visual_scene");
		VRETURN(pScene != IZ_NULL);

		size_t nNodeCnt = pScene->getNode_array().getCount();

		for (size_t i = 0; i < nNodeCnt; i++) {
			domNodeRef pNode = pScene->getNode_array().get(i);

			if (pNode->getInstance_controller_array().getCount() > 0) {
				IZ_ASSERT(pNode->getInstance_controller_array().getCount() == 1);

				domInstance_controller* pInstanceController = pNode->getInstance_controller_array().get(0);
				xsAnyURI & urltype  = pInstanceController->getUrl();
				domElement* pElement = urltype.getElement();
				domController* pController = (domController*)pElement;

				domSkinRef pSkin = pController->getSkin();
				IZ_ASSERT(pSkin != IZ_NULL);

				domElement* pSrcElement = pSkin->getSource().getElement();
				IZ_ASSERT(pSrcElement != IZ_NULL);
                IZ_ASSERT(pSrcElement->getElementName() == ColladaDOM141::COLLADA_TYPE_GEOMETRY);

				return (domGeometry*)pSrcElement;
			}
		}

		// If can't find from visual_scene...
		m_pGeometry = _GetElementFromLibrary<domGeometry>(
						m_pRoot,
						"library_geometries",
						"geometry");
		IZ_ASSERT(m_pGeometry != IZ_NULL);
	}

	return m_pGeometry;
}

domMesh* CColladaImporter::GetMesh()
{
	m_pGeometry = GetGeometry();
	IZ_ASSERT(m_pGeometry != IZ_NULL);

	// Get mesh.
	domMesh* pMesh = m_pGeometry->getMesh();

	return pMesh;
}

void CColladaImporter::BeginMesh(IZ_UINT nIdx)
{
	domMesh* pMesh = GetMesh();

	CColladaGeometry::GetInstance().Begin(m_pDAE, pMesh, nIdx);
}

void CColladaImporter::EndMesh()
{
	CColladaGeometry::GetInstance().End();
}

IZ_UINT CColladaImporter::GetMeshNum()
{
	IZ_UINT ret = CColladaGeometry::GetInstance().GetMeshNum(m_pDAE);
	return ret;
}

// 指定されているメッシュに関連するスキニング情報を取得.
void CColladaImporter::GetSkinList(std::vector<SSkin>& tvSkinList)
{
	if (tvSkinList.empty()) {
		CColladaGeometry::GetInstance().GetSkinList(
			m_pDAE,
			tvSkinList);
	}
}

// 指定されているメッシュに含まれる三角形を取得.
IZ_UINT CColladaImporter::GetTriangles(std::vector<STri>& tvTriList)
{
	domMesh* pMesh = GetMesh();

	IZ_UINT nVtxNum = CColladaGeometry::GetInstance().GetTriangles(
						pMesh,
						tvTriList);

	return nVtxNum;
}

IZ_UINT CColladaImporter::GetSkinIdxAffectToVtx(IZ_UINT nVtxIdx)
{
	domMesh* pMesh = GetMesh();

	IZ_UINT ret = CColladaGeometry::GetInstance().GetSkinIdx(
					pMesh,
					nVtxIdx);
	return ret;
}

IZ_UINT CColladaImporter::GetVtxSize()
{
	IZ_UINT ret = CColladaGeometry::GetInstance().GetVtxSize();
	return ret;
}

IZ_UINT CColladaImporter::GetVtxFmt()
{
	IZ_UINT ret = CColladaGeometry::GetInstance().GetVtxFmt();
	return ret;
}

IZ_BOOL CColladaImporter::GetVertex(
	IZ_UINT nIdx,
	izanagi::math::SVector& vec,
	izanagi::E_MSH_VTX_FMT_TYPE type)
{
	domMesh* pMesh = GetMesh();

	IZ_BOOL ret = CColladaGeometry::GetInstance().GetVertex(
					pMesh,
					nIdx,
					vec,
					type);
	return ret;
}

void CColladaImporter::GetMaterialForMesh(
	IZ_UINT nIdx,
	izanagi::S_MSH_MTRL& sMtrl)
{
	CColladaGeometry::GetInstance().GetMaterialForMesh(nIdx, sMtrl);
}

//////////////////////////////////
// For joint chunk.

void CColladaImporter::ExportJointCompleted()
{
	CColladaJoint::GetInstance().Clear();
}

IZ_BOOL CColladaImporter::BeginJoint()
{
	// 念のため
	CColladaJoint::GetInstance().Clear();

	IZ_ASSERT(m_pRoot != IZ_NULL);

	domVisual_scene* pScene = _GetElementFromLibrary<domVisual_scene>(
								m_pRoot,
								"library_visual_scenes",
								"visual_scene");

	VRETURN(
		CColladaJoint::GetInstance().ReadJoint(pScene));

	return IZ_TRUE;
}

void CColladaImporter::EndJoint()
{
	CColladaJoint::GetInstance().Clear();
}

IZ_UINT CColladaImporter::GetJointNum()
{
	IZ_UINT ret = CColladaJoint::GetInstance().GetJointNum();
	return ret;
}

IZ_PCSTR CColladaImporter::GetJointName(IZ_UINT nIdx)
{
	IZ_PCSTR ret = CColladaJoint::GetInstance().GetJointName(nIdx);
	IZ_ASSERT(ret != IZ_NULL);
	return ret;
}

IZ_INT CColladaImporter::GetJointParent(
	IZ_UINT nIdx,
	const std::vector<izanagi::S_SKL_JOINT>& tvJoint)
{
	const izanagi::S_SKL_JOINT& sJoint = tvJoint[nIdx];

	IZ_INT ret = CColladaJoint::GetInstance().GetJointTree(
					sJoint,
					tvJoint);

	return ret;
}

void CColladaImporter::GetJointInvMtx(
	IZ_UINT nIdx,
	izanagi::math::SMatrix& mtx)
{
	CColladaJoint::GetInstance().GetJointInvMtx(
		nIdx,
		mtx);
}

void CColladaImporter::GetJointTransform(
	IZ_UINT nIdx,
	const std::vector<izanagi::S_SKL_JOINT>& tvJoint,
	std::vector<SJointTransform>& tvTransform)
{
	const izanagi::S_SKL_JOINT& sJoint = tvJoint[nIdx];

	CColladaJoint::GetInstance().GetJointTransform(
		sJoint,
		tvTransform);
}

//////////////////////////////////
// For animation.

IZ_BOOL CColladaImporter::ReadBaseModel(IZ_PCSTR pszName)
{
	m_pBaseDAEForANm = new DAE();
	m_pBaseRootForAnm = m_pBaseDAEForANm->open141(pszName);
	VRETURN(m_pBaseRootForAnm != IZ_NULL);
	return IZ_TRUE;
}

IZ_UINT CColladaImporter::GetAnmSetNum()
{
	IZ_ASSERT(m_pRoot != IZ_NULL);

	IZ_UINT ret = CColladaAnimation::GetInstance().GetAnmLibNum(m_pRoot);
	IZ_ASSERT(ret > 0);

	return ret;
}

IZ_BOOL CColladaImporter::BeginAnm(IZ_UINT nSetIdx)
{
	IZ_ASSERT(m_pRoot != IZ_NULL);
	IZ_ASSERT(m_pBaseRootForAnm != IZ_NULL);

	// Find from visual_scene.		I
	domVisual_scene* pScene = _GetElementFromLibrary<domVisual_scene>(
								m_pBaseRootForAnm,
								"library_visual_scenes",
								"visual_scene");
	VRETURN(pScene != IZ_NULL);

	IZ_BOOL ret = CColladaAnimation::GetInstance().Begin(
					m_pRoot,
					pScene,
					nSetIdx);

	SAFE_DELETE(m_pBaseDAEForANm);
	m_pBaseRootForAnm = IZ_NULL;

	return ret;
}

IZ_BOOL CColladaImporter::EndAnm()
{
	IZ_BOOL ret = CColladaAnimation::GetInstance().End();
	return ret;
}

IZ_UINT CColladaImporter::GetAnmNodeNum()
{
	IZ_UINT ret = CColladaAnimation::GetInstance().GetAnmNodeNum();
	IZ_ASSERT(ret > 0);

	return ret;
}

IZ_UINT CColladaImporter::GetAnmChannelNum(IZ_UINT nNodeIdx)
{
	IZ_UINT ret = CColladaAnimation::GetInstance().GetAnmChannelNum(nNodeIdx);
	IZ_ASSERT(ret > 0);

	return ret;
}

IZ_BOOL CColladaImporter::GetAnmNode(
	IZ_UINT nNodeIdx,
	izanagi::S_ANM_NODE& sNode)
{
	IZ_BOOL ret = CColladaAnimation::GetInstance().GetAnmNode(
					nNodeIdx,
					sNode);
	return ret;
}

IZ_BOOL CColladaImporter::GetAnmChannel(
	IZ_UINT nNodeIdx,
	IZ_UINT nChannelIdx,
	izanagi::S_ANM_CHANNEL& sChannel)
{
	IZ_BOOL ret = CColladaAnimation::GetInstance().GetAnmChannel(
					nNodeIdx,
					nChannelIdx,
					sChannel);
	return ret;
}

IZ_BOOL CColladaImporter::GetAnmKey(
	IZ_UINT nNodeIdx,
	IZ_UINT nChannelIdx,
	IZ_UINT nKeyIdx,
	izanagi::S_ANM_KEY& sKey,
	std::vector<IZ_FLOAT>& tvValue)
{
	IZ_BOOL ret =  CColladaAnimation::GetInstance().GetAnmKey(
					nNodeIdx,
					nChannelIdx,
					nKeyIdx,
					sKey,
					tvValue);
	return ret;
}

//////////////////////////////////
// For material.

IZ_BOOL CColladaImporter::BeginMaterial()
{
	CColladaMaterial::GetInstance().ReadImages(m_pRoot);
	CColladaMaterial::GetInstance().ReadMaterials(m_pRoot);

	return IZ_TRUE;
}

IZ_BOOL CColladaImporter::EndMaterial()
{
	// Nothing is done.
	return IZ_TRUE;
}

IZ_UINT CColladaImporter::GetMaterialNum()
{
	IZ_UINT ret = CColladaMaterial::GetInstance().GetMaterialNum();
	
	if (ret == 0) {
		IZ_BOOL result = BeginMaterial();
		VRETURN_VAL(result, 0);

		ret = CColladaMaterial::GetInstance().GetMaterialNum();
	}

	return ret;
}

IZ_BOOL CColladaImporter::GetMaterial(
	IZ_UINT nMtrlIdx,
	izanagi::S_MTRL_MATERIAL& sMtrl)
{
	IZ_BOOL ret = CColladaMaterial::GetInstance().GetMaterial(nMtrlIdx, sMtrl);
	return ret;
}

void CColladaImporter::GetMaterialTexture(
	IZ_UINT nMtrlIdx,
	IZ_UINT nTexIdx,
	izanagi::S_MTRL_TEXTURE& sTex)
{
	CColladaMaterial::GetInstance().GetMaterialTexture(
		nMtrlIdx,
		nTexIdx,
		sTex);
}

void CColladaImporter::GetMaterialShader(
	IZ_UINT nMtrlIdx,
	IZ_UINT nShaderIdx,
	izanagi::S_MTRL_SHADER& sShader)
{
	CColladaMaterial::GetInstance().GetMaterialShader(
		nMtrlIdx,
		nShaderIdx,
		sShader);
}

void CColladaImporter::GetMaterialParam(
	IZ_UINT nMtrlIdx,
	IZ_UINT nParamIdx,
	izanagi::S_MTRL_PARAM& sParam)
{
	CColladaMaterial::GetInstance().GetMaterialParam(
		nMtrlIdx,
		nParamIdx,
		sParam);
}

void CColladaImporter::GetMaterialParamValue(
	IZ_UINT nMtrlIdx,
	IZ_UINT nParamIdx,
	std::vector<IZ_FLOAT>& tvValue)
{
	CColladaMaterial::GetInstance().GetMaterialParamValue(
		nMtrlIdx,
		nParamIdx,
		tvValue);
}
