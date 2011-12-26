#if !defined(__IZANAGI_STD_STD_TIMELINE_H__)
#define __IZANAGI_STD_STD_TIMELINE_H__

#include "izDefs.h"
#include "std/container/StdList.h"

namespace izanagi {
	/**
	* タイムライン
	*/
	class CStdTimeline {
	public:
		/** タイムラインが設定時間を超えたときのハンドラ
		 */
		class CTimeOverHandler {
		public:
			CTimeOverHandler() {}
			virtual ~CTimeOverHandler() {}

		public:
			virtual void Handle(const CStdTimeline& timeline) = 0;
		};

	public:
		CStdTimeline();
		CStdTimeline(
			IZ_FLOAT fDuration,
			IZ_FLOAT fDelay);

		~CStdTimeline() {}

		CStdTimeline(const CStdTimeline& rhs);
		const CStdTimeline& operator=(const CStdTimeline& rhs);

	public:
		// 初期化
		inline void Init(
			IZ_FLOAT fDuration,
			IZ_FLOAT fDelay);

		// 進行
		void Advance(IZ_FLOAT fDelta);

		// リセット
		inline void Reset();

		// 開始
		void Start() { m_Flags.is_pause = IZ_FALSE; }

		// ストップ
		inline void Stop();

		// ポーズ
		void Pause() { m_Flags.is_pause = IZ_TRUE; }

		// 逆回し
		void Rewind() { m_Flags.is_forward = !m_Flags.is_forward; }

		// タイムラインが設定時間を超えたときのハンドラをセット
		void SetTimeOverHandler(CTimeOverHandler* handler) { m_TimeOverHandler = handler; }

	public:
		// 現在時間取得
		IZ_FLOAT GetTime() const { return m_fTime; }

		IZ_FLOAT GetDuration() const { return m_fDuration; }

		// [0.0f - 1.0f]で取得
		inline IZ_FLOAT GetFraction() const;

		// ループフラグセット
		void SetIsLoop(IZ_BOOL flag) { m_Flags.is_loop = flag; }

		// ループするかどうか
		IZ_BOOL IsLoop() const { return m_Flags.is_loop; }

		// 逆回転するかどうか
		void SetIsReverse(IZ_BOOL flag) { m_Flags.is_reverse = flag; }

		// 逆回転するかどうか
		IZ_BOOL IsReverse() const { return m_Flags.is_reverse; }

		// ポーズ中かどうか
		IZ_BOOL IsPause() const { return m_Flags.is_pause; }

		// 順方向進行にするかどうかセット
		void EnableForwardDir(IZ_BOOL flag) { m_Flags.is_forward = flag; }

		// 順方向進行かどうか
		IZ_BOOL IsForwardDir() const { return m_Flags.is_forward; }

	protected:
		CStdList<CStdTimeline>::Item* GetListItem() { return &m_ListItem; }

	protected:
		IZ_FLOAT m_fTime;		// 時間
		IZ_FLOAT m_fDelayTime;

		IZ_FLOAT m_fDuration;	// 期間
		IZ_FLOAT m_fDelay;		// 遅延

		struct {
			IZ_UINT is_loop		: 1;	// ループするか
			IZ_UINT is_reverse	: 1;	// 逆回転するかどうか
			IZ_UINT is_pause	: 1;	// ポーズ中かどうか
			IZ_UINT is_forward	: 1;	// 順方向進行かどうか
		} m_Flags;

		CTimeOverHandler* m_TimeOverHandler;

		CStdList<CStdTimeline>::Item m_ListItem;
	};

	// inline *********************************

	/**
	* 初期化
	*/
	void CStdTimeline::Init(
		IZ_FLOAT fDuration,
		IZ_FLOAT fDelay)
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

		m_Flags.is_pause = IZ_TRUE;
	}

	/**
	* [0.0f - 1.0f]で取得
	*/
	IZ_FLOAT CStdTimeline::GetFraction() const
	{
		IZ_FLOAT ret = (m_fDuration != 0.0f ? m_fTime / m_fDuration : 0.0f);
		return ret;
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_STD_STD_TIMELINE_H__)
