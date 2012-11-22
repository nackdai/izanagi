#include "ColladaGeometry.h"
#include "izToolKit.h"

//////////////////////////////////////////////////

bool CColladaGeometry::SVtx::operator ==(const CColladaGeometry::SVtx &rhs)
{
	bool ret = true;
	for (size_t i = 0; i < input.size(); i++) {
		ret = (input[i] == rhs.input[i]);
		if (!ret) {
			break;
		}
	}
	return ret;
}

////////////////////////////////////////////

bool CColladaGeometry::SVtxFmt::operator==(IZ_PCSTR pszSemantic)
{
	IZ_BOOL ret = izanagi::tool::CString::CmpStr(
					semantic,
					pszSemantic);

	// Avoid warning which is casting IZ_BOOL to bool...
	return (ret == IZ_TRUE);
}

bool CColladaGeometry::SVtxFmt::operator<(const SVtxFmt& rhs) const
{
	return (type < rhs.type);
}

#if 0
bool CColladaGeometry::SVtxFmt::operator==(izanagi::E_MSH_VTX_FMT_TYPE _type)
{
	return (type == _type);
}
#endif

//////////////////////////////////////////////////

CColladaGeometry* CColladaGeometry::s_pInstance = IZ_NULL;

CColladaGeometry& CColladaGeometry::GetInstance()
{
	if (s_pInstance == IZ_NULL) {
		s_pInstance = new CColladaGeometry();
	}
	return *s_pInstance;
}

void CColladaGeometry::DeleteInstance()
{
	SAFE_DELETE(s_pInstance);
}

void CColladaGeometry::Clear()
{
	m_VtxList.clear();
	m_VtxFmtList.clear();
}

void CColladaGeometry::Begin(
	DAE* pDAE,
	domMesh* pMesh,
	IZ_UINT nMeshIdx)
{
	m_nMeshSubsetPos = 0;

	// メッシュのプリミティブタイプを取得
	m_PrimType = GetPrimType(
					pMesh,
					nMeshIdx,
					&m_nMeshSubsetPos);

	// Read vertex format.
	switch (m_PrimType) {
	case E_PRIM_TYPE_TRIANGLES:
		{
			domTrianglesRef triangles = pMesh->getTriangles_array().get(m_nMeshSubsetPos);
			ReadVtxFmt(pMesh, triangles);
			ReadMeshMtrl(pDAE, nMeshIdx, triangles);
		}
		break;
	case E_PRIM_TYPE_POLYLIST:
		{
			domPolylistRef polylist = pMesh->getPolylist_array().get(m_nMeshSubsetPos);
			ReadVtxFmt(pMesh, polylist);
			ReadMeshMtrl(pDAE, nMeshIdx, polylist);
		}
		break;
	case E_PRIM_TYPE_POLYGONS:
		{
			domPolygonsRef polygon = pMesh->getPolygons_array().get(m_nMeshSubsetPos);
			// TODO
			IZ_ASSERT(IZ_FALSE);
		}
		break;
	case E_PRIM_TYPE_TRISTRIP:
		{
			domTristripsRef tristrip = pMesh->getTristrips_array().get(m_nMeshSubsetPos);
			// TODO
			IZ_ASSERT(IZ_FALSE);
		}
		break;
	case E_PRIM_TYPE_TRIFAN:
		{
			domTrifansRef trifan = pMesh->getTrifans_array().get(m_nMeshSubsetPos);
			// TODO
			IZ_ASSERT(IZ_FALSE);
		}
		break;
	default:
		IZ_ASSERT(IZ_FALSE);
		break;
	}	// end of switch
}

void CColladaGeometry::End()
{
	m_VtxFmtList.clear();
}

