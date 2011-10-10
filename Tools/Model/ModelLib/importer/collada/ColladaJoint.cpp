#include "ColladaJoint.h"

#include "izMath.h"
#include "izToolKit.h"

////////////////////////////////////////////

struct CColladaJoint::SFuncIsSameNameNode {
	bool operator()(domNodeRef node)
	{
		domString strName = (type == E_JOINT_ACCESS_TYPE_NAME
								? node->getName()
								: node->getSid());

		IZ_BOOL ret = izanagi::izanagi_tk::CString::CmpStr(name, strName);

		// Avoid bool cast warning.
		return (ret == IZ_TRUE);
	}

	SFuncIsSameNameNode(
		IZ_PCSTR pszName,
		E_JOINT_ACCESS_TYPE nType)
	{
		name = pszName;
		type = nType;
	}

	IZ_PCSTR name;
	E_JOINT_ACCESS_TYPE type;
};

////////////////////////////////////////////

CColladaJoint* CColladaJoint::s_pInstance = IZ_NULL;

CColladaJoint& CColladaJoint::GetInstance()
{
	if (s_pInstance == IZ_NULL) {
		s_pInstance = new CColladaJoint();
	}
	return *s_pInstance;
}

void CColladaJoint::DeleteInstance()
{
	SAFE_DELETE(s_pInstance);
}

CColladaJoint::CColladaJoint()
{
	m_JointAccessType = E_JOINT_ACCESS_TYPE_NAME;

	m_pJointNameSrc = IZ_NULL;
	m_pInvBindMtxSrc = IZ_NULL;
}

void CColladaJoint::Clear()
{
	m_JointList.clear();

	m_JointAccessType = E_JOINT_ACCESS_TYPE_NAME;

	m_pJointNameSrc = IZ_NULL;
	m_pInvBindMtxSrc = IZ_NULL;
}

namespace {
	void _AddJoint(
		domNodeRef pNode,
		std::vector<domNodeRef>& tvJointList)
	{
		domNodeType type = pNode->getType();
		if (type == NODETYPE_JOINT) {
			tvJointList.push_back(pNode);
		}

		size_t nChildNodeCnt = pNode->getNode_array().getCount();

		for (size_t n = 0; n < nChildNodeCnt; n++) {
			domNodeRef node = pNode->getNode_array().get(n);
			_AddJoint(node, tvJointList);
		}
	}
}	// namespace

IZ_BOOL CColladaJoint::ReadJoint(domVisual_scene* pScene)
{
	IZ_ASSERT(pScene != IZ_NULL);

	size_t nNodeCnt = pScene->getNode_array().getCount();

	for (size_t i = 0; i < nNodeCnt; i++) {
		domNodeRef pNode = pScene->getNode_array().get(i);

		_AddJoint(pNode, m_JointList);
	}

	if (m_JointList.empty()) {
		return IZ_TRUE;
	}

	for (size_t i = 0; i < nNodeCnt; i++) {
		domNodeRef pNode = pScene->getNode_array().get(i);

		if (pNode->getInstance_controller_array().getCount() > 0) {
			IZ_ASSERT(pNode->getInstance_controller_array().getCount() == 1);

			domInstance_controller* pInstanceController = pNode->getInstance_controller_array().get(0);
			xsAnyURI & urltype  = pInstanceController->getUrl();
			domElement* pElement = urltype.getElement();
			domController* pController = (domController*)pElement;

			domSkinRef pSkin = pController->getSkin();

			if (pSkin != IZ_NULL) {
				// TODO
				// Get bind shape matrix.
				izanagi::SMatrix mtxBindShape;
				if (pSkin->getBind_shape_matrix() != IZ_NULL) {
					domSkin::domBind_shape_matrixRef pMtx = pSkin->getBind_shape_matrix();

					for (IZ_UINT m = 0; m < 16; m++) {
						mtxBindShape.a[m] = (IZ_FLOAT)pMtx->getValue()[m];
					}
				}

				IZ_ASSERT(izanagi::SMatrix::IsUnit(mtxBindShape));

				// Find the <joints> and <vertex_weights> elements inside <skin>
				domSkin::domJoints* pJointsElement = pSkin->getJoints();

				// Scan the <joint> looking for specific <input> semantics and remember where they are
				size_t nInputNum = pJointsElement->getInput_array().getCount();

				for (size_t nInputPos = 0; nInputPos < nInputNum; nInputPos++) {
					xsNMTOKEN strSemantic = pJointsElement->getInput_array().get(nInputPos)->getSemantic();

					if (izanagi::izanagi_tk::CString::CmpStr(strSemantic, "JOINT")) {
						// Found the JOINT input semantic
						m_pJointNameSrc = (domSource*)((daeElement*)pJointsElement->getInput_array()[nInputPos]->getSource().getElement());
					}
					else if (izanagi::izanagi_tk::CString::CmpStr(strSemantic, "INV_BIND_MATRIX")) {
						// Found the INV_BIND_MATRIX semantic
						m_pInvBindMtxSrc = (domSource*)((daeElement*)pJointsElement->getInput_array()[nInputPos]->getSource().getElement());
					}
				}
			}
		}
	}

	if (m_pJointNameSrc->getName_array() != IZ_NULL) {
		m_JointAccessType = E_JOINT_ACCESS_TYPE_NAME;
	}
	else if (m_pJointNameSrc->getIDREF_array() != IZ_NULL) {
		m_JointAccessType = E_JOINT_ACCESS_TYPE_IDREF;
	}
	else {
		VRETURN(IZ_FALSE);
	}

	return IZ_TRUE;
}

