#if !defined(__IZANAGI_SCENEGRAPH_ANIMATION_INTERP_H__)
#define __IZANAGI_SCENEGRAPH_ANIMATION_INTERP_H__

#include "AnimationInterface.h"

namespace izanagi {
    class CSkeletonInstance;
    class CPoseUpdater;

    /**
     */
    class CAnimationInterp : public IAnimation {
    public:
        // インスタンス作成
        static CAnimationInterp* CreateAnimationInterp(IMemoryAllocator* allocator);

        enum E_INTERP_TYPE {
            E_INTERP_TYPE_FROZEN = 0,
            E_INTERP_TYPE_SMOOTH,

            E_INTERP_TYPE_NUM,
        };

    protected:
        CAnimationInterp();
        virtual ~CAnimationInterp() {}

        NO_COPIABLE(CAnimationInterp);

        IZ_DEFINE_INTERNAL_RELEASE();

    public:
        IZ_BOOL SetAnimation(
            IZ_FLOAT interpTime,
            E_INTERP_TYPE type,
            IAnimation* startAnm,
            IZ_FLOAT targetStart,
            IAnimation* goalAnm,
            IZ_FLOAT targetGoal);

        void ApplyAnimation(
            CSkeletonInstance* skl,
            IZ_FLOAT time);

        void ApplyAnimationByIdx(
            CSkeletonInstance* skl,
            IZ_UINT jointIdx,
            IZ_FLOAT time);

    public:
        IZ_FLOAT GetAnimationTime() const { return m_InterpTime; }

    protected:
        IZ_UINT ApplyAnimation(
            IZ_FLOAT time,
            IZ_UINT nJointIdx,
            CPoseUpdater& poseUpdater);

        IZ_UINT ApplyAnimationBySmooth(
            IZ_FLOAT time,
            IZ_UINT nJointIdx,
            CPoseUpdater& poseUpdater);

        void UpdatePose(
            const S_SKL_JOINT_POSE& pose,
            IZ_UINT nJointIdx,
            IZ_UINT updateFlag,
            CPoseUpdater& poseUpdater);

        IZ_UINT GetPoseByIdx(
            S_SKL_JOINT_POSE& pose,
            IZ_UINT idx,
            IZ_FLOAT time);

    private:
        // 補間アニメ情報
        struct SInterpAnm {
            IAnimation* anm;        // 対象アニメ
            IZ_FLOAT targetTime;    // 補間元（or 先）のアニメ時間

            SInterpAnm()
            {
                anm = IZ_NULL;
                targetTime = 0.0f;
            }

            ~SInterpAnm()
            {
                SAFE_RELEASE(anm);
            }

            void Set(
                IAnimation* targetAnm,
                IZ_FLOAT time)
            {
                SAFE_REPLACE(anm, targetAnm);
                targetTime = time;
            }
        };

    private:
        IMemoryAllocator* m_Allocator;

        // 補間するアニメーション
        SInterpAnm m_InterpAnm[2];

        // 補間にかける時間
        IZ_FLOAT m_InterpTime;

        // 補間方法
        E_INTERP_TYPE m_Type;

        S_ANM_KEY* mKeys[2];

        // 補間計算用キー
        S_ANM_KEY mStartKey;
        S_ANM_KEY mGoalKey;

        // 補間計算用キーパラメータバッファ
        IZ_FLOAT mStartKeyParamBuf[4];
        IZ_FLOAT mGoalKeyParamBuf[4];
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SCENEGRAPH_ANIMATION_INTERP_H__)