// Return mesh count.
IZ_UINT CColladaGeometry::GetMeshNum(DAE* pDAE)
{
	domGeometry* pGeometry = IZ_NULL;
	pDAE->getDatabase()->getElement(
		(daeElement**)&pGeometry,
		0,
		IZ_NULL,
		"geometry");

	// Get mesh.
	domMesh* pMesh = pGeometry->getMesh();

	// Add all primitves count.
	size_t ret = 0;
	ret += pMesh->getTriangles_array().getCount();
	ret += pMesh->getTristrips_array().getCount();
	ret += pMesh->getTrifans_array().getCount();
	ret += pMesh->getPolylist_array().getCount();
	ret += pMesh->getPolygons_array().getCount();

	return static_cast<IZ_UINT>(ret);
}

namespace {
	domSkinRef _GetSkin(DAE* pDAE)
	{
		// NOTE
		// 複数のモデルが混在するシーンは対応しない
		// DOMツリーをたどれなくはないが、相当面倒くさいので・・・

		domVisual_scene* pScene = IZ_NULL;
		pDAE->getDatabase()->getElement(
			(daeElement**)&pScene,
			0,
			IZ_NULL,
			"visual_scene");

		size_t nNodeCnt = pScene->getNode_array().getCount();

		// Search all nodes.
		for (size_t i = 0; i < nNodeCnt; i++) {
			domNodeRef pNode = pScene->getNode_array().get(i);

			// If a node has controller, it is skin node.
			if (pNode->getInstance_controller_array().getCount() > 0) {
				// I think that skin must have "a" controller.
				IZ_ASSERT(pNode->getInstance_controller_array().getCount() == 1);

				// Get controller.
				domInstance_controller* pInstanceController = pNode->getInstance_controller_array().get(0);
				xsAnyURI & urltype  = pInstanceController->getUrl();
				domElement* pElement = urltype.getElement();
				domController* pController = (domController*)pElement;

				// Get skin.
				domSkinRef pSkin = pController->getSkin();
				return pSkin;
			}
		}

		return IZ_NULL;
	}
}	// namespace

// 指定されているメッシュに関連するスキニング情報を取得.
// Get informaitons of skin.
void CColladaGeometry::GetSkinList(
	DAE* pDAE,
	std::vector<SSkin>& tvSkinList)
{
	// そもそもスキンニングがあるのか
	domSkinRef pSkin = _GetSkin(pDAE);
	if (pSkin == IZ_NULL) {
		// There is no skin, so nothing is done.
		return;
	}

	// Get weights element.
	domSkin::domVertex_weightsRef pWeightsElement = pSkin->getVertex_weights();
	domSource* pWeightsSrc = NULL;

	// Get source for weight.
	for (size_t i = 0; i < pWeightsElement->getInput_array().getCount(); i++) {
		xsNMTOKEN strSemantic = pWeightsElement->getInput_array().get(i)->getSemantic();
		if (izanagi::tool::CString::CmpStr(strSemantic, "WEIGHT")) {
			pWeightsSrc = (domSource*)((daeElement*)pWeightsElement->getInput_array()[i]->getSource().getElement());
			break;
		}
	}

	IZ_UINT pos = 0;
	domUint nWeightNum = pSkin->getVertex_weights()->getCount();

	tvSkinList.reserve((IZ_UINT)nWeightNum);

	for (domUint i = 0; i < nWeightNum; i++) {
		tvSkinList.push_back(SSkin());
		SSkin& skin = tvSkinList.back();

		// 影響を与える頂点数を取得
		// Get count of information that joint influences vertex.
		domUint nInfluenceNum = pSkin->getVertex_weights()->getVcount()->getValue()[(size_t)i];

		for (domUint nInflCnt = 0; nInflCnt < nInfluenceNum; nInflCnt++) {
			// 関節インデックス取得
			IZ_UINT nJointIdx = (IZ_UINT)pSkin->getVertex_weights()->getV()->getValue()[pos + 0];

			// ウエイト値を取得
			IZ_UINT nWeightIdx = (IZ_UINT)pSkin->getVertex_weights()->getV()->getValue()[pos + 1];
			IZ_FLOAT fWeight = (IZ_FLOAT)pWeightsSrc->getFloat_array()->getValue()[nWeightIdx];

			skin.Add(nJointIdx, fWeight);
			
			pos += 2;
		}

		// The total of weights is made to become to 1.0.
		skin.Normalize();
	}
}