namespace {
	struct SFuncGetJointNameValue {
		domString operator()(
			IZ_UINT idx,
			domSource* pJointNameSrc)
		{
			return pJointNameSrc->getName_array()->getValue()[idx];
		}

		IZ_UINT operator()(domSource* pJointNameSrc)
		{
			return (IZ_UINT)pJointNameSrc->getName_array()->getCount();
		}
	};

	struct SFuncGetJointIDREFValue {
		domString operator()(
			IZ_UINT idx,
			domSource* pJointNameSrc)
		{
			return pJointNameSrc->getIDREF_array()->getValue()[idx].getID();
		}

		IZ_UINT operator()(domSource* pJointNameSrc)
		{
			return (IZ_UINT)pJointNameSrc->getIDREF_array()->getCount();
		}
	};

	template <typename _Func>
	domString _GetJointName(
		IZ_UINT nIdx,
		domSource* pJointNameSrc,
		_Func& func)
	{
		domString strName = func(nIdx, pJointNameSrc);
		IZ_ASSERT(strlen(strName) <= izanagi::SKL_NAME_LEN);

		return strName;
	}

}	// namespace

IZ_PCSTR CColladaJoint::GetJointName(IZ_UINT nIdx)
{
	if (m_JointList.empty()) {
		return IZ_NULL;
	}

	IZ_ASSERT(m_pJointNameSrc != IZ_NULL);

	if (m_pJointNameSrc->getName_array() != IZ_NULL) {
		domString strName = _GetJointName(
								nIdx,
								m_pJointNameSrc,
								SFuncGetJointNameValue());
		return strName;
	}
	else if (m_pJointNameSrc->getIDREF_array() != IZ_NULL) {
		domString strName = _GetJointName(
								nIdx,
								m_pJointNameSrc,
								SFuncGetJointIDREFValue());
		return strName;
	}

	IZ_ASSERT(IZ_FALSE);
	return IZ_NULL;
}

namespace {
	struct SFuncIsSameNameJoint {
		bool operator()(const izanagi::S_SKL_JOINT& sJoint)
		{
			IZ_BOOL ret = izanagi::izanagi_tk::CString::CmpStr(
							sJoint.name.GetString(),
							name);

			// Avoid bool cast warning.
			return (ret == IZ_TRUE);
		}

		SFuncIsSameNameJoint(IZ_PCSTR pszName) { name = pszName; }

		IZ_PCSTR name;
	};
}	// namespace


IZ_INT CColladaJoint::GetJointTree(
	const izanagi::S_SKL_JOINT& sJoint,
	const std::vector<izanagi::S_SKL_JOINT>& tvJoint)
{
	if (m_JointList.empty()) {
		return -1;
	}

	IZ_INT ret = -1;

	std::vector<domNodeRef>::iterator itNode;
	itNode = std::find_if(
				m_JointList.begin(),
				m_JointList.end(),
				SFuncIsSameNameNode(
					sJoint.name.GetString(),
					m_JointAccessType));
	IZ_ASSERT(itNode != m_JointList.end());

	domNodeRef pNode = *itNode;
	domNode* pParent = (domNode*)pNode->getParent();

	if (pParent != IZ_NULL) {
		// 親がJointかどうか
		itNode = std::find(
					m_JointList.begin(),
					m_JointList.end(),
					pParent);

		IZ_BOOL bIsParentJoint = (itNode != m_JointList.end());

		if (bIsParentJoint) {
			domString strParentName = (m_JointAccessType == E_JOINT_ACCESS_TYPE_NAME
										? pParent->getName()
										: pParent->getSid());

			std::vector<izanagi::S_SKL_JOINT>::const_iterator itJoint;
			itJoint = std::find_if(
						tvJoint.begin(),
						tvJoint.end(),
						SFuncIsSameNameJoint(strParentName));
			IZ_ASSERT(itJoint != tvJoint.end());

			size_t nParentIdx = std::distance(
									tvJoint.begin(),
									itJoint);

			ret = (IZ_INT)nParentIdx;
		}
	}

	return ret;
}

