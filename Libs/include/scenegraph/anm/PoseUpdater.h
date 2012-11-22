#if !defined(__IZANAGI_SCENEGRAPH_POSE_UPDAER_H__)
#define __IZANAGI_SCENEGRAPH_POSE_UPDAER_H__

#include "ANMFormat.h"

namespace izanagi {
	class CSkeletonInstance;
	struct S_SKL_JOINT_POSE;

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
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_POSE_UPDAER_H__)