// 指定されているメッシュに含まれる三角形を取得.
// Returns count of triangles in specified mesh.
IZ_UINT CColladaGeometry::GetTriangles(
	domMesh* pMesh,
	std::vector<STri>& tvTriList)
{
	m_TriList = &tvTriList;

	// 形状に応じて三角形を取得.

	switch (m_PrimType) {
	case E_PRIM_TYPE_TRIANGLES:
		{
			domTrianglesRef triangles = pMesh->getTriangles_array().get(m_nMeshSubsetPos);
			ReadVtx(triangles);
		}
		break;
	case E_PRIM_TYPE_POLYLIST:
		{
			domPolylistRef polylist = pMesh->getPolylist_array().get(m_nMeshSubsetPos);
			ReadVtx(polylist);
		}
		break;
	case E_PRIM_TYPE_POLYGONS:
		{
			domPolygonsRef polygon = pMesh->getPolygons_array().get(m_nMeshSubsetPos);
			// TODO
			IZ_ASSERT(IZ_FALSE);
		}
		break;
	case E_PRIM_TYPE_TRISTRIP:
		{
			domTristripsRef tristrip = pMesh->getTristrips_array().get(m_nMeshSubsetPos);
			// TODO
			IZ_ASSERT(IZ_FALSE);
		}
		break;
	case E_PRIM_TYPE_TRIFAN:
		{
			domTrifansRef trifan = pMesh->getTrifans_array().get(m_nMeshSubsetPos);
			// TODO
			IZ_ASSERT(IZ_FALSE);
		}
		break;
	default:
		IZ_ASSERT(IZ_FALSE);
		break;
	}	// end of switch

	IZ_UINT ret = (IZ_UINT)m_VtxList.size();
	return ret;
}

// Returns index of skin that influeces specified vertex.
IZ_UINT CColladaGeometry::GetSkinIdx(
	domMesh* pMesh,
	IZ_UINT nVtxIdx)
{
	IZ_UINT ret = 0;

	switch (m_PrimType) {
	case E_PRIM_TYPE_TRIANGLES:
		{
			domTrianglesRef triangles = pMesh->getTriangles_array().get(m_nMeshSubsetPos);
			ret = GetSkinIdx(triangles, nVtxIdx);
		}
		break;
	case E_PRIM_TYPE_POLYLIST:
		{
			domPolylistRef polylist = pMesh->getPolylist_array().get(m_nMeshSubsetPos);
			ret = GetSkinIdx(polylist, nVtxIdx);
		}
		break;
	case E_PRIM_TYPE_POLYGONS:
		{
			domPolygonsRef polygon = pMesh->getPolygons_array().get(m_nMeshSubsetPos);
			ret = GetSkinIdx(polygon, nVtxIdx);
		}
		break;
	case E_PRIM_TYPE_TRISTRIP:
		{
			domTristripsRef tristrip = pMesh->getTristrips_array().get(m_nMeshSubsetPos);
			ret = GetSkinIdx(tristrip, nVtxIdx);
		}
		break;
	case E_PRIM_TYPE_TRIFAN:
		{
			domTrifansRef trifan = pMesh->getTrifans_array().get(m_nMeshSubsetPos);
			ret = GetSkinIdx(trifan, nVtxIdx);
		}
		break;
	default:
		IZ_ASSERT(IZ_FALSE);
		break;
	}	// end of switch

	return ret;
}

