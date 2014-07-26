#include "scenegraph/skl/SkeletonInstance.h"
#include "scenegraph/skl/izSkeleton.h"
#include "scenegraph/skl/SkeletonUtil.h"
#include "izSystem.h"

#include "scenegraph/anm/AnimationInterface.h"

using namespace izanagi;

CSkeletonInstance* CSkeletonInstance::CreateSkeletonInstance(
    IMemoryAllocator* pAllocator,
    CSkeleton* pSkl)
{
    // Compute buffer size.
    IZ_UINT nSize = ComputeBufferSize(pSkl);

    // Allocate buffer.
    IZ_UINT8* pBuf = reinterpret_cast<IZ_UINT8*>(ALLOC_ZERO(pAllocator, nSize));
    VRETURN_NULL(pBuf != IZ_NULL);

    IZ_UINT8* pTmpBuf = pBuf;

    CSkeletonInstance* pInstance = new(pBuf) CSkeletonInstance;
    {
        pBuf += sizeof(CSkeletonInstance);

        pInstance->AddRef();
        pInstance->m_Allocator = pAllocator;

        SAFE_REPLACE(pInstance->m_pBody, pSkl);

        pInstance->m_nJointNum = pSkl->GetJointNum();

        pBuf = pInstance->SetJointData(pBuf);
    }

    IZ_ASSERT(CStdUtil::GetPtrDistance(pBuf, pTmpBuf) == nSize);

    return pInstance;
}

IZ_UINT CSkeletonInstance::ComputeBufferSize(const CSkeleton* pSkl)
{
    IZ_UINT size = sizeof(CSkeletonInstance);
    {
        IZ_UINT jointNum = pSkl->GetJointNum();

        size += sizeof(S_SKL_JOINT_POSE) * jointNum;
        size += sizeof(math::SMatrix) * jointNum;
        size += sizeof(IZ_UINT8) * jointNum;
    }

    return size; 
}

CSkeletonInstance::CSkeletonInstance()
{
    m_Allocator = IZ_NULL;

    m_pBody = IZ_NULL;

    m_nJointNum = 0;

    m_pJointPose = IZ_NULL;
    m_pGlobalPose = IZ_NULL;

    m_IsUpdatingPose = IZ_FALSE;
}

CSkeletonInstance::~CSkeletonInstance()
{
    SAFE_RELEASE(m_pBody);
}

void CSkeletonInstance::BuildMatrix(const math::SMatrix* mtxL2W)
{
    for (IZ_UINT i = 0; i < m_nJointNum; ++i) {
        BuildLocalMatrix(i);
    }

    // Apply parent' matrix.
    for (IZ_UINT i = 0; i < m_nJointNum; ++i) {
        IZ_INT nParentIdx = m_pBody->GetJoint(i)->parent;

        if (nParentIdx >= 0) {
            IZ_ASSERT((IZ_UINT)nParentIdx < i);
            ApplyParentMatrix(i, nParentIdx);
        }
        else if (mtxL2W != IZ_NULL) {
            // ルートに対しては、L2Wマトリクスを計算する
            math::SMatrix& mtxJoint = m_pGlobalPose[0];

            math::SMatrix::Mul(
                mtxJoint,
                mtxJoint,
                *mtxL2W);
        }
    }

    for (IZ_UINT i = 0; i < m_nJointNum; ++i) {
        ApplyInvBindMatrix(i);
    }
}

const math::SMatrix* CSkeletonInstance::GetJointMtx(IZ_INT idx) const
{
    if (idx < 0) {
        return IZ_NULL;
    }
    IZ_ASSERT((IZ_UINT)idx < m_nJointNum);
    return &m_pGlobalPose[idx];
}

IZ_UINT8* CSkeletonInstance::SetJointData(IZ_UINT8* pBuf)
{
    // 姿勢情報
    m_pJointPose = reinterpret_cast<S_SKL_JOINT_POSE*>(pBuf);
    pBuf += sizeof(S_SKL_JOINT_POSE) * m_nJointNum;

    // 最終的な行列
    m_pGlobalPose = reinterpret_cast<math::SMatrix*>(pBuf);
    pBuf += sizeof(math::SMatrix) * m_nJointNum;

    // アニメーションパラメータの中で更新が必要なパラメータフラグ
    m_ValidAnmParam = reinterpret_cast<IZ_UINT8*>(pBuf);
    pBuf += sizeof(IZ_UINT8) * m_nJointNum;

    // 元データからコピー
    for (IZ_UINT i = 0; i < m_nJointNum; ++i) {
        const S_SKL_JOINT* joint = m_pBody->GetJoint(i);

        // 姿勢情報
        memcpy(
            &m_pJointPose[i],
            &joint->pose,
            sizeof(S_SKL_JOINT_POSE));

        // アニメーションパラメータの中で更新が必要なパラメータフラグ
        m_ValidAnmParam[i] = joint->validAnmParam;
    }

    return pBuf;
}

namespace {
#if 0
    void _RotateAxis(
        math::SMatrix& dst,
        const math::SMatrix& src,
        IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z, IZ_FLOAT angle)
    {
        math::SQuat quat;
        SetQuatFromRadAxis(quat, angle, x, y, z);

        math::SMatrix mtx;
        MatrixFromQuat(mtx, quat);

        math::SMatrix::Mul(dst, src, mtx);
    }
#else
    void _RotateAxis(
        math::SMatrix& dst,
        const math::SMatrix& src,
        IZ_FLOAT x, IZ_FLOAT y, IZ_FLOAT z, IZ_FLOAT w)
    {
        math::SQuat quat;
        math::SQuat::Set(quat, x, y, z, w);

        math::SMatrix mtx;
        math::SQuat::MatrixFromQuat(mtx, quat);

        math::SMatrix::Mul(dst, src, mtx);
    }
#endif
}   // namespace