void CColladaJoint::GetJointInvMtx(
	IZ_UINT nIdx,
	izanagi::SMatrix& mtx)
{
	if (m_JointList.empty()) {
		return;
	}

	IZ_ASSERT(m_pInvBindMtxSrc != IZ_NULL);

	IZ_UINT nStride = (IZ_UINT)m_pInvBindMtxSrc->getTechnique_common()->getAccessor()->getStride();
	IZ_ASSERT(nStride == 16);

	IZ_UINT nOffset = nIdx * nStride;

	for (IZ_UINT m = 0; m < nStride; m++) {
		mtx.a[m] = (IZ_FLOAT)m_pInvBindMtxSrc->getFloat_array()->getValue()[nOffset + m];
	}

	// TODO
	// COLLADA-viewer のソースを見ていると必要らしいが・・・
	izanagi::SMatrix::Transpose(mtx, mtx);
}

#if 0
IZ_UINT CColladaJoint::GetJointTransformNum(IZ_PCSTR pszName)
{
	if (m_JointList.empty()) {
		return 0;
	}

	std::vector<domNodeRef>::iterator itNode;
	itNode = std::find_if(
				m_JointList.begin(),
				m_JointList.end(),
				SFuncIsSameNameNode(
					pszName,
					m_JointAccessType));
	IZ_ASSERT(itNode != m_JointList.end());

	domNodeRef pNode = *itNode;

	IZ_UINT ret = 0;

	// Read node transformations.
	for (size_t n = 0; n < pNode->getContents().getCount(); n++) {
		domElement* pContent = pNode->getContents().get(n);

		E_MDL_JOINT_TRANSFORM type = GetTransformType(pContent->getTypeName());

		switch (type) {
		case E_MDL_JOINT_TRANSFORM_TRANSLATE:
		case E_MDL_JOINT_TRANSFORM_QUATERNION:
		case E_MDL_JOINT_TRANSFORM_SCALE:
			ret++;
			break;
		default:
			IZ_ASSERT(IZ_FALSE);
			break;
		}
	}

	return ret;
}
#endif

E_MDL_JOINT_TRANSFORM CColladaJoint::GetTransformType(IZ_PCSTR pszName)
{
	static struct {
		IZ_PCSTR str;
		E_MDL_JOINT_TRANSFORM type;
	} tblType[] = {
		{"translate",	E_MDL_JOINT_TRANSFORM_TRANSLATE},
		{"quaternion",	E_MDL_JOINT_TRANSFORM_QUATERNION},	// dummy
		{"scale",		E_MDL_JOINT_TRANSFORM_SCALE},
		{"rotate",		E_MDL_JOINT_TRANSFORM_AXIS_ROT},
		{"rotate",		E_MDL_JOINT_TRANSFORM_ROTATE},		// dummy
		{"skew",		E_MDL_JOINT_TRANSFORM_SKEW},
		//{"matrix",    E_MDL_JOINT_TRANSFORM_MATRIX},
	};
	IZ_C_ASSERT(COUNTOF(tblType) == E_MDL_JOINT_TRANSFORM_NUM);

	for (IZ_UINT i = 0; i < E_MDL_JOINT_TRANSFORM_NUM; i++) {
		if (izanagi::izanagi_tk::CString::CmpStr(tblType[i].str, pszName)) {
			return tblType[i].type;
		}
	}

	return E_MDL_JOINT_TRANSFORM_NUM;
}

namespace {
	template <typename _T, IZ_UINT _NUM>
	_T* _GetTransformName(
		domElement* pContent,
		SJointTransform& sTransform)
	{
		_T* pTransform = reinterpret_cast<_T*>(pContent);
		IZ_ASSERT(pTransform->getValue().getCount() == _NUM);

		sTransform.name = pTransform->getSid();

		return pTransform;
	}

	// Export transformation data.
	template <typename _T, IZ_UINT _NUM>
	void _GetTransform(
		domElement* pContent,
		SJointTransform& sTransform)
	{
		IZ_C_ASSERT((0 < _NUM) && (_NUM <= 4));

		_T* pTransform = _GetTransformName<_T, _NUM>(
							pContent,
							sTransform);

		for (IZ_UINT i = 0; i < _NUM; i++) {
			sTransform.param.push_back((IZ_FLOAT)pTransform->getValue().get(i));
		}
	}