// Returns size per vertex.
IZ_UINT CColladaGeometry::GetVtxSize()
{
	static IZ_UINT tblVtxSize[] = {
		izanagi::E_MSH_VTX_SIZE_POS,
		izanagi::E_MSH_VTX_SIZE_NORMAL,
		izanagi::E_MSH_VTX_SIZE_COLOR,
		izanagi::E_MSH_VTX_SIZE_UV,
		izanagi::E_MSH_VTX_SIZE_TANGENT,
		izanagi::E_MSH_VTX_SIZE_BLENDINDICES,
		izanagi::E_MSH_VTX_SIZE_BLENDWEIGHT,
	};
	IZ_C_ASSERT(COUNTOF(tblVtxSize) == izanagi::E_MSH_VTX_FMT_TYPE_NUM);

	IZ_UINT ret = 0;

	std::set<SVtxFmt>::const_iterator it = m_VtxFmtList.begin();
	for (; it != m_VtxFmtList.end(); it++) {
		const SVtxFmt& sVtxFmt = *it;
		ret += tblVtxSize[sVtxFmt.type];
	}

	return ret;
}

// Returns vertex format.
IZ_UINT CColladaGeometry::GetVtxFmt()
{
	IZ_ASSERT(!m_VtxFmtList.empty());

	IZ_UINT ret = 0;

	std::set<SVtxFmt>::const_iterator it = m_VtxFmtList.begin();
	for (; it != m_VtxFmtList.end(); it++) {
		const SVtxFmt& sVtxFmt = *it;
		ret |= (1 << sVtxFmt.type);
	}

	return ret;
}

namespace {
	// Returns paramter's source.
	inline domSourceRef _GetSource(
		domMeshRef pMesh,
		domString strSrcId)
	{
		size_t num = pMesh->getSource_array().getCount();

		for (size_t i = 0; i < num; i++) {
			domSourceRef pSrc = pMesh->getSource_array().get(i);

			if (izanagi::tool::CString::CmpStr(pSrc->getID(), strSrcId)) {
				return pSrc;
			}
		}

		IZ_ASSERT(IZ_FALSE);
		return IZ_NULL;
	}

	enum E_VTX_ACCESS_TYPE {
		E_VTX_ACCESS_TYPE_FLOAT = 0,

		E_VTX_ACCESS_TYPE_NUM,
	};

	template <typename _T>
	void _GetVtxData(
		_T pParam,
		IZ_UINT nStride,
		IZ_UINT nPos,
		izanagi::SVector& sVec)
	{
		switch (nStride) {
		case 4:
			sVec.w = static_cast<IZ_FLOAT>(pParam->getValue()[nPos + 3]);
		case 3:
			sVec.z = static_cast<IZ_FLOAT>(pParam->getValue()[nPos + 2]);
		case 2:
			sVec.y = static_cast<IZ_FLOAT>(pParam->getValue()[nPos + 1]);
		case 1:
			sVec.x = static_cast<IZ_FLOAT>(pParam->getValue()[nPos + 0]);
			break;
		default:
			IZ_ASSERT(IZ_FALSE);
			break;
		}
	}

	E_VTX_ACCESS_TYPE _GetVtxData(
		domSourceRef pSrc,
		IZ_UINT nIdx,
		izanagi::SVector& sVec)
	{
		static IZ_PCSTR TypeTbl[] = {
			"float",
		};
		IZ_C_ASSERT(COUNTOF(TypeTbl) == E_VTX_ACCESS_TYPE_NUM);

		// Get paramter's type (float etc...).
		E_VTX_ACCESS_TYPE ret = E_VTX_ACCESS_TYPE_NUM;
		{
			xsNMTOKEN type = pSrc->getTechnique_common()->getAccessor()->getParam_array().get(0)->getType();
			for (IZ_UINT i = 0; i < E_VTX_ACCESS_TYPE_NUM; i++) {
				if (izanagi::tool::CString::CmpStr(TypeTbl[i], type)) {
					ret = static_cast<E_VTX_ACCESS_TYPE>(i);
					break;
				}
			}

			IZ_ASSERT(ret < E_VTX_ACCESS_TYPE_NUM);
		}

		domUint nStride = pSrc->getTechnique_common()->getAccessor()->getStride();

		IZ_UINT nPos = (IZ_UINT)(nIdx * nStride);

		switch (ret) {
		case E_VTX_ACCESS_TYPE_FLOAT:
			{
				domFloat_arrayRef pFloatArray = pSrc->getFloat_array();
				_GetVtxData(pFloatArray, (IZ_UINT)nStride, nPos, sVec);
			}
			break;
		default:
			IZ_ASSERT(IZ_FALSE);
			break;
		}

		return ret;
	}
}	// namespace

