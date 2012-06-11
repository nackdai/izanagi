#include "scenegraph/anm/izAnimation.h"
#include "scenegraph/skl/SkeletonInstance.h"
#include "scenegraph/anm/AnimationUtil.h"
#include "scenegraph/skl/SkeletonUtil.h"
#include "scenegraph/anm/PoseUpdater.h"
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
		pInstance->m_Allocator = pAllocator;

		memcpy(&pInstance->m_Header, &sHeader, sizeof(sHeader));

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
	m_Allocator = IZ_NULL;
	m_pNodes = IZ_NULL;

	FILL_ZERO(&m_Header, sizeof(m_Header));
}

IZ_UINT8* CAnimation::Init(
	IInputStream* pIn,
	IZ_UINT8* pBuf)
{
	// データサイズ
	size_t nDataSize = m_Header.sizeFile - m_Header.sizeHeader;

	// ノード情報読み込み
	IZ_BOOL result = IZ_INPUT_READ(pIn, pBuf, 0, sizeof(S_ANM_NODE) * m_Header.numNodes); 
	VRETURN_NULL(result);

	// 読み込んだノード情報分減らす
	nDataSize -= sizeof(S_ANM_NODE) * m_Header.numNodes;

	// ノードを探索しやすくするためにハッシュに登録
	for (IZ_UINT i = 0; i < m_Header.numNodes; ++i) {
		m_pNodes[i].node = reinterpret_cast<S_ANM_NODE*>(pBuf);
		pBuf += sizeof(S_ANM_NODE);

		m_pNodes[i].InitHash();
		m_NodeHash.Add(m_pNodes[i].GetHashItem());
	}

	// チャンネル情報読み込み
	result = IZ_INPUT_READ(pIn, pBuf, 0, sizeof(S_ANM_CHANNEL) * m_Header.numChannels);
	VRETURN_NULL(result);

	// 読み込んだチャンネル情報分減らす
	nDataSize -= sizeof(S_ANM_CHANNEL) * m_Header.numChannels;

	// ノード情報とチャンネル情報を関連付ける
	for (IZ_UINT nKeyIdx = 0; nKeyIdx < m_Header.numNodes; ++nKeyIdx) {
		S_ANM_NODE& sNode = *m_pNodes[nKeyIdx].node;

		sNode.channels = reinterpret_cast<S_ANM_CHANNEL*>(pBuf);
		pBuf += sizeof(S_ANM_CHANNEL) * sNode.numChannels;
	}

	// チャンネル情報にキー情報用のバッファを割り当てる
	for (IZ_UINT nKeyIdx = 0; nKeyIdx < m_Header.numNodes; ++nKeyIdx) {
		S_ANM_NODE& sNode = *m_pNodes[nKeyIdx].node;

		for (IZ_UINT nChannelIdx = 0; nChannelIdx < sNode.numChannels; ++nChannelIdx) {
			sNode.channels[nChannelIdx].keys = reinterpret_cast<S_ANM_KEY**>(pBuf);
			pBuf += sizeof(S_ANM_KEY*) * sNode.channels[nChannelIdx].numKeys;
		}
	}

	// 残りのデータ（キー情報）読み込み
	result = IZ_INPUT_READ(pIn, pBuf, 0, nDataSize);
	VRETURN_NULL(result);

	// チャンネル情報にキー情報を関連付ける
	for (IZ_UINT nKeyIdx = 0; nKeyIdx < m_Header.numNodes; ++nKeyIdx) {
		S_ANM_NODE& sNode = *m_pNodes[nKeyIdx].node;

		for (IZ_UINT nChannelIdx = 0; nChannelIdx < sNode.numChannels; ++nChannelIdx) {
			S_ANM_CHANNEL& sChannel = sNode.channels[nChannelIdx];

			for (IZ_UINT nKeyIdx = 0; nKeyIdx < sChannel.numKeys; ++nKeyIdx) {
				sChannel.keys[nKeyIdx] = reinterpret_cast<S_ANM_KEY*>(pBuf);
				pBuf += sizeof(S_ANM_KEY);
#if 0
				pBuf += sizeof(IZ_FLOAT) * (sChannel.keys[nKeyIdx]->numParams - 1);
#else
				// パラメータへのポインタへ実データを割り当てる
				sChannel.keys[nKeyIdx]->params = reinterpret_cast<IZ_FLOAT*>(pBuf);
				pBuf += sizeof(IZ_FLOAT) * (sChannel.keys[nKeyIdx]->numParams);
#endif
			}
		}
	}

	return pBuf;
}

IZ_UINT CAnimation::ApplyAnimation(
	IZ_FLOAT time,
	const S_ANM_NODE* anmNode,
	CPoseUpdater& poseUpdater)
{
	const S_ANM_NODE& sAnmNode = *anmNode;
	const IZ_UINT nJointIdx = sAnmNode.targetIdx;

	SVector param;
	IZ_UINT updateFlag = 0;

	// 姿勢情報更新開始
	VRETURN_VAL(poseUpdater.BeginUpdate(nJointIdx), 0);

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
		poseUpdater.Update(
			nJointIdx,
			nTransformType,
			nParamType,
			param);
	}

	// 姿勢情報更新終了
	poseUpdater.EndUpdate(
		nJointIdx,
		updateFlag);

	return updateFlag;
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
	IZ_UINT jointIdx,
	IZ_FLOAT time)
{
	IZ_ASSERT(skl != IZ_NULL);
	IZ_ASSERT(jointIdx < skl->GetJointNum());

	const S_ANM_NODE* pAnmNode = GetAnmNodeByJointIdx(jointIdx);
	if (pAnmNode != IZ_NULL) {
		ApplyAnimation(
			time,
			pAnmNode,
			CSklPoseUpdater(skl));
	}
}

IZ_UINT CAnimation::GetPoseByIdx(
	S_SKL_JOINT_POSE& pose,
	IZ_UINT idx,
	IZ_FLOAT time)
{
	IZ_UINT updateFlag = 0;

	const S_ANM_NODE* pAnmNode = GetAnmNodeByJointIdx(idx);
	if (pAnmNode != IZ_NULL) {
		updateFlag = ApplyAnimation(
						time,
						pAnmNode,
						CPoseUpdater(&pose));
	}

	return updateFlag;
}

const S_ANM_NODE* CAnimation::GetAnmNodeByIdx(IZ_UINT idx)
{
	IZ_ASSERT(idx < m_Header.numNodes);
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
	for (IZ_UINT i = 0; i < m_Header.numNodes; ++i) {
		const SAnmNodeInstance& sNode = m_pNodes[i];
		if (sNode.node->targetIdx == nJointIdx) {
			return sNode.node;
		}
	}

	return IZ_NULL;
}