	// Export rotate transformation data...
	template <>
	void _GetTransform<domRotate, 4>(
		domElement* pContent,
		SJointTransform& sTransform)
	{
		domRotate* pTransform = _GetTransformName<domRotate, 4>(
									pContent,
									sTransform);

		izanagi::SVector vec;
		for (IZ_UINT i = 0; i < 4; i++) {
			vec.v[i] = (IZ_FLOAT)pTransform->getValue().get(i);
		}

		vec.v[3] = IZ_DEG2RAD(vec.v[3]);

		sTransform.param.push_back(vec.v[0]);
		sTransform.param.push_back(vec.v[1]);
		sTransform.param.push_back(vec.v[2]);
		sTransform.param.push_back(vec.v[3]);
	}

	// Get matrix transformation data...
	template <>
	void _GetTransform<domMatrix, 16>(
		domElement* pContent,
		SJointTransform& sTransform)
	{
		domMatrix* pMtx = _GetTransformName<domMatrix, 16>(
							pContent,
							sTransform);

		izanagi::SMatrix mtx;
		for (IZ_UINT i = 0; i < 16; i++) {
			mtx.a[i] = (IZ_FLOAT)pMtx->getValue().get(i);
		}

		// TODO
		// COLLADA-viewer のソースを見ていると必要らしいが・・・
		izanagi::SMatrix::Transpose(mtx, mtx);

		for (IZ_UINT i = 0; i < 16; i++) {
			sTransform.param.push_back(mtx.a[i]);
		}
	}
}	// namespace

void CColladaJoint::GetJointTransform(
	const izanagi::S_SKL_JOINT& sJoint,
	std::vector<SJointTransform>& tvTransform)
{
	if (m_JointList.empty()) {
		return;
	}

	typedef void (*FuncGetTransform)(domElement*, SJointTransform&);
	static FuncGetTransform tblGetFunc[] = {
		_GetTransform<domTranslate, 3>,	// E_MDL_JOINT_TRANSFORM_TRANSLATE
		IZ_NULL,						// E_MDL_JOINT_TRANSFORM_QUATERNION
		_GetTransform<domScale, 3>,		// E_MDL_JOINT_TRANSFORM_SCALE
		_GetTransform<domRotate, 4>,	// E_MDL_JOINT_TRANSFORM_AXIS_ROT
		IZ_NULL,						// E_MDL_JOINT_TRANSFORM_ROTATE (dummy...)
		IZ_NULL,						// E_MDL_JOINT_TRANSFORM_SKEW (skew is unsupported...)
		//_GetTransform<domMatrix, 16>,	// E_MDL_JOINT_TRANSFORM_MATRIX
	};
	IZ_C_ASSERT(COUNTOF(tblGetFunc) == E_MDL_JOINT_TRANSFORM_NUM);

	std::vector<domNodeRef>::iterator itNode;
	itNode = std::find_if(
				m_JointList.begin(),
				m_JointList.end(),
				SFuncIsSameNameNode(
					sJoint.name.GetString(),
					m_JointAccessType));
	IZ_ASSERT(itNode != m_JointList.end());

	domNodeRef pNode = *itNode;

	std::vector<domElement*> tvColladaTransform;

	// Read node transformations.
	for (size_t n = 0; n < pNode->getContents().getCount(); n++) {
		domElement* pContent = pNode->getContents().get(n);

		daeString strType = pContent->getTypeName();
		if (izanagi::izanagi_tk::CString::CmpStr(strType, "node")) {
			// terminate...
			continue;
		}

		tvColladaTransform.push_back(pContent);
	}

	// NOTE
	// Collada は行列演算は左掛け前提
	// ということは、Collada は上にある要素から順番に左掛けで計算すればよい
	//   ex) S * T * R * p
	// しかし、izanagiは右掛け前提なので、順番を逆にする必要がある
	//   ex) p * R * T * S

	// 逆順に回す
	std::vector<domElement*>::reverse_iterator it = tvColladaTransform.rbegin();

	tvTransform.resize(tvColladaTransform.size());

	for (IZ_UINT n = 0; it != tvColladaTransform.rend(); it++, n++) {
		domElement* pContent = *it;
		SJointTransform& sTransform = tvTransform[n];

		daeString strType = pContent->getTypeName();

		sTransform.type = GetTransformType(strType);

		FuncGetTransform func = tblGetFunc[sTransform.type];
		IZ_ASSERT(func != IZ_NULL);

		(*func)(pContent, sTransform);
	}
}