// Get vertex paremter by specifed type.
//  ex) If specfied type is E_MSH_VTX_FMT_TYPE_NORMAL, so get vertex normal.
IZ_BOOL CColladaGeometry::GetVertex(
	domMesh* pMesh,
	IZ_UINT nIdx,
	izanagi::SVector& vec,
	izanagi::E_MSH_VTX_FMT_TYPE type)
{
#if 1
	std::set<SVtxFmt>::const_iterator itVtxFmt = m_VtxFmtList.begin();
	for (; itVtxFmt != m_VtxFmtList.end(); itVtxFmt++) {
		const SVtxFmt& sVtxFmt = *itVtxFmt;

		if (sVtxFmt.type == type) {
			break;
		}
	}
#else
	// Get type.
	std::set<SVtxFmt>::const_iterator itVtxFmt = std::find(
													m_VtxFmtList.begin(),
													m_VtxFmtList.end(),
													type);
#endif

	if (itVtxFmt == m_VtxFmtList.end()) {
		// There is no specfied type, so nothing is done.
		return IZ_FALSE;
	}

	const SVtxFmt& sVtxFmt = *itVtxFmt;

	const SVtx& sVtx = m_VtxList[nIdx];
	nIdx = sVtx.input[sVtxFmt.offset];

	// Get pareters's source.
	domSourceRef pSrc = _GetSource(pMesh, sVtxFmt.src_id);

	izanagi::SVector::SetDefaultZero(vec);

	// Get vertex paramter.
	E_VTX_ACCESS_TYPE nVtxType = _GetVtxData(
									pSrc,
									nIdx,
									vec);
	IZ_ASSERT(nVtxType == E_VTX_ACCESS_TYPE_FLOAT);

	return IZ_TRUE;
}

// メッシュにバインドされるマテリアル情報を取得
IZ_BOOL CColladaGeometry::GetMaterialForMesh(
	IZ_UINT nIdx,
	izanagi::S_MSH_MTRL& sMtrl)
{
	IZ_BOOL ret = IZ_FALSE;

	if (m_Mtrl.find(nIdx) != m_Mtrl.end()) {
		const std::string& mtrlName = m_Mtrl.find(nIdx)->second;
		sMtrl.name.SetString(mtrlName.c_str());
		sMtrl.nameKey = sMtrl.name.GetKeyValue();
	}

	return ret;
}

namespace {
	// プリミティブから三角形数を取得
	template <typename _T>
	inline IZ_UINT _GetTriNumFromPrim(
		_T pPrim,
		IZ_UINT nPos)
	{
		IZ_ASSERT(IZ_FALSE);
		return 0;
	}

	// プリミティブから三角形数を取得
	template <>
	inline IZ_UINT _GetTriNumFromPrim(
		domPolylistRef pPrim,
		size_t primCnt)
	{
		// 頂点数を取得
		domUint ret = pPrim->getVcount()->getValue().get(primCnt);

		// 頂点数から三角形数を計算
		ret -= 2;
		return static_cast<IZ_UINT>(ret);
	}

	template <typename _T>
	inline size_t _AdvanceOffset(
		_T pPrim,
		size_t nPos,
		size_t nInputNum,
		size_t nOffset)
	{
		domUint nVtxNum = pPrim->getVcount()->getValue().get(nPos);
		nOffset += (size_t)(nVtxNum * nInputNum);
		return nOffset;
	}

