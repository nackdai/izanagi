#include "scenegraph/skl/SkeletonInstance.h"
#include "scenegraph/skl/izSkeleton.h"
#include "izIo.h"

#include "scenegraph/anm/izAnimation.h"
#include "scenegraph/anm/AnimationUtil.h"

using namespace izanagi;

CSkeletonInstance* CSkeletonInstance::CreateSkeletonInstance(
	IMemoryAllocator* pAllocator,
	CSkeleton* pSkl)
{
	// Compute buffer size.
	IZ_UINT nSize = ComputeBufferSize(pSkl);

	// Allocate buffer.
	IZ_UINT8* pBuf = reinterpret_cast<IZ_UINT8*>(ALLOC_ZERO(pAllocator, nSize));
	VRETURN_NULL(pBuf != IZ_NULL);

	IZ_UINT8* pTmpBuf = pBuf;

	CSkeletonInstance* pInstance = new(pBuf) CSkeletonInstance;
	{
		pBuf += sizeof(CSkeletonInstance);

		pInstance->AddRef();
		pInstance->m_pAllocator = pAllocator;

		SAFE_REPLACE(pInstance->m_pBody, pSkl);

		pInstance->m_nJointNum = pSkl->GetJointNum();

		pBuf = pInstance->SetJointData(pBuf);
	}

	IZ_ASSERT(CStdUtil::GetPtrDistance(pBuf, pTmpBuf) == nSize);

	return pInstance;
}

IZ_UINT CSkeletonInstance::ComputeBufferSize(const CSkeleton* pSkl)
{
	IZ_UINT nSize = sizeof(CSkeletonInstance);
	{
		IZ_UINT nJointNum = pSkl->GetJointNum();

		nSize += sizeof(S_SKL_JOINT_POSE) * nJointNum;
		nSize += sizeof(SMatrix) * nJointNum;
	}

	return nSize; 
}

CSkeletonInstance::CSkeletonInstance()
{
	m_pAllocator = IZ_NULL;

	m_pBody = IZ_NULL;

	m_nJointNum = 0;

	m_pJointPose = IZ_NULL;
	m_pGlobalPose = IZ_NULL;
}

CSkeletonInstance::~CSkeletonInstance()
{
	SAFE_RELEASE(m_pBody);
}

void CSkeletonInstance::BuildMatrix()
{
	for (IZ_UINT i = 0; i < m_nJointNum; ++i) {
		BuildLocalMatrix(i);
	}

	// Apply parent' matrix.
	for (IZ_UINT i = 0; i < m_nJointNum; ++i) {
		IZ_INT nParentIdx = m_pBody->GetJoint(i)->parent;

		if (nParentIdx >= 0) {
			IZ_ASSERT((IZ_UINT)nParentIdx < i);
			ApplyParentMatrix(i, nParentIdx);
		}
	}

	for (IZ_UINT i = 0; i < m_nJointNum; ++i) {
		ApplyInvBindMatrix(i);
	}
}

const SMatrix* CSkeletonInstance::GetJointMtx(IZ_INT idx) const
{
	if (idx < 0) {
		return IZ_NULL;
	}
	IZ_ASSERT((IZ_UINT)idx < m_nJointNum);
	return &m_pGlobalPose[idx];
}

