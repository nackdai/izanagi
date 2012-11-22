#if !defined(__MODEL_LIB_COLLADA_JOINT_H__)
#define __MODEL_LIB_COLLADA_JOINT_H__

#include <dom/domCOLLADA.h>
#include <dae.h>

#include <vector>

#include "izIo.h"
#include "izSceneGraph.h"

#include "GeometryCommon.h"

class CColladaJoint {
	static CColladaJoint* s_pInstance;

	enum E_JOINT_ACCESS_TYPE {
		E_JOINT_ACCESS_TYPE_NAME = 0,
		E_JOINT_ACCESS_TYPE_IDREF,

		E_JOINT_ACCESS_TYPE_NUM,
		E_JOINT_ACCESS_TYPE_FORCE_INT32 = 0x7fffffff,
	};

	struct SFuncIsSameNameNode;

public:
	static CColladaJoint& GetInstance();
	static void DeleteInstance();

private:
	CColladaJoint();
	~CColladaJoint() { Clear(); }

public:
	void Clear();

	IZ_BOOL ReadJoint(domVisual_scene* pScene);

	IZ_UINT GetJointNum() { return (IZ_UINT)m_JointList.size(); }

	IZ_PCSTR GetJointName(IZ_UINT nIdx);

	IZ_INT GetJointTree(
		const izanagi::S_SKL_JOINT& sJoint,
		const std::vector<izanagi::S_SKL_JOINT>& tvJoint);

	void GetJointInvMtx(
		IZ_UINT nIdx,
		izanagi::SMatrix& mtx);

#if 0
	IZ_UINT GetJointTransformNum(IZ_PCSTR pszNmae);
#endif

	void GetJointTransform(
		const izanagi::S_SKL_JOINT& sJoint,
		std::vector<SJointTransform>& tvTransform);

private:
	E_MDL_JOINT_TRANSFORM GetTransformType(IZ_PCSTR pszName);

private:
	std::vector<domNodeRef> m_JointList;

	E_JOINT_ACCESS_TYPE m_JointAccessType;

	domSource* m_pJointNameSrc;
	domSource* m_pInvBindMtxSrc;
};

#endif	// #if !defined(__MODEL_LIB_COLLADA_JOINT_H__)
