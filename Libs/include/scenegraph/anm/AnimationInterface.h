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
		friend class CAnimationInterp;

	protected:
		IAnimation() {}
		virtual ~IAnimation() {}

		NO_COPIABLE(IAnimation);

	public:
		PURE_VIRTUAL(
			void ApplyAnimation(
				CSkeletonInstance* skl,
				IZ_FLOAT time));

		PURE_VIRTUAL(
			void ApplyAnimationByIdx(
				CSkeletonInstance* skl,
				IZ_UINT nJointIdx,
				IZ_FLOAT fTime));

		PURE_VIRTUAL(
			void ApplyAnimationByName(
				CSkeletonInstance* skl,
				IZ_PCSTR pszJointName,
				IZ_FLOAT fTime));

		PURE_VIRTUAL(
			void ApplyAnimationByKey(
				CSkeletonInstance* skl,
				IZ_UINT nJointKey,
				IZ_FLOAT fTime));

		PURE_VIRTUAL(IZ_FLOAT GetAnimationTime() const);

	protected:
		PURE_VIRTUAL(
			IZ_UINT GetPoseByIdx(
				S_SKL_JOINT_POSE& pose,
				IZ_UINT idx,
				IZ_FLOAT time));

	protected:
		class CPoseUpdater {
		public:
			CPoseUpdater(S_SKL_JOINT_POSE* pose);
			virtual ~CPoseUpdater() {}

		protected:
			CPoseUpdater() {}

		public:
			virtual IZ_BOOL BeginUpdate(IZ_UINT idx);
			virtual void EndUpdate(IZ_UINT idx, IZ_UINT updateFlag);
			virtual void Update(
				IZ_UINT idx,
				IZ_UINT transformType,
				IZ_UINT paramType,
				const SVector& param);

		private:
			S_SKL_JOINT_POSE* m_Pose;
		};

		class CSklPoseUpdater : public CPoseUpdater {
		public:
			CSklPoseUpdater(CSkeletonInstance* skl);
			virtual ~CSklPoseUpdater();

		public:
			IZ_BOOL BeginUpdate(IZ_UINT idx);
			void EndUpdate(IZ_UINT idx, IZ_UINT updateFlag);
			void Update(
				IZ_UINT idx,
				IZ_UINT transformType,
				IZ_UINT paramType,
				const SVector& param);

		private:
			CSkeletonInstance* m_Skl;
		};
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_ANIMATION_INTERFACE_H__)
