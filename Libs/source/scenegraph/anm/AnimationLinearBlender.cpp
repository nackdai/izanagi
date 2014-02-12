#include "scenegraph/anm/AnimationLinearBlender.h"
#include "scenegraph/anm/AnimationInterface.h"
#include "scenegraph/anm/AnimationUtil.h"
#include "scenegraph/skl/SkeletonInstance.h"
#include "scenegraph/skl/SKLFormat.h"
#include "scenegraph/anm/PoseUpdater.h"

using namespace izanagi;

CAnmLinearBlender::CAnmLinearBlender()
{
    m_StartWeight = 0.0f;
    m_GoalWeight = 1.0f;

    m_MinTime = 0.0f;
    m_MaxTime = 0.0f; 
}

CAnmLinearBlender::~CAnmLinearBlender()
{
}

IZ_BOOL CAnmLinearBlender::Init()
{
    // do nothing...
    return IZ_TRUE;
}

namespace
{
    inline void _SetDefaulsePoseParameter(S_SKL_JOINT_POSE& pose)
    {
        pose.trans[0] = pose.trans[1] = pose.trans[2] = 0.0f;
        math::SQuat::SetDefaultZero(pose.quat);
        pose.scale[0] = pose.scale[1] = pose.scale[2] = 1.0f;
    }
}

IZ_UINT CAnmLinearBlender::ApplyAnimation(
    IZ_FLOAT time,
    IZ_UINT nJointIdx,
    CPoseUpdater& poseUpdater)
{
    IZ_UINT updateFlag = 0;

    if (IsCachedAnimation(time, updateFlag))
    {
        return updateFlag;
    }

    // 姿勢情報更新開始
    VRETURN_VAL(poseUpdater.BeginUpdate(nJointIdx), 0);
    
    S_SKL_JOINT_POSE poseStart;
    S_SKL_JOINT_POSE poseGoal;

    _SetDefaulsePoseParameter(poseStart);
    _SetDefaulsePoseParameter(poseGoal);

    IZ_UINT updateFlagStart = 0;
    IZ_UINT updateFlagGoal = 0;

    // 開始モーション
    updateFlagStart = m_Anms[0].anm->GetPoseByIdx(
                        poseStart,
                        nJointIdx, 
                        time);

    CBit32Flag bitFlagStart(updateFlagStart);

    // 終了モーション
    updateFlagGoal = m_Anms[1].anm->GetPoseByIdx(
                        poseGoal,
                        nJointIdx, 
                        time);

    CBit32Flag bitFlagGoal(updateFlagGoal);

    S_SKL_JOINT_POSE finalPose;

    if (bitFlagStart.IsOn(E_SKL_JOINT_PARAM_SCALE) || bitFlagGoal.IsOn(E_SKL_JOINT_PARAM_SCALE)) {
        for (IZ_UINT i = 0; i < 3; i++) {
            // 線形補間
            finalPose.scale[i] = poseStart.scale[i] * m_StartWeight + poseGoal.scale[i] * m_GoalWeight;
        }

        updateFlag |= E_SKL_JOINT_PARAM_SCALE;
    }
    if (bitFlagStart.IsOn(E_SKL_JOINT_PARAM_TRANSLATE) || bitFlagGoal.IsOn(E_SKL_JOINT_PARAM_TRANSLATE)) {
        for (IZ_UINT i = 0; i < 3; i++) {
            // 線形補間
            finalPose.trans[i] = poseStart.trans[i] * m_StartWeight + poseGoal.trans[i] * m_GoalWeight;
        }

        updateFlag |= E_SKL_JOINT_PARAM_TRANSLATE;
    }
    if (bitFlagStart.IsOn(E_SKL_JOINT_PARAM_QUARTANION) || bitFlagGoal.IsOn(E_SKL_JOINT_PARAM_QUARTANION)) {
        // 球面線形補間
        math::SQuat::Slerp(
            finalPose.quat,
            poseStart.quat,
            poseGoal.quat,
            m_GoalWeight);

        updateFlag |= E_SKL_JOINT_PARAM_QUARTANION;
    }

    // 姿勢更新
    UpdatePose(
        finalPose,
        nJointIdx,
        updateFlag,
        poseUpdater);

    // 姿勢情報更新終了
    poseUpdater.EndUpdate(
        nJointIdx,
        updateFlag);

    SetUpdatedFlag(updateFlag);

    return updateFlag;
}

