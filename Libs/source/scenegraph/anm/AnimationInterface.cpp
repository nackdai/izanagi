#include "scenegraph/anm/AnimationInterface.h"
#include "scenegraph/skl/SkeletonInstance.h"
#include "scenegraph/skl/SkeletonUtil.h"

using namespace izanagi;

////////////////////////////////////////////////////

IAnimation::CPoseUpdater::CPoseUpdater(S_SKL_JOINT_POSE* pose)
{
	m_Pose = pose;
}

IZ_BOOL IAnimation::CPoseUpdater::BeginUpdate(IZ_UINT idx)
{
	// Nothing is done.
	UNUSED_ALWAYS(idx);
	return true;
}

void IAnimation::CPoseUpdater::EndUpdate(IZ_UINT idx, IZ_UINT updateFlag)
{
	// Nothing is done.
	UNUSED_ALWAYS(idx);
	UNUSED_ALWAYS(updateFlag);
}

void IAnimation::CPoseUpdater::Update(
	IZ_UINT idx,
	IZ_UINT transformType,
	IZ_UINT paramType,
	const SVector& param)
{
	UNUSED_ALWAYS(idx);

	IZ_ASSERT(m_Pose != IZ_NULL);

	CSkeletonUtil::UpdatePose(
		*m_Pose,
		transformType,
		paramType,
		param);
}

////////////////////////////////////////////////////

IAnimation::CSklPoseUpdater::CSklPoseUpdater(CSkeletonInstance* skl)
{
	m_Skl = IZ_NULL;
	SAFE_REPLACE(m_Skl, skl);
}

IAnimation::CSklPoseUpdater::~CSklPoseUpdater()
{
	SAFE_RELEASE(m_Skl);
}

IZ_BOOL IAnimation::CSklPoseUpdater::BeginUpdate(IZ_UINT idx)
{
	IZ_ASSERT(m_Skl != IZ_NULL);
	return m_Skl->BeginUpdatePose(idx);
}

void IAnimation::CSklPoseUpdater::EndUpdate(IZ_UINT idx, IZ_UINT updateFlag)
{
	IZ_ASSERT(m_Skl != IZ_NULL);
	m_Skl->EndUpdatePose(idx, updateFlag);
}

void IAnimation::CSklPoseUpdater::Update(
	IZ_UINT idx,
	IZ_UINT transformType,
	IZ_UINT paramType,
	const SVector& param)
{
	IZ_ASSERT(m_Skl != IZ_NULL);
	m_Skl->UpdatePose(
		idx,
		transformType,
		paramType,
		param);
}
