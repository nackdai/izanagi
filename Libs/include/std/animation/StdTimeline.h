#if !defined(__URANUS_STD_STD_TIMELINE_H__)
#define __URANUS_STD_STD_TIMELINE_H__

#include "unDefs.h"
#include "std/container/StdList.h"

namespace uranus {
	/**
	* タイムライン
	*/
	class CStdTimeline {
	public:
		CStdTimeline();
		CStdTimeline(
			UN_FLOAT fDuration,
			UN_FLOAT fDelay);

		~CStdTimeline() {}

		CStdTimeline(const CStdTimeline& rhs);
		const CStdTimeline& operator=(const CStdTimeline& rhs);

	public:
		// 初期化
		inline void Init(
			UN_FLOAT fDuration,
			UN_FLOAT fDelay);

		// 進行
		void Advance(UN_FLOAT fDelta);

		// リセット
		inline void Reset();

		// 開始
		void Start() { m_Flags.is_pause = UN_TRUE; }

		// ストップ
		inline void Stop();

		// ポーズ
		void Pause() { m_Flags.is_pause = UN_TRUE; }

		// 逆回し
		void Rewind() { m_Flags.is_forward = !m_Flags.is_forward; }

	public:
		// 現在時間取得
		UN_FLOAT GetTime() const { return m_fTime; }

		UN_FLOAT GetDuration() const { return m_fDuration; }

		// [0.0f - 1.0f]で取得
		inline UN_FLOAT GetFraction() const;

		// ループフラグセット
		void SetIsLoop(UN_BOOL flag) { m_Flags.is_loop = flag; }

		// ループするかどうか
		UN_BOOL IsLoop() const { return m_Flags.is_loop; }

		// ポーズ中かどうか
		UN_BOOL IsPause() const { return m_Flags.is_pause; }

		// 順方向進行にするかどうかセット
		void EnableForwardDir(UN_BOOL flag) { m_Flags.is_forward = flag; }

		// 順方向進行かどうか
		UN_BOOL IsForwardDir() const { return m_Flags.is_forward; }

	protected:
		CStdList<CStdTimeline>::Item* GetListItem() { return &m_ListItem; }

	protected:
		UN_FLOAT m_fTime;		// 時間
		UN_FLOAT m_fDelayTime;

		UN_FLOAT m_fDuration;	// 期間
		UN_FLOAT m_fDelay;		// 遅延

		struct {
			UN_UINT is_loop		: 1;	// ループするか
			UN_UINT is_pause	: 1;	// ポーズ中かどうか
			UN_UINT is_forward	: 1;	// 順方向進行かどうか
		} m_Flags;

		CStdList<CStdTimeline>::Item m_ListItem;
	};

	// inline *********************************

	/**
	* 初期化
	*/
	void CStdTimeline::Init(
		UN_FLOAT fDuration,
		UN_FLOAT fDelay)
	{
		m_fDuration = fDuration;
		m_fDelay = fDelay;
	}

	/**
	* リセット
	*/
	void CStdTimeline::Reset()
	{
		m_fTime = 0.0f;
		m_fDelayTime = 0.0f;
	}

	/**
	* ストップ
	*/
	void CStdTimeline::Stop()
	{
		m_fTime = (IsForwardDir() ? 0.0f : m_fDuration);
		m_fDelayTime = 0.0f;

		m_Flags.is_pause = UN_TRUE;
	}

	/**
	* [0.0f - 1.0f]で取得
	*/
	UN_FLOAT CStdTimeline::GetFraction() const
	{
		UN_FLOAT ret = (m_fDuration != 0.0f ? m_fTime / m_fDuration : 0.0f);
		return ret;
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_STD_STD_TIMELINE_H__)
