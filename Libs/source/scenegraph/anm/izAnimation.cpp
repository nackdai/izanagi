#include "scenegraph/anm/izAnimation.h"
#include "scenegraph/skl/SkeletonInstance.h"
#include "scenegraph/anm/AnimationUtil.h"
#include "izIo.h"

using namespace izanagi;

CAnimation* CAnimation::CreateAnimation(
	IMemoryAllocator* pAllocator,
	IInputStream* pIn)
{
	S_ANM_HEADER sHeader;
	IZ_INPUT_READ_ASSERT(pIn, &sHeader, 0, sizeof(sHeader));

	size_t nSize = sizeof(CAnimation);
	nSize += sizeof(SAnmNodeInstance) * sHeader.numNodes;
	nSize += sizeof(S_ANM_KEY*) * sHeader.numKeys;
	nSize += (sHeader.sizeFile - sHeader.sizeHeader);

	IZ_UINT8* pBuf = reinterpret_cast<IZ_UINT8*>(ALLOC_ZERO(pAllocator, nSize));
	VRETURN_NULL(pBuf != IZ_NULL);

	IZ_UINT8* pBufTop = pBuf;

	IZ_BOOL result = IZ_TRUE;

	CAnimation* pInstance = new(pBuf) CAnimation;
	{
		pBuf += sizeof(CAnimation);

		pInstance->AddRef();
		pInstance->m_pAllocator = pAllocator;

		memcpy(&pInstance->m_sHeader, &sHeader, sizeof(sHeader));

		pInstance->m_pNodes = reinterpret_cast<SAnmNodeInstance*>(pBuf);
		pBuf += sizeof(SAnmNodeInstance) * sHeader.numNodes;

		pBuf = pInstance->Init(pIn, pBuf);
		VGOTO(pBuf != IZ_NULL, __EXIT__);
	}

	IZ_ASSERT(CStdUtil::GetPtrAdvanced(pBufTop, pBuf) == nSize);

__EXIT__:
	if (!result) {
		SAFE_RELEASE(pInstance);
	}
	return pInstance;
}

CAnimation::CAnimation()
{
	m_pAllocator = IZ_NULL;
	m_pNodes = IZ_NULL;

	FILL_ZERO(&m_sHeader, sizeof(m_sHeader));
}

IZ_UINT8* CAnimation::Init(
	IInputStream* pIn,
	IZ_UINT8* pBuf)
{
	size_t nDataSize = m_sHeader.sizeFile - m_sHeader.sizeHeader;

	IZ_BOOL result = IZ_INPUT_READ(pIn, pBuf, 0, sizeof(S_ANM_NODE) * m_sHeader.numNodes); 
	VRETURN_NULL(result);

	nDataSize -= sizeof(S_ANM_NODE) * m_sHeader.numNodes;

	for (IZ_UINT i = 0; i < m_sHeader.numNodes; ++i) {
		m_pNodes[i].node = reinterpret_cast<S_ANM_NODE*>(pBuf);
		pBuf += sizeof(S_ANM_NODE);

		m_pNodes[i].InitHash();
		m_NodeHash.Add(m_pNodes[i].GetHashItem());
	}

	result = IZ_INPUT_READ(pIn, pBuf, 0, sizeof(S_ANM_CHANNEL) * m_sHeader.numChannels);
	VRETURN_NULL(result);

	nDataSize -= sizeof(S_ANM_CHANNEL) * m_sHeader.numChannels;

	for (IZ_UINT nKeyIdx = 0; nKeyIdx < m_sHeader.numNodes; ++nKeyIdx) {
		S_ANM_NODE& sNode = *m_pNodes[nKeyIdx].node;

		sNode.channels = reinterpret_cast<S_ANM_CHANNEL*>(pBuf);
		pBuf += sizeof(S_ANM_CHANNEL) * sNode.numChannels;
	}

	for (IZ_UINT nKeyIdx = 0; nKeyIdx < m_sHeader.numNodes; ++nKeyIdx) {
		S_ANM_NODE& sNode = *m_pNodes[nKeyIdx].node;

		for (IZ_UINT nChannelIdx = 0; nChannelIdx < sNode.numChannels; ++nChannelIdx) {
			sNode.channels[nChannelIdx].keys = reinterpret_cast<S_ANM_KEY**>(pBuf);
			pBuf += sizeof(S_ANM_KEY*) * sNode.channels[nChannelIdx].numKeys;
		}
	}

	result = IZ_INPUT_READ(pIn, pBuf, 0, nDataSize);
	VRETURN_NULL(result);

	for (IZ_UINT nKeyIdx = 0; nKeyIdx < m_sHeader.numNodes; ++nKeyIdx) {
		S_ANM_NODE& sNode = *m_pNodes[nKeyIdx].node;

		for (IZ_UINT nChannelIdx = 0; nChannelIdx < sNode.numChannels; ++nChannelIdx) {
			S_ANM_CHANNEL& sChannel = sNode.channels[nChannelIdx];

			for (IZ_UINT nKeyIdx = 0; nKeyIdx < sChannel.numKeys; ++nKeyIdx) {
				sChannel.keys[nKeyIdx] = reinterpret_cast<S_ANM_KEY*>(pBuf);
				pBuf += sizeof(S_ANM_KEY);
				pBuf += sizeof(IZ_FLOAT) * (sChannel.keys[nKeyIdx]->numParams - 1);
			}
		}
	}

	return pBuf;
}

