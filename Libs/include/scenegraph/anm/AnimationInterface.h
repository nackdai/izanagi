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
		PURE_VIRTUAL(
			void ApplyAnimation(
				CSkeletonInstance* skl,
				IZ_FLOAT time));

		PURE_VIRTUAL(
			void ApplyAnimationByIdx(
				CSkeletonInstance* skl,
				IZ_UINT jointIdx,
				IZ_FLOAT time));

		PURE_VIRTUAL(IZ_FLOAT GetAnimationTime() const);

		PURE_VIRTUAL(
			IZ_UINT GetPoseByIdx(
				S_SKL_JOINT_POSE& pose,
				IZ_UINT idx,
				IZ_FLOAT time));

	protected:
		
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_ANIMATION_INTERFACE_H__)