	template <>
	inline size_t _AdvanceOffset(
		domTrianglesRef pTriangles,
		size_t nPos,
		size_t nInputNum,
		size_t nOffset)
	{
		nOffset += 3 * nInputNum;
		return nOffset;
	}
}	// namespace

// 頂点データを読み込む
template <typename _T>
void CColladaGeometry::ReadVtx(_T pPrim)
{
	size_t nInputNum = pPrim->getInput_array().getCount();

	// プリミティブ数
	size_t nPrimNum = pPrim->getVcount()->getValue().getCount();

	size_t nOffset = 0;

	for (size_t nPrimCnt = 0; nPrimCnt < nPrimNum; nPrimCnt++) {
		// プリミティブから三角形数を取得
		IZ_UINT nTriNum = _GetTriNumFromPrim(pPrim, nPrimCnt);

		ReadVtx(
			nInputNum,
			nTriNum,
			nOffset,
			pPrim);

		nOffset = _AdvanceOffset(
					pPrim,
					nPrimCnt,
					nInputNum,
					nOffset);
	}
}

template <>
void CColladaGeometry::ReadVtx(domTrianglesRef pTriangles)
{
	size_t nInputNum = pTriangles->getInput_array().getCount();

	// プリミティブ数
	size_t nPrimNum = (size_t)pTriangles->getCount();

	size_t nOffset = 0;

	for (size_t nPrimCnt = 0; nPrimCnt < nPrimNum; nPrimCnt++) {
		ReadVtx(
			nInputNum,
			1,
			nOffset,
			pTriangles);

		nOffset = _AdvanceOffset(
					pTriangles,
					nPrimCnt,
					nInputNum,
					nOffset);
	}
}

// 頂点データを読み込む
template <typename _T>
void CColladaGeometry::ReadVtx(
	size_t nInputNum,
	IZ_UINT nTriNum,
	size_t nOffset,
	_T pPrim)
{
	size_t nIdx = nInputNum;

	for (IZ_UINT nTriCnt = 0; nTriCnt < nTriNum; nTriCnt++) {
		m_TriList->push_back(STri());
		STri& sTri = m_TriList->back();

		// 1st vertex.
		{
			SVtx sVtx;

			for (size_t i = 0; i < nInputNum; i++) {
				IZ_UINT idx = (IZ_UINT)pPrim->getP()->getValue().get(nOffset + i);
				sVtx.input.push_back(idx);
			}

			AddVtx(sTri, 0, sVtx);
		}

		// 2nd vertex.
		{
			SVtx sVtx;

			for (size_t i = 0; i < nInputNum; i++) {
				IZ_UINT idx = (IZ_UINT)pPrim->getP()->getValue().get(nOffset + nIdx + i);
				sVtx.input.push_back(idx);
			}

			AddVtx(sTri, 1, sVtx);
		}
		nIdx += nInputNum;

		// 3rd vertex.
		{
			SVtx sVtx;

			for (size_t i = 0; i < nInputNum; i++) {
				IZ_UINT idx = (IZ_UINT)pPrim->getP()->getValue().get(nOffset + nIdx + i);
				sVtx.input.push_back(idx);
			}

			AddVtx(sTri, 2, sVtx);
		}
	}
}

// 頂点を追加 & 三角形に頂点インデックスを設定
void CColladaGeometry::AddVtx(
	STri& sTri,
	IZ_UINT nPos,
	CColladaGeometry::SVtx& sVtx)
{
	// 指定された頂点を探す
	std::vector<CColladaGeometry::SVtx>::iterator it;
	it = std::find(
			m_VtxList.begin(), 
			m_VtxList.end(),
			sVtx);

	if (it == m_VtxList.end()) {
		// 見つからなかったので新規登録
		sTri.vtx[nPos] = (IZ_UINT)m_VtxList.size();
		m_VtxList.push_back(sVtx);
	}
	else {
		// 見つかったので、インデックスをセット
		sTri.vtx[nPos] = (IZ_UINT)std::distance(m_VtxList.begin(), it);
	}
}

