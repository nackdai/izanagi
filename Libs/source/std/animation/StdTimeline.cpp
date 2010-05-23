#include "std/animation/StdTimeline.h"

using namespace uranus;

// コンストラクタ
CStdTimeline::CStdTimeline()
{
	m_fTime = 0.0f;
	m_fDelayTime = 0.0f;

	m_fDuration = 0.0f;
	m_fDelay = 0.0f;

	m_Flags.is_loop = UN_FALSE;
	m_Flags.is_pause = UN_TRUE;

	m_ListItem.Init(this);
}

CStdTimeline::CStdTimeline(
	UN_FLOAT fDuration,
	UN_FLOAT fDelay)
{
	m_fTime = 0.0f;
	m_fDelayTime = 0.0f;

	m_fDuration = fDuration;
	m_fDelay = fDelay;

	m_Flags.is_loop = UN_FALSE;
	m_Flags.is_pause = UN_TRUE;

	m_ListItem.Init(this);
}

CStdTimeline::CStdTimeline(const CStdTimeline& rhs)
{
	*this = rhs;
}

const CStdTimeline& CStdTimeline::operator=(const CStdTimeline& rhs)
{
	m_fTime = rhs.m_fTime;
	m_fDelayTime = rhs.m_fDelayTime;

	m_fDuration = rhs.m_fDuration;
	m_fDelay = rhs.m_fDelay;

	m_Flags.is_loop = rhs.m_Flags.is_loop;
	m_Flags.is_pause = rhs.m_Flags.is_pause;
	m_Flags.is_forward = rhs.m_Flags.is_forward;

	return *this;
}

/**
* 進行
*/
void CStdTimeline::Advance(UN_FLOAT fDelta)
{
	if (IsPause()) {
		// ポーズ中なので何もしない
		return;
	}

	if (m_fDelayTime <= m_fDelay) {
		// 遅延
		m_fDelayTime += fDelta;

		if (m_fDelayTime <= m_fDelay) {
			// まだまだ始まらない
			return;
		}

		fDelta = m_fDelayTime - m_fDelay;
	}

	UN_BOOL bIsOver = UN_FALSE;
	
	if (IsForwardDir()) {
		m_fTime += fDelta;
		bIsOver = (m_fTime >= m_fDuration);
	}
	else {
		// 逆方向
		m_fTime -= fDelta;
		bIsOver = (m_fTime <= 0.0f);
	}

	if (bIsOver) {
		if (IsLoop()) {
			// 向きを変える
			Rewind();
		}

		// リセット
		Stop();

		if (IsLoop()) {
			// ループするので再開
			Start();
		}
	}
}
