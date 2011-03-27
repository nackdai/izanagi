#include "scenegraph/anm/izAnimation.h"
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
