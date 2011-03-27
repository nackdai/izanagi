#include "scenegraph/mdl/MdlSkeleton.h"
#include "scenegraph/mdl/MdlJointTransform.h"
#include "scenegraph/mdl/MdlJoint.h"
#include "izIo.h"

#include "scenegraph/anm/izAnimation.h"
#include "scenegraph/anm/AnimationUtil.h"

using namespace izanagi;

CMdlSkeleton* CMdlSkeleton::CreateMdlSkeleton(
	IMemoryAllocator* pAllocator,
	CMdlJoint* pJoints)
{
	// Compute buffer size.
	IZ_UINT nSize = ComputeBufferSize(pJoints);

	// Allocate buffer.
	IZ_UINT8* pBuf = reinterpret_cast<IZ_UINT8*>(ALLOC_ZERO(pAllocator, nSize));
	VRETURN_NULL(pBuf != IZ_NULL);

	IZ_UINT8* pTmpBuf = pBuf;

	CMdlSkeleton* pInstance = new(pBuf) CMdlSkeleton;
	{
		pBuf += sizeof(CMdlSkeleton);

		pInstance->AddRef();
		pInstance->m_pAllocator = pAllocator;

		SAFE_REPLACE(pInstance->m_pJoints, pJoints);

		pInstance->m_nJointNodeNum = pJoints->GetHeader().numJoint;

		pBuf = pInstance->SetJointData(pBuf, pJoints);
	}

	IZ_ASSERT(CStdUtil::GetPtrDiff(pBuf, pTmpBuf) == nSize);

	return pInstance;
}

IZ_UINT CMdlSkeleton::ComputeBufferSize(const CMdlJoint* pJoints)
{
	IZ_UINT nSize = sizeof(CMdlSkeleton);
	{
		const S_MDL_JOINT_CHUNK_HEADER& sHeader = pJoints->GetHeader();

		nSize += sizeof(S_MDL_JOINT_POSE) * sHeader.numJoint;
		nSize += sizeof(SMatrix) * sHeader.numJoint;
	}

	return nSize; 
}

CMdlSkeleton::CMdlSkeleton()
{
	m_pAllocator = IZ_NULL;

	m_nJointNodeNum = 0;
	m_pJoints = IZ_NULL;

	m_pJointPose = IZ_NULL;
	m_pGlobalPose = IZ_NULL;
}

CMdlSkeleton::~CMdlSkeleton()
{
	SAFE_RELEASE(m_pJoints);
}

void CMdlSkeleton::BuildMatrix()
{
	for (IZ_UINT i = 0; i < m_nJointNodeNum; i++) {
		BuildLocalMatrix(i);
	}

	// Apply parent' matrix.
	for (IZ_UINT i = 0; i < m_nJointNodeNum; i++) {
		IZ_INT nParentIdx = m_pJoints->GetJoint(i)->parent;

		if (nParentIdx >= 0) {
			IZ_ASSERT((IZ_UINT)nParentIdx < i);
			ApplyParentMatrix(i, nParentIdx);
		}
	}

	for (IZ_UINT i = 0; i < m_nJointNodeNum; i++) {
		ApplyInvBindMatrix(i);
	}
}

const SMatrix* CMdlSkeleton::GetJointMtx(IZ_INT idx) const
{
	if (idx < 0) {
		return IZ_NULL;
	}
	IZ_ASSERT((IZ_UINT)idx < m_nJointNodeNum);
	return &m_pGlobalPose[idx];
}

IZ_UINT8* CMdlSkeleton::SetJointData(
	IZ_UINT8* pBuf,
	const CMdlJoint* pJoints)
{
	m_pJointPose = reinterpret_cast<S_MDL_JOINT_POSE*>(pBuf);
	pBuf += sizeof(S_MDL_JOINT_POSE) * m_nJointNodeNum;

	m_pGlobalPose = reinterpret_cast<SMatrix*>(pBuf);
	pBuf += sizeof(SMatrix) * m_nJointNodeNum;

	for (IZ_UINT i = 0; i < m_nJointNodeNum; i++) {
		const S_MDL_JOINT* pJoint = pJoints->GetJoint(i);

		memcpy(
			&m_pJointPose[i],
			&pJoint->pose,
			sizeof(S_MDL_JOINT_POSE));
	}

	return pBuf;
}

