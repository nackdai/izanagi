#include "scenegraph/anm/AnimationInterp.h"
#include "scenegraph/skl/SkeletonInstance.h"
#include "scenegraph/anm/AnimationUtil.h"
#include "izIo.h"

using namespace izanagi;

// インスタンス作成
CAnimationInterp* CAnimationInterp::CreateAnimationInterp(IMemoryAllocator* allocator)
{
	IZ_ASSERT(allocator != IZ_NULL);

	// メモリサイズ計算
	size_t size = sizeof(CAnimationInterp);

	// メモリ確保
	void* buf = ALLOC_ZERO(allocator, size);
	VRETURN_NULL(buf != IZ_NULL);

	// インスタンス作成
	CAnimationInterp* instance = new(buf) CAnimationInterp;
	{
		instance->AddRef();

		instance->m_pAllocator = allocator;
		instance->m_InterpTime = 0.0f;

		instance->mStartKey.keyTime = 0;
		instance->mGoalKey.keyTime = 0;
	}

	return instance;
}

CAnimationInterp::CAnimationInterp()
{
	m_pAllocator = IZ_NULL;

	m_InterpTime = 0.0f;
	m_Type = E_INTERP_TYPE_FROZEN;

	memset(&mStartKey, 0, sizeof(mStartKey));
	mStartKey.numParams = 4;
	mStartKey.params = mStartKeyParamBuf;

	memset(&mGoalKey, 0, sizeof(mGoalKey));
	mGoalKey.numParams = 4;
	mGoalKey.params = mGoalKeyParamBuf;

	mKeys[0] = &mStartKey;
	mKeys[1] = &mGoalKey;
}

IZ_BOOL CAnimationInterp::SetAnimation(
	IZ_FLOAT interpTime,
	E_INTERP_TYPE type,
	IAnimation* startAnm,
	IZ_FLOAT targetStart,
	IAnimation* goalAnm,
	IZ_FLOAT targetGoal)
{
	IZ_ASSERT(startAnm != IZ_NULL);
	IZ_ASSERT(goalAnm != IZ_NULL);
	IZ_ASSERT(interpTime > 0.0f);
	IZ_ASSERT(type < E_INTERP_TYPE_NUM);

	VRETURN(targetStart <= startAnm->GetAnimationTime());
	VRETURN(targetGoal <= goalAnm->GetAnimationTime());

	m_InterpAnm[0].Set(startAnm, targetStart);
	m_InterpAnm[1].Set(goalAnm, targetGoal);

	m_InterpTime = interpTime;
	m_Type = type;

	mStartKey.keyTime = 0;
	mGoalKey.keyTime = interpTime;

	return IZ_TRUE;
}