void CAnimation::ApplyAnimation(
	CSkeletonInstance* skl,
	IZ_FLOAT time,
	const S_ANM_NODE* anmNode)
{
	const S_ANM_NODE& sAnmNode = *anmNode;
	const IZ_UINT nJointIdx = sAnmNode.targetIdx;

	SVector param;
	IZ_UINT updateFlag = 0;

	// 姿勢情報更新開始
	VRETURN_VAL(skl->BeginUpdatePose(nJointIdx),);

	for (IZ_UINT nChannelIdx = 0; nChannelIdx < sAnmNode.numChannels; ++nChannelIdx) {
		const S_ANM_CHANNEL& sChannel = sAnmNode.channels[nChannelIdx];

		IZ_UINT nParamType = (sChannel.type & E_ANM_TRANSFORM_TYPE_PARAM_MASK);
		IZ_UINT nTransformType = (sChannel.type & E_ANM_TRANSFORM_TYPE_TRANSFORM_MASK);

		switch (nTransformType) {
		case E_ANM_TRANSFORM_TYPE_TRANSLATE:
			updateFlag |= E_SKL_JOINT_PARAM_TRANSLATE;
			break;
		case E_ANM_TRANSFORM_TYPE_QUATERNION:
			updateFlag |= E_SKL_JOINT_PARAM_QUARTANION;
			break;
		case E_ANM_TRANSFORM_TYPE_SCALE:
			updateFlag |= E_SKL_JOINT_PARAM_SCALE;
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
			case E_ANM_TRANSFORM_TYPE_X:	// Xのみ
				param.v[0] = CAnimationUtil::ComputeInterp(nInterp, time, nKeyNum, 0, pKey);
				break;
			case E_ANM_TRANSFORM_TYPE_Y:	// Yのみ
				param.v[1] = CAnimationUtil::ComputeInterp(nInterp, time, nKeyNum, 0, pKey);
				break;
			case E_ANM_TRANSFORM_TYPE_Z:	// Zのみ
				param.v[2] = CAnimationUtil::ComputeInterp(nInterp, time, nKeyNum, 0, pKey);
				break;
			case E_ANM_TRANSFORM_TYPE_W:	// Wのみ
				param.v[3] = CAnimationUtil::ComputeInterp(nInterp, time, nKeyNum, 0, pKey);
				break;
			case E_ANM_TRANSFORM_TYPE_XYZ:	// XWZのみ
				param.v[0] = CAnimationUtil::ComputeInterp(nInterp, time, nKeyNum, 0, pKey);
				param.v[1] = CAnimationUtil::ComputeInterp(nInterp, time, nKeyNum, 1, pKey);
				param.v[2] = CAnimationUtil::ComputeInterp(nInterp, time, nKeyNum, 2, pKey);
				break;
			case E_ANM_TRANSFORM_TYPE_XYZW:	// XYZWすべて
				param.v[0] = CAnimationUtil::ComputeInterp(nInterp, time, nKeyNum, 0, pKey);
				param.v[1] = CAnimationUtil::ComputeInterp(nInterp, time, nKeyNum, 1, pKey);
				param.v[2] = CAnimationUtil::ComputeInterp(nInterp, time, nKeyNum, 2, pKey);
				param.v[3] = CAnimationUtil::ComputeInterp(nInterp, time, nKeyNum, 3, pKey);
				break;
			default:
				IZ_ASSERT(IZ_FALSE);
				break;
			}
		}
		else {
			// NOTE
			// 現状slerpを行う場合

			// TODO
			IZ_ASSERT(nParamType == E_ANM_TRANSFORM_TYPE_XYZW);
			IZ_ASSERT(nTransformType == E_ANM_TRANSFORM_TYPE_QUATERNION);

			CAnimationUtil::ComputeInterp(
				param,
				nInterp,
				time,
				nKeyNum,
				0,
				pKey);
		}

		// 姿勢情報更新
		skl->UpdatePose(
			nJointIdx,
			nTransformType,
			nParamType,
			param);
	}

	// 姿勢情報更新終了
	skl->EndUpdatePose(
		nJointIdx,
		updateFlag);
}

