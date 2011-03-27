#if !defined(__IZANAGI_SCENEGRAPH_MDL_SKELETON_H__)
#define __IZANAGI_SCENEGRAPH_MDL_SKELETON_H__

#include "MDLFormat.h"
#include "izMath.h"

namespace izanagi {
	class IInputStream;
	class CMdlJoint;
	class CAnimation;
	struct S_ANM_NODE;

	class CMdlSkeleton : public CObject {
		friend class CModelInstance;

	private:
		static CMdlSkeleton* CreateMdlSkeleton(
			IMemoryAllocator* pAllocator,
			CMdlJoint* pJoints);

		static IZ_UINT ComputeBufferSize(const CMdlJoint* pJoints);

	private:
		CMdlSkeleton();
		~CMdlSkeleton();

		NO_COPIABLE(CMdlSkeleton);

		IZ_DEFINE_INTERNAL_RELEASE();

	public:
		void BuildMatrix();

		const SMatrix* GetJointMtx(IZ_INT idx) const;

		IZ_UINT GetJointNum() { return m_nJointNodeNum; }

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
		IZ_UINT8* SetJointData(
			IZ_UINT8* pBuf,
			const CMdlJoint* pJoints);

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

		IZ_UINT m_nJointNodeNum;
		CMdlJoint* m_pJoints;

		S_MDL_JOINT_POSE* m_pJointPose;
		SMatrix* m_pGlobalPose;
	};
}	// namespace iznagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_MDL_SKELETON_H__)