namespace {
#if 0
	void _RotateAxis(
		SMatrix& dst,
		const SMatrix& src,
		IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z, IZ_FLOAT angle)
	{
		SQuat quat;
		SetQuatFromRadAxis(quat, angle, x, y, z);

		SMatrix mtx;
		MatrixFromQuat(mtx, quat);

		MulMatrix(dst, src, mtx);
	}
#else
	void _RotateAxis(
		SMatrix& dst,
		const SMatrix& src,
		IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z, IZ_FLOAT w)
	{
		SQuat quat;
		SetQuat(quat, x, y, z, w);

		SMatrix mtx;
		MatrixFromQuat(mtx, quat);

		MulMatrix(dst, src, mtx);
	}
#endif
}	// namespace

void CMdlSkeleton::BuildLocalMatrix(IZ_UINT nIdx)
{
	IZ_ASSERT(m_pJoints != IZ_NULL);
	const S_MDL_JOINT* pJoint = m_pJoints->GetJoint(nIdx);

	// NOTE
	// izanagiは右掛け

	CBit32Flag flag(pJoint->validParam);

	const S_MDL_JOINT_POSE& pose = m_pJointPose[nIdx];
	SMatrix& mtxJoint = m_pGlobalPose[nIdx];

	SetUnitMatrix(mtxJoint);

	if (flag.IsOn(E_MDL_JOINT_PARAM_SCALE)) {
		ScaleMatrix(
			mtxJoint,
			mtxJoint, 
			pose.scale[0],
			pose.scale[1],
			pose.scale[2]);
	}

	if (flag.IsOn(E_MDL_JOINT_PARAM_QUARTANION)) {
		_RotateAxis(
			mtxJoint,
			mtxJoint,
			pose.quat.x,
			pose.quat.y,
			pose.quat.z,
			pose.quat.w);
	}

	if (flag.IsOn(E_MDL_JOINT_PARAM_TRANSLATE)) {
		TransMatrix(
			mtxJoint,
			mtxJoint, 
			pose.trans[0],
			pose.trans[1],
			pose.trans[2]);
	}
}

void CMdlSkeleton::ApplyInvBindMatrix(IZ_UINT nIdx)
{
	IZ_ASSERT(m_pJoints != IZ_NULL);
	const S_MDL_JOINT* pJoint = m_pJoints->GetJoint(nIdx);

	SMatrix& mtxJoint = m_pGlobalPose[nIdx];

	MulMatrix(
		mtxJoint,
		pJoint->mtxInvBind,
		mtxJoint);
}

void CMdlSkeleton::ApplyParentMatrix(
	IZ_UINT nIdx,
	IZ_UINT nParentIdx)
{
	SMatrix& mtxJoint = m_pGlobalPose[nIdx];
	const SMatrix& mtxParent = m_pGlobalPose[nParentIdx];

	MulMatrix(
		mtxJoint,
		mtxJoint,
		mtxParent);
}

void CMdlSkeleton::ApplyAnimation(
	IZ_FLOAT fTime,
	CAnimation* pAnm)
{
#if 1
	for (IZ_UINT i = 0; i < m_nJointNodeNum; i++) {
		ApplyAnimationByIdx(
			i,
			fTime,
			pAnm);
	}
#else
	ApplyAnimationByIdx(
		13,
		fTime,
		pAnm);
#endif
}

void CMdlSkeleton::ApplyAnimationByIdx(
	IZ_UINT nJointIdx,
	IZ_FLOAT fTime,
	CAnimation* pAnm)
{
	const S_ANM_NODE* pAnmNode = pAnm->GetAnmNodeByJointIdx(nJointIdx);
	if (pAnmNode != IZ_NULL) {
		ApplyAnimation(fTime, pAnmNode);
	}
}

void CMdlSkeleton::ApplyAnimationByName(
	IZ_PCSTR pszJointName,
	IZ_FLOAT fTime,
	CAnimation* pAnm)
{
	IZ_UINT nKey = CKey::GenerateValue(pszJointName);

	ApplyAnimationByKey(
		nKey,
		fTime,
		pAnm);
}

void CMdlSkeleton::ApplyAnimationByKey(
	IZ_UINT nJointKey,
	IZ_FLOAT fTime,
	CAnimation* pAnm)
{
	const S_ANM_NODE* pAnmNode = pAnm->GetAnmNodeByKey(nJointKey);
	if (pAnmNode != IZ_NULL) {
		ApplyAnimation(fTime, pAnmNode);
	}
}