// アニメーション適用
void CAnimation::ApplyAnimation(
	CSkeletonInstance* skl,
	IZ_FLOAT time)
{
	IZ_ASSERT(skl != IZ_NULL);

	IZ_UINT jointNum = skl->GetJointNum();

	for (IZ_UINT i = 0; i < jointNum; ++i) {
		ApplyAnimationByIdx(skl, i, time);
	}
}

void CAnimation::ApplyAnimationByIdx(
	CSkeletonInstance* skl,
	IZ_UINT nJointIdx,
	IZ_FLOAT fTime)
{
	const S_ANM_NODE* pAnmNode = GetAnmNodeByJointIdx(nJointIdx);
	if (pAnmNode != IZ_NULL) {
		ApplyAnimation(skl, fTime, pAnmNode);
	}
}

void CAnimation::ApplyAnimationByName(
	CSkeletonInstance* skl,
	IZ_PCSTR pszJointName,
	IZ_FLOAT fTime)
{
	IZ_UINT nKey = CKey::GenerateValue(pszJointName);

	ApplyAnimationByKey(skl, nKey, fTime);
}

void CAnimation::ApplyAnimationByKey(
	CSkeletonInstance* skl,
	IZ_UINT nJointKey,
	IZ_FLOAT fTime)
{
	const S_ANM_NODE* pAnmNode = GetAnmNodeByKey(nJointKey);
	if (pAnmNode != IZ_NULL) {
		ApplyAnimation(skl, fTime, pAnmNode);
	}
}

const S_ANM_NODE* CAnimation::GetAnmNodeByIdx(IZ_UINT idx)
{
	IZ_ASSERT(idx < m_sHeader.numNodes);
	return m_pNodes[idx].node;
}

const S_ANM_NODE* CAnimation::GetAnmNodeByName(IZ_PCSTR name)
{
	IZ_UINT nKey = CKey::GenerateValue(name);
	return GetAnmNodeByKey(nKey);
}

const S_ANM_NODE* CAnimation::GetAnmNodeByKey(IZ_UINT key)
{
	CNodeHashItem* pItem = m_NodeHash.Find(key);

	if (pItem != IZ_NULL) {
		const SAnmNodeInstance* pNode = pItem->GetData();
		return pNode->node;
	}

	return IZ_NULL;
}

const S_ANM_NODE* CAnimation::GetAnmNodeByJointIdx(IZ_UINT nJointIdx)
{
	for (IZ_UINT i = 0; i < m_sHeader.numNodes; ++i) {
		const SAnmNodeInstance& sNode = m_pNodes[i];
		if (sNode.node->targetIdx == nJointIdx) {
			return sNode.node;
		}
	}

	return IZ_NULL;
}