// 頂点フォーマットを読み込む
template <typename _T>
void CColladaGeometry::ReadVtxFmt(
	domMesh* pMesh,
	_T pPrim)
{
	std::vector<SVtxFmt> tvVtxFmt;

	size_t nInputNum = pPrim->getInput_array().getCount();

	for (size_t i = 0; i < nInputNum; i++) {
		domInputLocalOffsetRef refInput = pPrim->getInput_array().get(i);

		daeString strSemantic = refInput->getSemantic();
		if (izanagi::tool::CString::CmpStr(strSemantic, "VERTEX")) {
			// <vertices> 経由
			domVerticesRef refVtx = pMesh->getVertices();
			size_t nVtxInputNum = refVtx->getInput_array().getCount();

			for (size_t n = 0; n < nVtxInputNum; n++) {
				domInputLocal* pVtxInput = refVtx->getInput_array().get(n);

				SVtxFmt sVtxFmt;
				{
					// NOTE
					// offsetは<input>から取ってくる
					sVtxFmt.offset = static_cast<size_t>(refInput->getOffset());
					sVtxFmt.semantic = pVtxInput->getSemantic();
					sVtxFmt.src_id = pVtxInput->getSource().getFragment();
				}
				tvVtxFmt.push_back(sVtxFmt);
			}
		}
		else {
			SVtxFmt sVtxFmt;
			{
				sVtxFmt.offset = static_cast<size_t>(refInput->getOffset());
				sVtxFmt.semantic = strSemantic;
				sVtxFmt.src_id = refInput->getSource().getFragment();
			}
			tvVtxFmt.push_back(sVtxFmt);
		}
	}

	static struct {
		IZ_PCSTR semantic;
		izanagi::E_MSH_VTX_FMT_TYPE type;
	} tblInput[] = {
		"POSITION",     izanagi::E_MSH_VTX_FMT_TYPE_POS,
		"NORMAL",       izanagi::E_MSH_VTX_FMT_TYPE_NORMAL,
		"COLOR",        izanagi::E_MSH_VTX_FMT_TYPE_COLOR,
		"TEXCOORD",     izanagi::E_MSH_VTX_FMT_TYPE_UV,
		"TANGENT",      izanagi::E_MSH_VTX_FMT_TYPE_TANGENT,
		"BLENDINDICES", izanagi::E_MSH_VTX_FMT_TYPE_BLENDINDICES,
		"BLENDWEIGHT",  izanagi::E_MSH_VTX_FMT_TYPE_BLENDWEIGHT,
	};
	IZ_C_ASSERT(COUNTOF(tblInput) == izanagi::E_MSH_VTX_FMT_TYPE_NUM);

	for (IZ_UINT i = 0; i < izanagi::E_MSH_VTX_FMT_TYPE_NUM; i++) {
		std::vector<SVtxFmt>::iterator it = std::find(
												tvVtxFmt.begin(),
												tvVtxFmt.end(),
												tblInput[i].semantic);

		if (it != tvVtxFmt.end()) {
			SVtxFmt& sVtxFmt = *it;
			sVtxFmt.type = tblInput[i].type;

			m_VtxFmtList.insert(sVtxFmt);
		}
	}
}

// Return index of skin that influeces specified vertex in specified primitive.
template <typename _T>
IZ_UINT CColladaGeometry::GetSkinIdx(
	_T pPrim,
	IZ_UINT nVtxIdx)
{
	size_t nInputNum = pPrim->getInput_array().getCount();

	IZ_UINT nVtxOffset = 0;

	// Find input element which is vertex.
	for (size_t i = 0; i < nInputNum; i++) {
		domInputLocalOffsetRef pInput = pPrim->getInput_array().get(i);

		daeString strSemantic = pInput->getSemantic();
		if (izanagi::tool::CString::CmpStr(strSemantic, "VERTEX")) {
			nVtxOffset = (IZ_UINT)pInput->getOffset();
			break;
		}
		else if (izanagi::tool::CString::CmpStr(strSemantic, "POSITION")) {
			nVtxOffset = (IZ_UINT)pInput->getOffset();
			break;
		}
	}

	SVtx& sVtx = m_VtxList[nVtxIdx];

	IZ_UINT ret = sVtx.input[nVtxOffset];

	return ret;
}