void CMdlSkeleton::ApplyAnimation(
	IZ_FLOAT fTime,
	const S_ANM_NODE* pAnmNode)
{
	IZ_ASSERT(pAnmNode != IZ_NULL);

	const S_ANM_NODE& sAnmNode = *pAnmNode;
	const IZ_UINT nJointIdx = sAnmNode.targetIdx;

	IZ_ASSERT(nJointIdx < m_nJointNodeNum);

	const S_MDL_JOINT* pJoint = m_pJoints->GetJoint(nJointIdx);
	S_MDL_JOINT_POSE& sPose = m_pJointPose[nJointIdx];

	IZ_ASSERT(pJoint->nameKey == sAnmNode.targetKey);

	for (IZ_UINT nChannelIdx = 0; nChannelIdx < sAnmNode.numChannels; nChannelIdx++) {
		const S_ANM_CHANNEL& sChannel = sAnmNode.channels[nChannelIdx];

		IZ_UINT nParamType = (sChannel.type & E_ANM_TRANSFORM_TYPE_PARAM_MASK);
		IZ_UINT nTransformType = (sChannel.type & E_ANM_TRANSFORM_TYPE_TRANSFORM_MASK);

		IZ_FLOAT* param = IZ_NULL;
		switch (nTransformType) {
		case E_ANM_TRANSFORM_TYPE_TRANSLATE:
			param = sPose.trans;
			break;
		case E_ANM_TRANSFORM_TYPE_QUATERNION:
			// Nothing is done.
			break;
		case E_ANM_TRANSFORM_TYPE_SCALE:
			param = sPose.scale;
			break;
		default:
			IZ_ASSERT(IZ_FALSE);
			break;
		}

		const E_ANM_INTERP_TYPE nInterp = static_cast<E_ANM_INTERP_TYPE>(sChannel.interp);
		const IZ_UINT nKeyNum = sChannel.numKeys;
		S_ANM_KEY** const pKey = sChannel.keys;

		if (CAnimationUtil::IsScalarInterp(sChannel.interp)) {
			switch (nParamType) {
			case E_ANM_TRANSFORM_TYPE_X:
				param[0] = CAnimationUtil::ComputeInterp(nInterp, fTime, nKeyNum, 0, pKey);
				break;
			case E_ANM_TRANSFORM_TYPE_Y:
				param[1] = CAnimationUtil::ComputeInterp(nInterp, fTime, nKeyNum, 0, pKey);
				break;
			case E_ANM_TRANSFORM_TYPE_Z:
				param[2] = CAnimationUtil::ComputeInterp(nInterp, fTime, nKeyNum, 0, pKey);
				break;
			case E_ANM_TRANSFORM_TYPE_W:
				param[3] = CAnimationUtil::ComputeInterp(nInterp, fTime, nKeyNum, 0, pKey);
				break;
			case E_ANM_TRANSFORM_TYPE_XYZ:
				param[0] = CAnimationUtil::ComputeInterp(nInterp, fTime, nKeyNum, 0, pKey);
				param[1] = CAnimationUtil::ComputeInterp(nInterp, fTime, nKeyNum, 1, pKey);
				param[2] = CAnimationUtil::ComputeInterp(nInterp, fTime, nKeyNum, 2, pKey);
				break;
			case E_ANM_TRANSFORM_TYPE_XYZW:
				param[0] = CAnimationUtil::ComputeInterp(nInterp, fTime, nKeyNum, 0, pKey);
				param[1] = CAnimationUtil::ComputeInterp(nInterp, fTime, nKeyNum, 1, pKey);
				param[2] = CAnimationUtil::ComputeInterp(nInterp, fTime, nKeyNum, 2, pKey);
				param[3] = CAnimationUtil::ComputeInterp(nInterp, fTime, nKeyNum, 3, pKey);
				break;
			default:
				IZ_ASSERT(IZ_FALSE);
				break;
			}
		}
		else {
			// TODO
			IZ_ASSERT(nParamType == E_ANM_TRANSFORM_TYPE_XYZW);
			IZ_ASSERT(nTransformType == E_ANM_TRANSFORM_TYPE_QUATERNION);

			CAnimationUtil::ComputeInterp(
				sPose.quat,
				nInterp,
				fTime,
				nKeyNum,
				0,
				pKey);
		}
	}
}
