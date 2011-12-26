#include <algorithm>
#include "XFileAnimation.h"

CXFileAnimation* CXFileAnimation::s_pInstance = IZ_NULL;

CXFileAnimation& CXFileAnimation::GetInstance()
{
	if (s_pInstance == NULL) {
		s_pInstance = new CXFileAnimation();
	}
	return *s_pInstance;
}

void CXFileAnimation::DeleteInstance()
{
	SAFE_DELETE(s_pInstance);
}

namespace {
	inline IZ_INT _GetNodeIdxByName(
		const std::vector<SXFileNode*>& tvNodeList,
		std::string& name)
	{
		for (size_t i = 0; i < tvNodeList.size(); i++) {
			const SXFileNode* pNode = tvNodeList[i];
			if (pNode->name.compare(name) == 0) {
				return static_cast<IZ_INT>(i);
			}
		}
		return -1;
	}
}	// namespace

IZ_BOOL CXFileAnimation::Begin(
	CXFileParser& cParser,
	const std::vector<SXFileNode*>& tvNodeList,
	IZ_UINT nAnmSetIdx)
{
	const SXFileAnmSet* pAnmSet = cParser.GetAnmSetList()[nAnmSetIdx];

	m_AnmNodeList.resize(pAnmSet->anm.size());

	for (size_t i = 0; i < m_AnmNodeList.size(); i++) {
		SXFileAnm* pAnm = pAnmSet->anm[i];

		BreakAnmKeyMtx(pAnm);

		SAnmNode& sNode = m_AnmNodeList[i];
		{
			sNode.nameJoint = pAnm->name;

			IZ_INT idx = _GetNodeIdxByName(tvNodeList, sNode.nameJoint);
#if 0
			VRETURN(idx >= 0);
#else
			if (idx < 0) {
				continue;
			}
#endif

			sNode.idxJoint = idx;

			VRETURN(SetAnmChannels(pAnm, sNode));
		}
	}

	return IZ_TRUE;
}

IZ_UINT CXFileAnimation::GetAnmNodeNum()
{
	size_t ret = m_AnmNodeList.size();
	return static_cast<IZ_UINT>(ret);
}

IZ_UINT CXFileAnimation::GetAnmChannelNum(IZ_UINT nNodeIdx)
{
	IZ_ASSERT(nNodeIdx < GetAnmNodeNum());

	const SAnmNode& sNode = m_AnmNodeList[nNodeIdx];

	size_t ret = sNode.channels.size();
	return static_cast<IZ_UINT>(ret);
}

IZ_BOOL CXFileAnimation::GetAnmNode(
	IZ_UINT nNodeIdx,
	izanagi::S_ANM_NODE& sNode)
{
	VRETURN(nNodeIdx < GetAnmNodeNum());

	const SAnmNode& sAnmNode = m_AnmNodeList[nNodeIdx];

	sNode.target.SetString(sAnmNode.nameJoint.c_str());
	sNode.targetKey = sNode.target.GetKeyValue();

	sNode.targetIdx = sAnmNode.idxJoint;
	sNode.numChannels = static_cast<IZ_UINT16>(sAnmNode.channels.size());

	return IZ_TRUE;
}

IZ_BOOL CXFileAnimation::GetAnmChannel(
	IZ_UINT nNodeIdx,
	IZ_UINT nChannelIdx,
	izanagi::S_ANM_CHANNEL& sChannel)
{
	VRETURN(nNodeIdx < GetAnmNodeNum());

	const SAnmNode& sAnmNode = m_AnmNodeList[nNodeIdx];

	VRETURN(nChannelIdx < sAnmNode.channels.size());

	const SAnmChannel& sAnmChannel = sAnmNode.channels[nChannelIdx];

	sChannel.interp = sAnmChannel.interp;
	sChannel.numKeys = static_cast<IZ_UINT16>(sAnmChannel.keys.size());

	switch (sAnmChannel.type) {
	case izanagi::E_ANM_TRANSFORM_TYPE_QUATERNION:
		sChannel.stride = 4;
		break;
	case izanagi::E_ANM_TRANSFORM_TYPE_SCALE:
		sChannel.stride = 3;
		break;
	case izanagi::E_ANM_TRANSFORM_TYPE_TRANSLATE:
		sChannel.stride = 3;
		break;
	default:
		VRETURN(IZ_FALSE);
		break;
	}

	sChannel.type = sAnmChannel.type;
	if (sChannel.type & izanagi::E_ANM_TRANSFORM_TYPE_QUATERNION) {
		sChannel.type |= izanagi::E_ANM_TRANSFORM_TYPE_XYZW;
	}
	else {
		sChannel.type |= izanagi::E_ANM_TRANSFORM_TYPE_XYZ;
	}

	return IZ_TRUE;
}

