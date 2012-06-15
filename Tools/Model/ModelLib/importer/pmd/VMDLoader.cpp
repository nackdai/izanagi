#include "VMDLoader.h"
#include "PMDLoader.h"

// ファイルロード
IZ_BOOL CVmdLoader::Load(IZ_PCSTR path)
{
	// Read all data.
	izanagi::CFileInputStream stream;
	VRETURN(stream.Open(path));

	// ファイルヘッダ
	IZ_INPUT_READ_VRETURN(&stream, &m_Header, 0, sizeof(m_Header));

	// フレームチャンク
	VRETURN(LoadFrameChunk(&stream));

	return IZ_TRUE;
}

// クリア
void CVmdLoader::Clear()
{
	m_FrameList.clear();
}

// フレームデータ取得
const SVmdFrame& CVmdLoader::GetFrame(IZ_UINT idx)
{
	IZ_ASSERT(idx < m_FrameList.size());
	return m_FrameList[idx];
}

// 移動アニメーションが有効かどうか
IZ_BOOL CVmdLoader::IsValidTrans(IZ_UINT idx)
{
	if (m_FrameOrder[idx].size() > 0)
	{
		// TODO
		// 最初だけみればＯＫ？
		const SVmdFrame& frame = m_FrameOrder[idx].begin()->frame;

		IZ_BOOL ret = (frame.trans[0] != 0.0f
						|| frame.trans[1] != 0.0f
						|| frame.trans[2] != 0.0f);
		return ret;
	}

	return IZ_FALSE;
}

// 回転アニメーションが有効かどうか
IZ_BOOL CVmdLoader::IsValidRotate(IZ_UINT idx)
{
	if (m_FrameOrder[idx].size() > 0)
	{
		// TODO
		// 最初だけみればＯＫ？
		const SVmdFrame& frame = m_FrameOrder[idx].begin()->frame;

		IZ_BOOL ret = (frame.quat[0] != 0.0f
						|| frame.quat[1] != 0.0f
						|| frame.quat[2] != 0.0f
						|| frame.quat[3] != 1.0f);
		return ret;
	}

	return IZ_FALSE;
}

// フレームの整列を行う
IZ_BOOL CVmdLoader::MakeFrameOrder(const CPmdLoader& pmd)
{
	m_FrameOrder.resize(pmd.GetBoneNum());

	for (IZ_UINT i = 0; i < m_FrameList.size(); i++)
	{
		const SVmdFrame& frame = m_FrameList[i];

		IZ_INT idx = pmd.GetIdxFromBoneName(frame.jointName);

		// TODO
		//VRETURN(idx >= 0);

		if (idx >= 0)
		{
			m_FrameOrder[idx].insert(SFrameInfo(frame));
		}
	}

	return IZ_TRUE;
}

// ボーンごとにまとめられたフレーム情報を取得
const std::set<SFrameInfo>& CVmdLoader::GetFrameInfo(IZ_UINT idx)
{
	IZ_ASSERT(idx < m_FrameOrder.size());
	return m_FrameOrder[idx];
}

// フレーム情報を取得.
const SVmdFrame* CVmdLoader::GetFrameInfo(
	IZ_UINT nodeIdx,
	IZ_UINT frameIdx)
{
	const std::set<SFrameInfo>& infoList = GetFrameInfo(nodeIdx);

	std::set<SFrameInfo>::const_iterator it = infoList.begin();

	for (IZ_UINT i = 0; it != infoList.end(); i++, it++)
	{
		if (i == frameIdx)
		{
			return &(*it).frame;
		}
	}

	IZ_ASSERT(IZ_FALSE);
	return IZ_NULL;
}

IZ_BOOL CVmdLoader::LoadFrameChunk(izanagi::IInputStream* stream)
{
	for (IZ_UINT i = 0; i < m_Header.frameNum; i++)
	{
		SVmdFrame frame;
		IZ_INPUT_READ_VRETURN(stream, &frame, 0, sizeof(frame));
		m_FrameList.push_back(frame);
	}

	return IZ_TRUE;
}
