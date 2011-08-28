#if !defined(__IZANAGI_SCENEGRAPH_ANIMATION_INTERFACE_H__)
#define __IZANAGI_SCENEGRAPH_ANIMATION_INTERFACE_H__

#include "izStd.h"
#include "ANMFormat.h"

namespace izanagi {
	class CSkeletonInstance;

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

	public:
		PURE_VIRTUAL(const S_ANM_NODE* GetAnmNodeByIdx(IZ_UINT idx));
		PURE_VIRTUAL(const S_ANM_NODE* GetAnmNodeByName(IZ_PCSTR name));
		PURE_VIRTUAL(const S_ANM_NODE* GetAnmNodeByKey(IZ_UINT key));
		PURE_VIRTUAL(const S_ANM_NODE* GetAnmNodeByJointIdx(IZ_UINT nJointIdx));

	public:
		PURE_VIRTUAL(IZ_UINT GetNodelNum() const);
		PURE_VIRTUAL(IZ_FLOAT GetAnimationTime() const);

		PURE_VIRTUAL(E_ANM_KEY_TYPE GetKeyType() const);
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_ANIMATION_INTERFACE_H__)