// メッシュのプリミティブタイプを取得
CColladaGeometry::E_PRIM_TYPE CColladaGeometry::GetPrimType(
	domMesh* pMesh,
	IZ_UINT nIdx,
	IZ_UINT* pMeshSubsetPos)
{
	static size_t tblMeshSubsetCnt[] = {
		pMesh->getTriangles_array().getCount(),
		pMesh->getTristrips_array().getCount(),
		pMesh->getTrifans_array().getCount(),
		pMesh->getPolylist_array().getCount(),
		pMesh->getPolygons_array().getCount(),
	};
	IZ_C_ASSERT(COUNTOF(tblMeshSubsetCnt) == CColladaGeometry::E_PRIM_TYPE_NUM);

	IZ_UINT nPos = nIdx;

	// NOTE
	// 力技だがどのプリミティブの範囲に入るかを探索して
	// ヒットしたプリミティブのタイプを返す

	for (IZ_UINT i = 0; i < E_PRIM_TYPE_NUM; i++) {
		if (nPos < tblMeshSubsetCnt[i]) {
			if (pMeshSubsetPos != IZ_NULL) {
				*pMeshSubsetPos = nPos;
			}

			return static_cast<E_PRIM_TYPE>(i);
		}

		nPos -= (IZ_UINT)tblMeshSubsetCnt[i];
	}

	IZ_ASSERT(IZ_FALSE);
	return E_PRIM_TYPE_TRIANGLES;
}

// メッシュにバインドされるマテリアルを読み込み
template <typename _T>
void CColladaGeometry::ReadMeshMtrl(
	DAE* pDAE,
	IZ_UINT nIdx,
	_T pPrim)
{
	domVisual_scene* pScene = IZ_NULL;
	pDAE->getDatabase()->getElement(
		(daeElement**)&pScene,
		0,
		IZ_NULL,
		"visual_scene");

	size_t nNodeCnt = pScene->getNode_array().getCount();

	std::map<std::string, std::string> mapBindMtrl;

	// Search all nodes.
	for (size_t i = 0; i < nNodeCnt; i++) {
		domNodeRef pNode = pScene->getNode_array().get(i);

		// If a node has controller, it is skin node.
		if (pNode->getInstance_controller_array().getCount() > 0) {
			// I think that skin must have "a" controller.
			IZ_ASSERT(pNode->getInstance_controller_array().getCount() == 1);

			// Get controller.
			domInstance_controller* pInstanceController = pNode->getInstance_controller_array().get(0);
			
			// Get <bind_material>
			domBind_materialRef bindMaterial = pInstanceController->getBind_material();
			if (bindMaterial) {
				// Get <technique_common>
				domBind_material::domTechnique_common *techCommon = bindMaterial->getTechnique_common();
				if (techCommon) {
					// Get <instance_material>
					domInstance_material_Array &instMtrlArray = techCommon->getInstance_material_array();
					for(size_t n = 0; n < instMtrlArray.getCount(); n++) {
						domInstance_materialRef mtrl = instMtrlArray[n];

						mapBindMtrl[mtrl->getSymbol()] = mtrl->getTarget().id();
					}
				}
			}
		}
	}

	const std::string& bindMtrlName = pPrim->getMaterial();
	
	if (mapBindMtrl.find(bindMtrlName) != mapBindMtrl.end()) {
		const std::string& mtrlName = mapBindMtrl.find(bindMtrlName)->second;
		m_Mtrl.insert(std::pair<IZ_UINT, std::string>(nIdx, mtrlName));
	}
}
