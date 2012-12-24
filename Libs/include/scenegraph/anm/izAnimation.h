#if !defined(__IZANAGI_SCENEGRAPH_IZ_ANIMATION_H__)
#define __IZANAGI_SCENEGRAPH_IZ_ANIMATION_H__

#include "ANMFormat.h"
#include "AnimationInterface.h"

namespace izanagi {
    class IInputStream;
    class CSkeletonInstance;
    class CPoseUpdater;

    /**
    */
    class CAnimation : public IAnimation {
        static const IZ_UINT HASH_SIZE = 16;

        struct SAnmNodeInstance;

        typedef CStdHash<IZ_UINT, SAnmNodeInstance, HASH_SIZE>  CNodeHash;
        typedef CNodeHash::Item CNodeHashItem;

        struct SAnmNodeInstance {
            S_ANM_NODE* node;
            CNodeHashItem hashItem;

            void InitHash()
            {
                IZ_ASSERT(node != IZ_NULL);
                hashItem.Init(node->targetKey, this);
            }
            CNodeHashItem* GetHashItem() { return & hashItem; }
        };

    public:
        /** Create instance.
         */
        static CAnimation* CreateAnimation(
            IMemoryAllocator* pAllocator,
            IInputStream* pIn);

        /** スカラー値の補間処理を設定.
         */
        static void SetUserFuncInterpScalar(FuncInterpScalar func);

        /** ベクターの補間処理を設定.
         */
        static void SetUserFuncInterpVector(FuncInterpVector func);

    private:
        CAnimation();
        virtual ~CAnimation() {}

        NO_COPIABLE(CAnimation);

        IZ_DEFINE_INTERNAL_RELEASE();

    private:
        IZ_UINT8* Init(
            IInputStream* pIn,
            IZ_UINT8* pBuf);

        IZ_UINT ApplyAnimation(
            IZ_FLOAT time,
            const S_ANM_NODE* anmNode,
            CPoseUpdater& poseUpdater);

    public:
        /** 指定されたスケルトンにアニメーションを適用する.
         */
        void ApplyAnimation(
            CSkeletonInstance* skl,
            IZ_FLOAT time);

        /** 指定されたスケルトンの指定されたジョイントにアニメーションを適用する.
         */
        void ApplyAnimationByIdx(
            CSkeletonInstance* skl,
            IZ_UINT jointIdx,
            IZ_FLOAT time);

    protected:
        /** 指定されたジョイントのポーズ情報を取得.
         */
        IZ_UINT GetPoseByIdx(
            S_SKL_JOINT_POSE& pose,
            IZ_UINT idx,
            IZ_FLOAT time);

    public:
        const S_ANM_NODE* GetAnmNodeByIdx(IZ_UINT idx);
        const S_ANM_NODE* GetAnmNodeByName(IZ_PCSTR name);
        const S_ANM_NODE* GetAnmNodeByKey(IZ_UINT key);
        const S_ANM_NODE* GetAnmNodeByJointIdx(IZ_UINT nJointIdx);

    public:
        IZ_UINT GetNodelNum() const { return m_Header.numNodes; }
        IZ_FLOAT GetAnimationTime() const { return m_Header.time; }

        E_ANM_KEY_TYPE GetKeyType() const { return m_Header.keyType; }
        
    private:
        IMemoryAllocator* m_Allocator;

        S_ANM_HEADER m_Header;

        SAnmNodeInstance* m_pNodes;

        CNodeHash m_NodeHash;
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SCENEGRAPH_IZ_ANIMATION_H__)
