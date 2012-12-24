#include "scenegraph/anm/PoseUpdater.h"
#include "scenegraph/skl/SkeletonInstance.h"
#include "scenegraph/skl/SkeletonUtil.h"

using namespace izanagi;

CPoseUpdater::CPoseUpdater(S_SKL_JOINT_POSE* pose)
{
    m_Pose = pose;
}

IZ_BOOL CPoseUpdater::BeginUpdate(IZ_UINT idx)
{
    // Nothing is done.
    UNUSED_ALWAYS(idx);
    return true;
}

void CPoseUpdater::EndUpdate(IZ_UINT idx, IZ_UINT updateFlag)
{
    // Nothing is done.
    UNUSED_ALWAYS(idx);
    UNUSED_ALWAYS(updateFlag);
}

void CPoseUpdater::Update(
    IZ_UINT idx,
    IZ_UINT transformType,
    IZ_UINT paramType,
    const math::SVector& param)
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

CSklPoseUpdater::CSklPoseUpdater(CSkeletonInstance* skl)
{
    m_Skl = IZ_NULL;
    SAFE_REPLACE(m_Skl, skl);
}

CSklPoseUpdater::~CSklPoseUpdater()
{
    SAFE_RELEASE(m_Skl);
}

IZ_BOOL CSklPoseUpdater::BeginUpdate(IZ_UINT idx)
{
    IZ_ASSERT(m_Skl != IZ_NULL);
    return m_Skl->BeginUpdatePose(idx);
}

void CSklPoseUpdater::EndUpdate(IZ_UINT idx, IZ_UINT updateFlag)
{
    IZ_ASSERT(m_Skl != IZ_NULL);
    m_Skl->EndUpdatePose(idx, updateFlag);
}

void CSklPoseUpdater::Update(
    IZ_UINT idx,
    IZ_UINT transformType,
    IZ_UINT paramType,
    const math::SVector& param)
{
    IZ_ASSERT(m_Skl != IZ_NULL);
    m_Skl->UpdatePose(
        idx,
        transformType,
        paramType,
        param);
}
