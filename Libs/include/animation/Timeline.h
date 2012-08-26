#if !defined(__IZANAGI_ANIMATION_TIMELINE_H__)
#define __IZANAGI_ANIMATION_TIMELINE_H__

#include "izDefs.h"
#include "izStd.h"

namespace izanagi {
namespace animation {
	/**
	* タイムライン
	*/
	class CTimeline {
	public:
		/** タイムラインが設定時間を超えたときのハンドラ
		 */
		class CTimeOverHandler {
		public:
			CTimeOverHandler() {}
			virtual ~CTimeOverHandler() {}

		public:
			virtual void Handle(const CTimeline& timeline) = 0;
		};

	public:
		CTimeline();
		CTimeline(
			IZ_FLOAT duration,
			IZ_FLOAT delay);

		~CTimeline() {}

		CTimeline(const CTimeline& rhs);
		const CTimeline& operator=(const CTimeline& rhs);

	public:
		// 初期化
		void Init(
			IZ_FLOAT duration,
			IZ_FLOAT delay);

		// 進行
		void Advance(IZ_FLOAT delta);

		// リセット
		void Reset();

		// 開始
		void Start() { m_Flags.isPause = IZ_FALSE; }

		// ストップ
		inline void Stop();

		// ポーズ
		void Pause() { m_Flags.isPause = IZ_TRUE; }

		// 逆回し
		void Rewind() { m_Flags.isForward = !m_Flags.isForward; }

		// タイムラインが設定時間を超えたときのハンドラをセット
		void SetTimeOverHandler(CTimeOverHandler* handler) { m_TimeOverHandler = handler; }

	public:
		// 現在時間取得
		IZ_FLOAT GetTime() const { return m_Time; }

		IZ_FLOAT GetDuration() const { return m_Duration; }

		// [0.0f - 1.0f]で取得
		IZ_FLOAT GetFraction() const;

		// ループフラグセット
		void SetIsLoop(IZ_BOOL flag) { m_Flags.isLoop = flag; }

		// ループするかどうか
		IZ_BOOL IsLoop() const { return m_Flags.isLoop; }

		// 逆回転するかどうか
		void SetIsReverse(IZ_BOOL flag) { m_Flags.is_reverse = flag; }

		// 逆回転するかどうか
		IZ_BOOL IsReverse() const { return m_Flags.is_reverse; }

		// ポーズ中かどうか
		IZ_BOOL IsPause() const { return m_Flags.isPause; }

		// 順方向進行にするかどうかセット
		void EnableForwardDir(IZ_BOOL flag) { m_Flags.isForward = flag; }

		// 順方向進行かどうか
		IZ_BOOL IsForwardDir() const { return m_Flags.isForward; }

	protected:
		CStdList<CTimeline>::Item* GetListItem() { return &m_ListItem; }

	protected:
		IZ_FLOAT m_Time;		// 時間
		IZ_FLOAT m_DelayTime;

		IZ_FLOAT m_Duration;	// 期間
		IZ_FLOAT m_Delay;		// 遅延

		struct {
			IZ_UINT isLoop		: 1;	// ループするか
			IZ_UINT is_reverse	: 1;	// 逆回転するかどうか
			IZ_UINT isPause	: 1;	// ポーズ中かどうか
			IZ_UINT isForward	: 1;	// 順方向進行かどうか
		} m_Flags;

		CTimeOverHandler* m_TimeOverHandler;

		CStdList<CTimeline>::Item m_ListItem;
	};

}	// namespace animation
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_ANIMATION_TIMELINE_H__)