IZ_BOOL CXFileAnimation::GetAnmKey(
	IZ_UINT nNodeIdx,
	IZ_UINT nChannelIdx,
	IZ_UINT nKeyIdx,
	izanagi::S_ANM_KEY& sKey,
	std::vector<IZ_FLOAT>& tvValue)
{
	VRETURN(nNodeIdx < GetAnmNodeNum());

	const SAnmNode& sAnmNode = m_AnmNodeList[nNodeIdx];

	VRETURN(nChannelIdx < sAnmNode.channels.size());

	const SAnmChannel& sAnmChannel = sAnmNode.channels[nChannelIdx];

	VRETURN(nKeyIdx < sAnmChannel.keys.size());

	const SAnmKey& sAnmKey = sAnmChannel.keys[nKeyIdx];

	sKey.keyTime = static_cast<IZ_FLOAT>(sAnmKey.time);

	switch (sAnmChannel.type) {
	case izanagi::E_ANM_TRANSFORM_TYPE_QUATERNION:
		sKey.numParams = 4;
		sKey.stride = 4;
		break;
	case izanagi::E_ANM_TRANSFORM_TYPE_SCALE:
		sKey.numParams = 3;
		sKey.stride = 3;
		break;
	case izanagi::E_ANM_TRANSFORM_TYPE_TRANSLATE:
		sKey.numParams = 3;
		sKey.stride = 3;
		break;
	default:
		VRETURN(IZ_FALSE);
		break;
	}

	tvValue.resize(sKey.numParams);

	switch (sAnmChannel.type) {
	case izanagi::E_ANM_TRANSFORM_TYPE_QUATERNION:
		tvValue[0] = sAnmKey.param.x;
		tvValue[1] = sAnmKey.param.y;
		tvValue[2] = sAnmKey.param.z;
		tvValue[3] = sAnmKey.param.w;
		break;
	case izanagi::E_ANM_TRANSFORM_TYPE_SCALE:
	case izanagi::E_ANM_TRANSFORM_TYPE_TRANSLATE:
		tvValue[0] = sAnmKey.param.x;
		tvValue[1] = sAnmKey.param.y;
		tvValue[2] = sAnmKey.param.z;
		break;
	}

	return IZ_TRUE;
}

IZ_BOOL CXFileAnimation::BreakAnmKeyMtx(SXFileAnm* pAnm)
{
	for (size_t i = 0; i < pAnm->keys.size(); i++) {
		SXFileAnmKey& sAnmKey = pAnm->keys[i];
		if (!sAnmKey.type.mtx) {
			// This key has not matrix.
			continue;
		}

		izanagi::S_SKL_JOINT_POSE sPoseTmp;

		IZ_BOOL bHasScale = CXFileMathUtil::BreakDownMatrix(
								sAnmKey.data.mtx,
								sPoseTmp);
		VRETURN(!bHasScale);

		memcpy(&sAnmKey.data.pose, &sPoseTmp, sizeof(izanagi::S_SKL_JOINT_POSE));

		sAnmKey.type.mtx = IZ_FALSE;

		// TODO
		sAnmKey.type.quat = IZ_TRUE;
		sAnmKey.type.scale = IZ_TRUE;
		sAnmKey.type.trans = IZ_TRUE;
	}

	return IZ_TRUE;
}

IZ_BOOL CXFileAnimation::SetAnmChannels(
	const SXFileAnm* pAnm,
	SAnmNode& sNode)
{
	enum {
		CHANNEL_QUAT = 0,
		CHANNEL_SCALE,
		CHANNEL_TRANS,

		CHANNEL_NUM,
	};

	sNode.channels.resize(CHANNEL_NUM);
	{
		sNode.channels[CHANNEL_QUAT].interp  = izanagi::E_ANM_INTERP_TYPE_SLERP;
		sNode.channels[CHANNEL_QUAT].type = izanagi::E_ANM_TRANSFORM_TYPE_QUATERNION;

		sNode.channels[CHANNEL_SCALE].interp = izanagi::E_ANM_INTERP_TYPE_LINEAR;
		sNode.channels[CHANNEL_SCALE].type = izanagi::E_ANM_TRANSFORM_TYPE_SCALE;

		sNode.channels[CHANNEL_TRANS].interp = izanagi::E_ANM_INTERP_TYPE_LINEAR;
		sNode.channels[CHANNEL_TRANS].type = izanagi::E_ANM_TRANSFORM_TYPE_TRANSLATE;
	}

	for (size_t i = 0; i < pAnm->keys.size(); i++) {
		const SXFileAnmKey& sAnmKey = pAnm->keys[i];
		VRETURN(sAnmKey.type.mtx == 0);
		VRETURN(sAnmKey.type.val > 0);

		if (sAnmKey.type.quat) {
			SAnmChannel& sChannel = sNode.channels[CHANNEL_QUAT];

			SAnmKey sKey;
			{
				sKey.time = sAnmKey.time;
				izanagi::SQuat::Copy(sKey.param, sAnmKey.data.pose.quat);
			}
			sChannel.keys.push_back(sKey);
		}
		if (sAnmKey.type.scale) {
			SAnmChannel& sChannel = sNode.channels[CHANNEL_SCALE];

			SAnmKey sKey;
			{
				sKey.time = sAnmKey.time;
				sKey.param.x = sAnmKey.data.pose.scale[0];
				sKey.param.y = sAnmKey.data.pose.scale[1];
				sKey.param.z = sAnmKey.data.pose.scale[2];
			}
			sChannel.keys.push_back(sKey);
		}
		if (sAnmKey.type.trans) {
			SAnmChannel& sChannel = sNode.channels[CHANNEL_TRANS];

			SAnmKey sKey;
			{
				sKey.time = sAnmKey.time;
				sKey.param.x = sAnmKey.data.pose.trans[0];
				sKey.param.y = sAnmKey.data.pose.trans[1];
				sKey.param.z = sAnmKey.data.pose.trans[2];
			}
			sChannel.keys.push_back(sKey);
		}
	}
	
	return IZ_TRUE;
}