void CAnmLinearBlender::UpdatePose(
    const S_SKL_JOINT_POSE& pose,
    IZ_UINT nJointIdx,
    IZ_UINT updateFlag,
    CPoseUpdater& poseUpdater)
{
    math::SVector param;

    CBit32Flag bitFlag(updateFlag);

    // 姿勢更新
    if (bitFlag.IsOn(E_SKL_JOINT_PARAM_SCALE)) {
        math::SVector::SetXYZ(
            param,
            pose.scale[0], pose.scale[1], pose.scale[2]);

        poseUpdater.Update(
            nJointIdx,
            E_ANM_TRANSFORM_TYPE_SCALE,
            E_ANM_TRANSFORM_TYPE_XYZ,
            param);
    }
    if (bitFlag.IsOn(E_SKL_JOINT_PARAM_TRANSLATE)) {
        math::SVector::SetXYZ(
            param,
            pose.trans[0], pose.trans[1], pose.trans[2]);

        poseUpdater.Update(
            nJointIdx,
            E_ANM_TRANSFORM_TYPE_TRANSLATE,
            E_ANM_TRANSFORM_TYPE_XYZ,
            param);
    }
    if (bitFlag.IsOn(E_SKL_JOINT_PARAM_QUARTANION)) {
        poseUpdater.Update(
            nJointIdx,
            E_ANM_TRANSFORM_TYPE_QUATERNION,
            E_ANM_TRANSFORM_TYPE_XYZW,
            pose.quat);
    }
}

IZ_BOOL CAnmLinearBlender::SetAnimation(
    IZ_FLOAT weight,
    IAnimation* startAnm,
    IAnimation* goalAnm)
{
    IZ_ASSERT(startAnm != IZ_NULL);
    IZ_ASSERT(goalAnm != IZ_NULL);
    IZ_ASSERT(weight > 0.0f);


    m_Anms[0].Set(startAnm);
    m_Anms[1].Set(goalAnm);

    m_StartWeight = 1.0f - math::CMath::Clamp(weight, 0.0f, 1.0f);
    m_GoalWeight = 1.0f - m_StartWeight;

    m_MinTime = IZ_MIN(
                    m_Anms[0].anm->GetAnimationTime(),
                    m_Anms[1].anm->GetAnimationTime());
    m_MaxTime = IZ_MAX(
                    m_Anms[0].anm->GetAnimationTime(),
                    m_Anms[1].anm->GetAnimationTime());

    return IZ_TRUE;
}

void CAnmLinearBlender::ApplyAnimation(
    CSkeletonInstance* skl,
    IZ_FLOAT time)
{
    IZ_ASSERT(skl != IZ_NULL);

    IZ_UINT jointNum = skl->GetJointNum();

    for (IZ_UINT i = 0; i < jointNum; i++) {
        ApplyAnimationByIdx(skl, i, time);
    }
}

void CAnmLinearBlender::ApplyAnimationByIdx(
    CSkeletonInstance* skl,
    IZ_UINT jointIdx,
    IZ_FLOAT time)
{
    IZ_ASSERT(skl != IZ_NULL);
    IZ_ASSERT(time <= GetAnimationTime());

    CSklPoseUpdater sklPoseUpdater(skl);

    ApplyAnimation(
        time,
        jointIdx,
        *(CPoseUpdater*)&sklPoseUpdater);
}

IZ_UINT CAnmLinearBlender::GetPoseByIdx(
    S_SKL_JOINT_POSE& pose,
    IZ_UINT idx,
    IZ_FLOAT time)
{
    IZ_UINT updateFlag = 0;

    CPoseUpdater poseUpdater(&pose);

    updateFlag = ApplyAnimation(
                    time,
                    idx,
                    poseUpdater);

    return updateFlag;
}
