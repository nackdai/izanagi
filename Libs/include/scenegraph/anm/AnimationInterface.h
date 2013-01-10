#if !defined(__IZANAGI_SCENEGRAPH_ANIMATION_INTERFACE_H__)
#define __IZANAGI_SCENEGRAPH_ANIMATION_INTERFACE_H__

#include "izStd.h"
#include "ANMFormat.h"

namespace izanagi {
    class CSkeletonInstance;
    struct S_SKL_JOINT_POSE;

    /**
     */
    class IAnimation : public CObject {
    protected:
        IAnimation() {}
        virtual ~IAnimation() {}

        NO_COPIABLE(IAnimation);

    public:
        /** 指定されたスケルトンにアニメーションを適用する.
         */
        PURE_VIRTUAL(
            void ApplyAnimation(
                CSkeletonInstance* skl,
                IZ_FLOAT time));

        /** 指定されたスケルトンの指定されたジョイントにアニメーションを適用する.
         */
        PURE_VIRTUAL(
            void ApplyAnimationByIdx(
                CSkeletonInstance* skl,
                IZ_UINT jointIdx,
                IZ_FLOAT time));

        PURE_VIRTUAL(IZ_FLOAT GetAnimationTime() const);

        /** 指定されたジョイントのポーズ情報を取得.
         */
        PURE_VIRTUAL(
            IZ_UINT GetPoseByIdx(
                S_SKL_JOINT_POSE& pose,
                IZ_UINT idx,
                IZ_FLOAT time));
    };

    class CAnimationCachable : public IAnimation
    {
    protected:
        CAnimationCachable()
        {
            m_AppliedTime = -1.0f;
            m_UpdatedFlag = 0;
        }
        virtual ~CAnimationCachable() {}

    protected:
        IZ_BOOL IsCachedAnimation(IZ_FLOAT time, IZ_UINT& updateFlag)
        {
            IZ_BOOL disable = math::CMath::IsNearyEqual(m_AppliedTime, time, 1.0f / 60.0f);
            IZ_BOOL enable = !disable;

            m_AppliedTime = time;
            updateFlag = m_UpdatedFlag;

            return IZ_FALSE;;
        }

        void SetUpdatedFlag(IZ_UINT updateFlag)
        {
            m_UpdatedFlag = updateFlag;
        }

    private:
        IZ_FLOAT m_AppliedTime;
        IZ_UINT m_UpdatedFlag;
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SCENEGRAPH_ANIMATION_INTERFACE_H__)
