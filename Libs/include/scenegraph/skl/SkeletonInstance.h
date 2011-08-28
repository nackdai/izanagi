#if !defined(__IZANAGI_SCENEGRAPH_SKELETON_INSTANCE_H__)
#define __IZANAGI_SCENEGRAPH_SKELETON_INSTANCE_H__

#include "SKLFormat.h"
#include "izMath.h"

namespace izanagi {
	class IInputStream;
	class CSkeleton;
	class CAnimation;
	struct S_ANM_NODE;

	class CSkeletonInstance : public CObject {
		friend class CModel;

	public:
		static CSkeletonInstance* CreateSkeletonInstance(
			IMemoryAllocator* pAllocator,
			CSkeleton* pSkl);

	private:
		static IZ_UINT ComputeBufferSize(const CSkeleton* pSkl);

	private:
		CSkeletonInstance();
		~CSkeletonInstance();

		NO_COPIABLE(CSkeletonInstance);

		IZ_DEFINE_INTERNAL_RELEASE();

	public:
		void BuildMatrix(const SMatrix* mtxL2W);

		const SMatrix* GetJointMtx(IZ_INT idx) const;

		IZ_UINT GetJointNum() { return m_nJointNum; }

		void ApplyAnimation(
			IZ_FLOAT fTime,
			CAnimation* pAnm);

		void ApplyAnimationByIdx(
			IZ_UINT nJointIdx,
			IZ_FLOAT fTime,
			CAnimation* pAnm);

		void ApplyAnimationByName(
			IZ_PCSTR pszJointName,
			IZ_FLOAT fTime,
			CAnimation* pAnm);

		void ApplyAnimationByKey(
			IZ_UINT nJointKey,
			IZ_FLOAT fTime,
			CAnimation* pAnm);

	private:
		IZ_UINT8* SetJointData(IZ_UINT8* pBuf);

		void BuildLocalMatrix(IZ_UINT nIdx);

		void ApplyAnimation(
			IZ_FLOAT fTime,
			const S_ANM_NODE* pAnmNode);

		inline void ApplyParentMatrix(
			IZ_UINT nIdx,
			IZ_UINT nParentIdx);

		inline void ApplyInvBindMatrix(IZ_UINT nIdx);

	private:
		IMemoryAllocator* m_pAllocator;

		CSkeleton* m_pBody;

		IZ_UINT m_nJointNum;

		S_SKL_JOINT_POSE* m_pJointPose;
		SMatrix* m_pGlobalPose;
	};
}	// namespace iznagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_SKELETON_INSTANCE_H__)
