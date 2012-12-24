#if !defined(__IZANAGI_SCENEGRAPH_ANIMATION_BINDER_H__)
#define __IZANAGI_SCENEGRAPH_ANIMATION_BINDER_H__

#include "AnimationInterface.h"
#include "scenegraph/skl/SKLFormat.h"

namespace izanagi {
    class CSkeleton;
    class CSkeletonInstance;

    /**
     */
    class CAnimationBinder : public IAnimation {
    public:
        // インスタンス作成
        static CAnimationBinder* CreateAnimationBinder(
            IMemoryAllocator* allocator,
            CSkeleton* skl);

    protected:
        CAnimationBinder();
        virtual ~CAnimationBinder();

        NO_COPIABLE(CAnimationBinder);

        IZ_DEFINE_INTERNAL_RELEASE();

    public:
        IZ_BOOL BindByIdx(
            IZ_UINT idx,
            IAnimation* anm);

        IZ_BOOL BindByName(
            IZ_PCSTR name,
            IAnimation* anm);

        IZ_BOOL BindByKey(
            IZ_UINT key,
            IAnimation* anm);

        void ApplyAnimation(
            CSkeletonInstance* skl,
            IZ_FLOAT time);

        void ApplyAnimationByIdx(
            CSkeletonInstance* skl,
            IZ_UINT jointIdx,
            IZ_FLOAT time);

        IZ_FLOAT GetAnimationTime() const { return GetMaxAnimationTime(); }

        IZ_FLOAT GetMinAnimationTime() const { return m_MinTime; }
        IZ_FLOAT GetMaxAnimationTime() const { return m_MaxTime; }

    protected:
        IZ_UINT GetPoseByIdx(
            S_SKL_JOINT_POSE& pose,
            IZ_UINT idx,
            IZ_FLOAT time);

    private:
        struct SJointInfo;

        enum {
            HASH_SIZE = 5,
        };

        typedef CStdHash<IZ_UINT, SJointInfo, HASH_SIZE>    CJointInfoHash;
        typedef CJointInfoHash::Item    CJointInfoHashItem;

        struct SJointInfo {
            IZ_UINT jointIdx;

            CStdString<izChar, SKL_NAME_LEN> name;
            IZ_UINT key;

            IAnimation* anm;

            CJointInfoHashItem hashItem;

            void Init(
                IZ_UINT idx,
                const izChar* n)
            {
                jointIdx = idx;

                name.SetString(n);
                key = name.GetKeyValue();

                hashItem.Init(key, this);
            }
        };

    private:
        IMemoryAllocator* m_Allocator;

        IZ_UINT m_JointNum;
        SJointInfo* m_Joints;

        CJointInfoHash m_JointHash;

        IZ_FLOAT m_MinTime;
        IZ_FLOAT m_MaxTime;

        CSkeleton* m_Skl;
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SCENEGRAPH_ANIMATION_BINDER_H__)
