#if !defined(__MODEL_LIB_XFILE_ANIMATION_H__)
#define __MODEL_LIB_XFILE_ANIMATION_H__

#include <vector>
#include "izSceneGraph.h"
#include "XFileParser.h"

class CXFileAnimation {
	static CXFileAnimation* s_pInstance;

private:
	struct SAnmKey {
		IZ_UINT time;
		izanagi::SVector param;
	};

	struct SAnmChannel {
		izanagi::E_ANM_INTERP_TYPE interp;
		izanagi::E_ANM_TRANSFORM_TYPE type;
		std::vector<SAnmKey> keys;
	};

	struct SAnmNode {
		std::string nameJoint;
		IZ_UINT idxJoint;

		std::vector<SAnmChannel> channels;
	};

public:
	static CXFileAnimation& GetInstance();
	static void DeleteInstance();

private:
	CXFileAnimation() {}
	~CXFileAnimation() {}

public:
	IZ_BOOL Begin(
		CXFileParser& cParser,
		const std::vector<SXFileNode*>& tvNodeList,
		IZ_UINT nAnmSetIdx);

	IZ_UINT GetAnmNodeNum();

	IZ_UINT GetAnmChannelNum(IZ_UINT nNodeIdx);

	IZ_BOOL GetAnmNode(
		IZ_UINT nNodeIdx,
		izanagi::S_ANM_NODE& sNode);

	IZ_BOOL GetAnmChannel(
		IZ_UINT nNodeIdx,
		IZ_UINT nChannelIdx,
		izanagi::S_ANM_CHANNEL& sChannel);

	IZ_BOOL GetAnmKey(
		IZ_UINT nNodeIdx,
		IZ_UINT nChannelIdx,
		IZ_UINT nKeyIdx,
		izanagi::S_ANM_KEY& sKey,
		std::vector<IZ_FLOAT>& tvValue);

private:
	IZ_BOOL BreakAnmKeyMtx(SXFileAnm* pAnm);

	IZ_BOOL SetAnmChannels(
		const SXFileAnm* pAnm,
		SAnmNode& sNode);

private:
	std::vector<SAnmNode> m_AnmNodeList;
};

#endif	// #if !defined(__MODEL_LIB_XFILE_ANIMATION_H__)