IZ_UINT CAnimationInterp::ApplyAnimation(
	IZ_FLOAT time,
	IZ_UINT nJointIdx,
	CPoseUpdater& poseUpdater)
{
	time = IZ_MIN(time, m_InterpTime);

	IZ_UINT updateFlag = 0;

	// 姿勢情報更新開始
	VRETURN_VAL(poseUpdater.BeginUpdate(nJointIdx), 0);

	if ((time == 0) || (time >= m_InterpTime)) {
		// 開始 or 終了モーションのターゲット姿勢を取得する
		IZ_UINT idx = (time == 0 ? 0 : 1);

		S_SKL_JOINT_POSE pose;
		updateFlag = m_InterpAnm[idx].anm->GetPoseByIdx(
						pose,
						nJointIdx, 
						m_InterpAnm[idx].targetTime);

		// 姿勢更新
		UpdatePose(
			pose,
			nJointIdx,
			updateFlag,
			poseUpdater);
	}
	else {
		S_SKL_JOINT_POSE poseStart;
		S_SKL_JOINT_POSE poseGoal;

		IZ_UINT updateFlagStart = 0;
		IZ_UINT updateFlagGoal = 0;

		IZ_FLOAT addTime = (m_Type == E_INTERP_TYPE_FROZEN
							? 0.0f
							: time);

		// 開始モーション
		updateFlagStart = m_InterpAnm[0].anm->GetPoseByIdx(
							poseStart,
							nJointIdx, 
							m_InterpAnm[0].targetTime + addTime);

		CBit32Flag bitFlagStart(updateFlagStart);

		// 終了モーション
		updateFlagGoal = m_InterpAnm[1].anm->GetPoseByIdx(
							poseGoal,
							nJointIdx, 
							m_InterpAnm[1].targetTime + addTime);

		CBit32Flag bitFlagGoal(updateFlagGoal);

		S_SKL_JOINT_POSE finalPose;

		if (bitFlagStart.IsOn(E_SKL_JOINT_PARAM_SCALE) || bitFlagGoal.IsOn(E_SKL_JOINT_PARAM_SCALE)) {
			for (IZ_UINT i = 0; i < 3; i++) {
				mStartKey.params[i] = poseStart.scale[i];
				mGoalKey.params[i] = poseGoal.scale[i];

				// TODO
				// 線形補間
				finalPose.scale[i] = CAnimationUtil::ComputeInterp(E_ANM_INTERP_TYPE_LINEAR, time, 2, i, mKeys);
			}

			updateFlag |= E_SKL_JOINT_PARAM_SCALE;
		}
		if (bitFlagStart.IsOn(E_SKL_JOINT_PARAM_TRANSLATE) || bitFlagGoal.IsOn(E_SKL_JOINT_PARAM_TRANSLATE)) {
			for (IZ_UINT i = 0; i < 3; i++) {
				mStartKey.params[i] = poseStart.trans[i];
				mGoalKey.params[i] = poseGoal.trans[i];

				// TODO
				// 線形補間
				finalPose.trans[i] = CAnimationUtil::ComputeInterp(E_ANM_INTERP_TYPE_LINEAR, time, 2, i, mKeys);
			}

			updateFlag |= E_SKL_JOINT_PARAM_TRANSLATE;
		}
		if (bitFlagStart.IsOn(E_SKL_JOINT_PARAM_QUARTANION) || bitFlagGoal.IsOn(E_SKL_JOINT_PARAM_QUARTANION)) {
			for (IZ_UINT i = 0; i < 4; i++) {
				mStartKey.params[i] = poseStart.quat.v[i];
				mGoalKey.params[i] = poseGoal.quat.v[i];
			}

			// 球面線形補間のみ
			CAnimationUtil::ComputeInterp(
				finalPose.quat,
				E_ANM_INTERP_TYPE_SLERP,
				time,
				2,
				0,
				mKeys);

			updateFlag |= E_SKL_JOINT_PARAM_QUARTANION;
		}

		// 姿勢更新
		UpdatePose(
			finalPose,
			nJointIdx,
			updateFlag,
			poseUpdater);
	}

	// 姿勢情報更新終了
	poseUpdater.EndUpdate(
		nJointIdx,
		updateFlag);

	return updateFlag;
}

void CAnimationInterp::UpdatePose(
	const S_SKL_JOINT_POSE& pose,
	IZ_UINT nJointIdx,
	IZ_UINT updateFlag,
	CPoseUpdater& poseUpdater)
{
	SVector param;

	CBit32Flag bitFlag(updateFlag);

	// 姿勢更新
	if (bitFlag.IsOn(E_SKL_JOINT_PARAM_SCALE)) {
		SVector::SetXYZ(
			param,
			pose.scale[0], pose.scale[1], pose.scale[2]);

		poseUpdater.Update(
			nJointIdx,
			E_ANM_TRANSFORM_TYPE_SCALE,
			E_ANM_TRANSFORM_TYPE_XYZ,
			param);
	}
	if (bitFlag.IsOn(E_SKL_JOINT_PARAM_TRANSLATE)) {
		SVector::SetXYZ(
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

// アニメーション適用
void CAnimationInterp::ApplyAnimation(
	CSkeletonInstance* skl,
	IZ_FLOAT time)
{
	IZ_ASSERT(skl != IZ_NULL);

	IZ_UINT jointNum = skl->GetJointNum();

	for (IZ_UINT i = 0; i < jointNum; ++i) {
		ApplyAnimationByIdx(skl, i, time);
	}
}

void CAnimationInterp::ApplyAnimationByIdx(
	CSkeletonInstance* skl,
	IZ_UINT jointIdx,
	IZ_FLOAT time)
{
	IZ_ASSERT(skl != IZ_NULL);
	IZ_ASSERT(jointIdx < skl->GetJointNum());

	ApplyAnimation(
		time,
		jointIdx,
		CSklPoseUpdater(skl));
}

IZ_UINT CAnimationInterp::GetPoseByIdx(
	S_SKL_JOINT_POSE& pose,
	IZ_UINT idx,
	IZ_FLOAT time)
{
	IZ_UINT updateFlag = 0;

	updateFlag = ApplyAnimation(
					time,
					idx,
					CPoseUpdater(&pose));

	return updateFlag;
}
