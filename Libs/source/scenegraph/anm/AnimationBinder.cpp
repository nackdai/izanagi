#include "scenegraph/anm/AnimationBinder.h"
#include "scenegraph/skl/SkeletonInstance.h"
#include "scenegraph/skl/izSkeleton.h"

using namespace izanagi;

// インスタンス作成
CAnimationBinder* CAnimationBinder::CreateAnimationBinder(
	IMemoryAllocator* allocator,
	CSkeleton* skl)
{
	IZ_ASSERT(allocator != IZ_NULL);

	// メモリサイズ計算
	size_t size = sizeof(CAnimationBinder);

	IZ_UINT jointNum = skl->GetJointNum();
	size += sizeof(SJointInfo) * jointNum;

	// メモリ確保
	IZ_UINT8* buf = reinterpret_cast<IZ_UINT8*>(ALLOC_ZERO(allocator, size));
	VRETURN_NULL(buf != IZ_NULL);

	// インスタンス作成
	CAnimationBinder* instance = new(buf) CAnimationBinder;
	{
		buf += sizeof(SJointInfo) * jointNum;

		instance->AddRef();

		instance->m_pAllocator = allocator;
		instance->m_JointNum = jointNum;
		instance->m_Joints = reinterpret_cast<SJointInfo*>(buf);

		SAFE_REPLACE(
			instance->m_Skl,
			skl);

		for (IZ_UINT i = 0; i < jointNum; i++) {
			const S_SKL_JOINT* joint = (static_cast<const CSkeleton*>(skl))->GetJoint(i);
			SJointInfo& p = instance->m_Joints[i];

			p.Init(
				i,
				joint->name.GetString());

			instance->m_JointHash.Add(&p.hashItem);
		}
	}

	return instance;
}

CAnimationBinder::CAnimationBinder()
{
	m_pAllocator = IZ_NULL;

	m_JointNum = 0;
	m_Joints = IZ_NULL;

	m_MinTime = IZ_FLOAT_MAX;
	m_MaxTime = 0.0f;
}

CAnimationBinder::~CAnimationBinder()
{
	for (IZ_UINT i = 0; i < m_JointNum; i++) {
		SJointInfo& joint = m_Joints[i];
		SAFE_RELEASE(joint.anm);
	}

	m_JointHash.Clear();

	SAFE_RELEASE(m_Skl);
}

IZ_BOOL CAnimationBinder::BindByIdx(
	IZ_UINT idx,
	IAnimation* anm)
{
	VRETURN(idx < m_JointNum);

	SJointInfo& joint = m_Joints[idx];
	SAFE_REPLACE(joint.anm, anm);

	m_MinTime = IZ_MIN(m_MinTime, anm->GetAnimationTime());
	m_MaxTime = IZ_MAX(m_MaxTime, anm->GetAnimationTime());

	return IZ_TRUE;
}

IZ_BOOL CAnimationBinder::BindByName(
	IZ_PCSTR name,
	IAnimation* anm)
{
	IZ_UINT key = CKey::GenerateKey(name);

	IZ_BOOL ret = BindByKey(key, anm);
	return ret;
}

IZ_BOOL CAnimationBinder::BindByKey(
	IZ_UINT key,
	IAnimation* anm)
{
	SJointInfo* joint = m_JointHash.FindData(key);
	VRETURN(joint != IZ_NULL);

	SAFE_REPLACE(joint->anm, anm);

	m_MinTime = IZ_MIN(m_MinTime, anm->GetAnimationTime());
	m_MaxTime = IZ_MAX(m_MaxTime, anm->GetAnimationTime());

	return IZ_TRUE;
}

void CAnimationBinder::ApplyAnimation(
	CSkeletonInstance* skl,
	IZ_FLOAT time)
{
	IZ_ASSERT(skl != IZ_NULL);
	
	for (IZ_UINT i = 0; i < m_JointNum; i++) {
		ApplyAnimationByIdx(skl, i, time);
	}
}

void CAnimationBinder::ApplyAnimationByIdx(
	CSkeletonInstance* skl,
	IZ_UINT jointIdx,
	IZ_FLOAT time)
{
	IZ_ASSERT(skl != IZ_NULL);
	IZ_ASSERT(jointIdx < m_JointNum);

	const SJointInfo& joint = m_Joints[jointIdx];
	IZ_ASSERT(joint.anm != IZ_NULL);

	if (joint.anm != IZ_NULL) {
		skl->ApplyAnimationByIdx(
			jointIdx,
			time,
			joint.anm);
	}
}

IZ_UINT CAnimationBinder::GetPoseByIdx(
	S_SKL_JOINT_POSE& pose,
	IZ_UINT idx,
	IZ_FLOAT time)
{
	IZ_ASSERT(idx < m_JointNum);

	const SJointInfo& jointInfo = m_Joints[idx];

	IZ_UINT updateFlag = 0;

	if (jointInfo.anm != IZ_NULL) {
		updateFlag = jointInfo.anm->GetPoseByIdx(pose, idx, time);
	}
	else {
		const S_SKL_JOINT* joint = (static_cast<const CSkeleton*>(m_Skl))->GetJoint(idx);
		memcpy(&pose, &joint->pose, sizeof(pose));

		updateFlag = joint->validParam;
	}

	return updateFlag;
}