void CSkeletonInstance::BuildLocalMatrix(IZ_UINT nIdx)
{
    IZ_ASSERT(m_pBody != IZ_NULL);
    const S_SKL_JOINT* pJoint = m_pBody->GetJoint(nIdx);

    // NOTE
    // izanagiは右掛け

    // 計算するパラメータを判定するフラグ
    CBit32Flag flag(
        pJoint->validParam
        | m_ValidAnmParam[nIdx]);

    // 念のため次に向けてリセットしておく
    m_ValidAnmParam[nIdx] = 0;

    const S_SKL_JOINT_POSE& pose = m_pJointPose[nIdx];
    math::SMatrix& mtxJoint = m_pGlobalPose[nIdx];

    math::SMatrix::SetUnit(mtxJoint);

    if (flag.IsOn(E_SKL_JOINT_PARAM_SCALE)) {
        math::SMatrix::Scale(
            mtxJoint,
            mtxJoint, 
            pose.scale[0],
            pose.scale[1],
            pose.scale[2]);
    }

    if (flag.IsOn(E_SKL_JOINT_PARAM_QUARTANION)) {
        _RotateAxis(
            mtxJoint,
            mtxJoint,
            pose.quat.x,
            pose.quat.y,
            pose.quat.z,
            pose.quat.w);
    }

    if (flag.IsOn(E_SKL_JOINT_PARAM_TRANSLATE)) {
        math::SMatrix::Trans(
            mtxJoint,
            mtxJoint, 
            pose.trans[0],
            pose.trans[1],
            pose.trans[2]);
    }

    //IZ_PRINTF("[%d]-----\n", nIdx);
    //math::SMatrix::Dump(mtxJoint);
}

void CSkeletonInstance::ApplyInvBindMatrix(IZ_UINT nIdx)
{
    IZ_ASSERT(m_pBody != IZ_NULL);
    const S_SKL_JOINT* pJoint = m_pBody->GetJoint(nIdx);

    math::SMatrix& mtxJoint = m_pGlobalPose[nIdx];

    math::SMatrix::Mul(
        mtxJoint,
        pJoint->mtxInvBind,
        mtxJoint);
}

void CSkeletonInstance::ApplyParentMatrix(
    IZ_UINT nIdx,
    IZ_UINT nParentIdx)
{
    math::SMatrix& mtxJoint = m_pGlobalPose[nIdx];
    const math::SMatrix& mtxParent = m_pGlobalPose[nParentIdx];

    math::SMatrix::Mul(
        mtxJoint,
        mtxJoint,
        mtxParent);
}

void CSkeletonInstance::ApplyAnimation(
    IZ_FLOAT fTime,
    IAnimation* pAnm)
{
    IZ_ASSERT(pAnm != IZ_NULL);
    pAnm->ApplyAnimation(this, fTime);
}

void CSkeletonInstance::ApplyAnimationByIdx(
    IZ_UINT nJointIdx,
    IZ_FLOAT fTime,
    IAnimation* pAnm)
{
    IZ_ASSERT(pAnm != IZ_NULL);
    pAnm->ApplyAnimationByIdx(this, nJointIdx, fTime);
}

// 姿勢情報更新開始
IZ_BOOL CSkeletonInstance::BeginUpdatePose(IZ_UINT idx)
{
    VRETURN(!m_IsUpdatingPose);
    VRETURN(idx < m_nJointNum);

    m_IsUpdatingPose = IZ_TRUE;
    return IZ_TRUE;
}

// 姿勢情報更新終了
void CSkeletonInstance::EndUpdatePose(
    IZ_UINT idx,
    IZ_UINT8 updateFlag)
{
    IZ_ASSERT(idx < m_nJointNum);
    IZ_ASSERT(m_IsUpdatingPose);

    const S_SKL_JOINT* pJoint = m_pBody->GetJoint(idx);
    m_ValidAnmParam[idx] = updateFlag;

    m_IsUpdatingPose = IZ_FALSE;
}

// 姿勢情報更新
void CSkeletonInstance::UpdatePose(
    IZ_UINT idx,
    IZ_UINT transformType,
    IZ_UINT paramType,
    const math::SVector4& param)
{
    IZ_ASSERT(m_IsUpdatingPose);

    S_SKL_JOINT_POSE& sPose = m_pJointPose[idx];

    CSkeletonUtil::UpdatePose(
        sPose,
        transformType,
        paramType,
        param);
}

// ジョイントキーからジョイントインデックスを逆引きする
IZ_INT CSkeletonInstance::GetJointIdxByKey(IZ_UINT key)
{
    IZ_ASSERT(m_pBody != IZ_NULL);

    IZ_UINT jointNum = m_pBody->GetJointNum();

    for (IZ_UINT i = 0; i < jointNum; i++) {
        const S_SKL_JOINT* joint = m_pBody->GetJoint(i);
        if (joint->nameKey == key) {
            return i;
        }
    }

    IZ_ASSERT(IZ_FALSE);
    return -1;
}
