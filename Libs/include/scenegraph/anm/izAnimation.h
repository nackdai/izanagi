#if !defined(__IZANAGI_SCENEGRAPH_IZ_ANIMATION_H__)
#define __IZANAGI_SCENEGRAPH_IZ_ANIMATION_H__

#include "ANMFormat.h"
#include "AnimationInterface.h"

namespace izanagi {
	class IInputStream;
	class CSkeletonInstance;

	/**
	*/
	class CAnimation : public IAnimation {
		static const IZ_UINT HASH_SIZE = 16;

		struct SAnmNodeInstance;

		typedef CStdHash<IZ_UINT, SAnmNodeInstance, HASH_SIZE>	CNodeHash;
		typedef CNodeHash::Item	CNodeHashItem;

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
		// Create instance.
		static CAnimation* CreateAnimation(
			IMemoryAllocator* pAllocator,
			IInputStream* pIn);

	private:
		CAnimation();
		virtual ~CAnimation() {}

		NO_COPIABLE(CAnimation);

		IZ_DEFINE_INTERNAL_RELEASE();

	private:
		IZ_UINT8* Init(
			IInputStream* pIn,
			IZ_UINT8* pBuf);

		void ApplyAnimation(
			CSkeletonInstance* skl,
			IZ_FLOAT time,
			const S_ANM_NODE* anmNode);

	public:
		// アニメーション適用
		void ApplyAnimation(
			CSkeletonInstance* skl,
			IZ_FLOAT time);

		void ApplyAnimationByIdx(
			CSkeletonInstance* skl,
			IZ_UINT nJointIdx,
			IZ_FLOAT fTime);

		void ApplyAnimationByName(
			CSkeletonInstance* skl,
			IZ_PCSTR pszJointName,
			IZ_FLOAT fTime);

		void ApplyAnimationByKey(
			CSkeletonInstance* skl,
			IZ_UINT nJointKey,
			IZ_FLOAT fTime);

	public:
		const S_ANM_NODE* GetAnmNodeByIdx(IZ_UINT idx);
		const S_ANM_NODE* GetAnmNodeByName(IZ_PCSTR name);
		const S_ANM_NODE* GetAnmNodeByKey(IZ_UINT key);
		const S_ANM_NODE* GetAnmNodeByJointIdx(IZ_UINT nJointIdx);

	public:
		IZ_UINT GetNodelNum() const { return m_sHeader.numNodes; }
		IZ_FLOAT GetAnimationTime() const { return m_sHeader.time; }

		E_ANM_KEY_TYPE GetKeyType() const { return m_sHeader.keyType; }

	private:
		IMemoryAllocator* m_pAllocator;

		S_ANM_HEADER m_sHeader;

		SAnmNodeInstance* m_pNodes;

		CNodeHash m_NodeHash;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_IZ_ANIMATION_H__)
