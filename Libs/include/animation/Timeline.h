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

        IZ_DECL_PLACEMENT_NEW();

    public:
        /** Initialize timeline. 
         */
        void Init(
            IZ_FLOAT duration,
            IZ_FLOAT delay);

        /** Start timeline.
         */
        void Start();

        /** Stop timeline.
         */
        void Stop();

        /** Pause timeline
         */
        void Pause();

        /** Reset timeline.
         */
        void Reset();

        /** Advance timeline by specified delta time.
         */
        void Advance(IZ_FLOAT delta);

        /** Rewind timeline.
         */
        void Rewind();

        /** Set handler if timeline is over the specified time.
         */
        void SetTimeOverHandler(CTimeOverHandler* handler);

        /** Get current time.
         */
        IZ_FLOAT GetTime() const;

        /** Get duration.
         */
        IZ_FLOAT GetDuration() const;

        /** Get normalized time.
         */
        IZ_FLOAT GetNormalized() const;

        /** Set if timeline is loop.
         */
        void EnableLoop(IZ_BOOL enable);

        /** Get if timeline is loop.
         */
        IZ_BOOL IsLoop() const;

        /** Set whether timeline is reverse when timeline is loop.
         */
        void AutoReverse(IZ_BOOL enable);

        /** Get whether timeline is reverse when timeline is loop.
         */ 
        IZ_BOOL WillReverse() const;

        /** Get whether timeline is posed.
         */
        IZ_BOOL IsPaused() const;

        /** Get whether timeline runs forward.
         */
        IZ_BOOL IsForward() const;

    protected:
        CStdList<CTimeline>::Item* GetListItem();

    protected:
        IZ_FLOAT m_Time;        // 時間
        IZ_FLOAT m_DelayTime;

        IZ_FLOAT m_Duration;    // 期間
        IZ_FLOAT m_Delay;       // 遅延

        struct {
            IZ_UINT isLoop      : 1;    // ループするか
            IZ_UINT isReverse   : 1;    // 逆回転するかどうか
            IZ_UINT isPause     : 1;    // ポーズ中かどうか
            IZ_UINT isForward   : 1;    // 順方向進行かどうか
        } m_Flags;

        CTimeOverHandler* m_TimeOverHandler;

        CStdList<CTimeline>::Item m_ListItem;
    };

}   // namespace animation
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_ANIMATION_TIMELINE_H__)