IZ_UINT8* CSkeletonInstance::SetJointData(IZ_UINT8* pBuf)
{
	m_pJointPose = reinterpret_cast<S_SKL_JOINT_POSE*>(pBuf);
	pBuf += sizeof(S_SKL_JOINT_POSE) * m_nJointNum;

	m_pGlobalPose = reinterpret_cast<SMatrix*>(pBuf);
	pBuf += sizeof(SMatrix) * m_nJointNum;

	for (IZ_UINT i = 0; i < m_nJointNum; ++i) {
		const S_SKL_JOINT* pJoint = m_pBody->GetJoint(i);

		memcpy(
			&m_pJointPose[i],
			&pJoint->pose,
			sizeof(S_SKL_JOINT_POSE));
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

void CSkeletonInstance::BuildLocalMatrix(IZ_UINT nIdx)
{
	IZ_ASSERT(m_pBody != IZ_NULL);
	const S_SKL_JOINT* pJoint = m_pBody->GetJoint(nIdx);

	// NOTE
	// izanagiは右掛け

	CBit32Flag flag(pJoint->validParam);

	const S_SKL_JOINT_POSE& pose = m_pJointPose[nIdx];
	SMatrix& mtxJoint = m_pGlobalPose[nIdx];

	SetUnitMatrix(mtxJoint);

	if (flag.IsOn(E_SKL_JOINT_PARAM_SCALE)) {
		ScaleMatrix(
			mtxJoint,
			mtxJoint, 
			pose.scale[0],
			pose.scale[1],
			pose.scale[2]);
	}

	if (flag.IsOn(E_SKL_JOINT_PARAM_QUARTANION)) {
		_RotateAxis(
			mtxJoint,
			mtxJoint,
			pose.quat.x,
			pose.quat.y,
			pose.quat.z,
			pose.quat.w);
	}

	if (flag.IsOn(E_SKL_JOINT_PARAM_TRANSLATE)) {
		TransMatrix(
			mtxJoint,
			mtxJoint, 
			pose.trans[0],
			pose.trans[1],
			pose.trans[2]);
	}
}

void CSkeletonInstance::ApplyInvBindMatrix(IZ_UINT nIdx)
{
	IZ_ASSERT(m_pBody != IZ_NULL);
	const S_SKL_JOINT* pJoint = m_pBody->GetJoint(nIdx);

	SMatrix& mtxJoint = m_pGlobalPose[nIdx];

	MulMatrix(
		mtxJoint,
		pJoint->mtxInvBind,
		mtxJoint);
}

void CSkeletonInstance::ApplyParentMatrix(
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

void CSkeletonInstance::ApplyAnimation(
	IZ_FLOAT fTime,
	CAnimation* pAnm)
{
#if 1
	for (IZ_UINT i = 0; i < m_nJointNum; ++i) {
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

void CSkeletonInstance::ApplyAnimationByIdx(
	IZ_UINT nJointIdx,
	IZ_FLOAT fTime,
	CAnimation* pAnm)
{
	const S_ANM_NODE* pAnmNode = pAnm->GetAnmNodeByJointIdx(nJointIdx);
	if (pAnmNode != IZ_NULL) {
		ApplyAnimation(fTime, pAnmNode);
	}
}

void CSkeletonInstance::ApplyAnimationByName(
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

void CSkeletonInstance::ApplyAnimationByKey(
	IZ_UINT nJointKey,
	IZ_FLOAT fTime,
	CAnimation* pAnm)
{
	const S_ANM_NODE* pAnmNode = pAnm->GetAnmNodeByKey(nJointKey);
	if (pAnmNode != IZ_NULL) {
		ApplyAnimation(fTime, pAnmNode);
	}
}

void CSkeletonInstance::ApplyAnimation(
	IZ_FLOAT fTime,
	const S_ANM_NODE* pAnmNode)
{
	IZ_ASSERT(pAnmNode != IZ_NULL);

	const S_ANM_NODE& sAnmNode = *pAnmNode;
	const IZ_UINT nJointIdx = sAnmNode.targetIdx;

	IZ_ASSERT(nJointIdx < m_nJointNum);

	const S_SKL_JOINT* pJoint = m_pBody->GetJoint(nJointIdx);
	S_SKL_JOINT_POSE& sPose = m_pJointPose[nJointIdx];

	IZ_ASSERT(pJoint->nameKey == sAnmNode.targetKey);

	for (IZ_UINT nChannelIdx = 0; nChannelIdx < sAnmNode.numChannels; ++nChannelIdx) {
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
