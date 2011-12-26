#if !defined(__IZANAGI_SCENEGRAPH_ANIMATION_LINEAR_BLENDER_H__)
#define __IZANAGI_SCENEGRAPH_ANIMATION_LINEAR_BLENDER_H__

#include "AnimationBlender.h"

namespace izanagi {
	class IAnimation;
	class CPoseUpdater;
	struct S_SKL_JOINT_POSE;

	/** ２モーションの線形ブレンディング
	 */
	class CAnmLinearBlender : public IAnimationBlender {
		friend class IAnimationBlender;

	protected:
		CAnmLinearBlender();
		virtual ~CAnmLinearBlender();

	protected:
		IZ_BOOL Init();

		IZ_UINT ApplyAnimation(
			IZ_FLOAT time,
			IZ_UINT nJointIdx,
			CPoseUpdater& poseUpdater);

		void UpdatePose(
			const S_SKL_JOINT_POSE& pose,
			IZ_UINT nJointIdx,
			IZ_UINT updateFlag,
			CPoseUpdater& poseUpdater);

	public:
		IZ_BOOL SetAnimation(
			IZ_FLOAT weight,
			IAnimation* startAnm,
			IAnimation* goalAnm);

		void ApplyAnimation(
			CSkeletonInstance* skl,
			IZ_FLOAT time);

		void ApplyAnimationByIdx(
			CSkeletonInstance* skl,
			IZ_UINT jointIdx,
			IZ_FLOAT time);

		IZ_FLOAT GetAnimationTime() const { return GetMinAnimationTime(); }
		
		IZ_UINT GetPoseByIdx(
			S_SKL_JOINT_POSE& pose,
			IZ_UINT idx,
			IZ_FLOAT time);

		IZ_FLOAT GetMinAnimationTime() const { return m_MinTime; }
		IZ_FLOAT GetMaxAnimationTime() const { return m_MaxTime; }

	protected:
		// ブレンドアニメ情報
		struct SBlendAnm {
			IAnimation* anm;		// 対象アニメ

			SBlendAnm()
			{
				anm = IZ_NULL;
			}

			~SBlendAnm()
			{
				SAFE_RELEASE(anm);
			}

			void Set(IAnimation* targetAnm)
			{
				SAFE_REPLACE(anm, targetAnm);
			}

			IZ_BOOL IsValid() const
			{
				return (anm != IZ_NULL);
			}
		};

	protected:
		// 何度も使うのであらかじめ持っておく
		IZ_FLOAT m_StartWeight;
		IZ_FLOAT m_GoalWeight;

		IZ_FLOAT m_MinTime;
		IZ_FLOAT m_MaxTime;

		SBlendAnm m_Anms[2];
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_ANIMATION_LINEAR_BLENDER_H__)
