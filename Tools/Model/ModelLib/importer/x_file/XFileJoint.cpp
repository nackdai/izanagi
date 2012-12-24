#include "XFileJoint.h"
#include "GeometryCommon.h"

#include <algorithm>

CXFileJoint* CXFileJoint::s_pInstance = IZ_NULL;

CXFileJoint& CXFileJoint::GetInstance()
{
    if (s_pInstance == IZ_NULL) {
        s_pInstance = new CXFileJoint();
    }
    return *s_pInstance;
}

void CXFileJoint::DeleteInstance()
{
    SAFE_DELETE(s_pInstance);
}

CXFileJoint::CXFileJoint()
{
}

CXFileJoint::~CXFileJoint()
{
}

void CXFileJoint::Clear()
{
    m_JointList.clear();
}

namespace {
    inline const SXFileBone* _GetBoneByName(
        const std::vector<SXFileBone>& tvBoneList,
        const std::string& name)
    {
        for (size_t i = 0; i < tvBoneList.size(); i++) {
            const SXFileBone& sBone = tvBoneList[i];
            if (sBone.name.compare(name) == 0) {
                return &sBone;
            }
        }

        return IZ_NULL;
    }
}   // namespace

IZ_BOOL CXFileJoint::Init(CXFileParser* pParser)
{
    const std::vector<SXFileNode*>& tvNodeList = pParser->GetNodeList();

    m_JointList.reserve(tvNodeList.size());

    for (size_t i = 0; i < tvNodeList.size(); i++) {
        const SXFileNode* pNode = tvNodeList[i];

        m_JointList.push_back(SJoint());
        SJoint& sJoint = m_JointList.back();

        sJoint.name = pNode->name;

        // Break down matrix to trans, quartanion.
        // If matrix has scale, so this function fails.
        IZ_BOOL bHasScale = CXFileMathUtil::BreakDownMatrix(
                                pNode->mtxTransform,
                                sJoint.pose);
#if _CHECK_SCALE
        if (bHasScale) {
            // TODO
            VRETURN(IZ_FALSE);
        }
#endif  // #if _CHECK_SCALE

        // Get parent index.
        if (pNode->parent != IZ_NULL) {
            std::vector<SJoint>::iterator it = std::find(
                                                m_JointList.begin(),
                                                m_JointList.end(),
                                                pNode->parent->name);
            VRETURN(it != m_JointList.end());

            sJoint.parent = static_cast<IZ_INT>(std::distance(m_JointList.begin(), it));
        }
        else {
            sJoint.parent = -1;
        }

        const SXFileBone* pBone = _GetBoneByName(
                                    pParser->GetBoneList(),
                                    sJoint.name);

        if (pBone != IZ_NULL) {
            izanagi::math::SMatrix::Copy(
                sJoint.mtxInv,
                pBone->mtx);
            sJoint.isMtxInvFromSkin = IZ_TRUE;
        }
        else {
            izanagi::math::SMatrix::Copy(
                sJoint.mtxInv,
                pNode->mtxTransform);
            sJoint.isMtxInvFromSkin = IZ_FALSE;
        }
    }

    // Build joint matrix.
    for (size_t i = 0; i < m_JointList.size(); i++) {
        SJoint& sJoint = m_JointList[i];

        if (!sJoint.isMtxInvFromSkin) {
            if (sJoint.parent >= 0) {
                const SJoint& sParent = m_JointList[sJoint.parent];

                izanagi::math::SMatrix::Mul(
                    sJoint.mtxInv,
                    sJoint.mtxInv,
                    sParent.mtxInv);
            }
        }
    }

    // Inverse matrix.
    for (size_t i = 0; i < m_JointList.size(); i++) {
        SJoint& sJoint = m_JointList[i];

        if (!sJoint.isMtxInvFromSkin) {
            izanagi::math::SMatrix::Inverse(sJoint.mtxInv, sJoint.mtxInv);
        }
    }

    return IZ_TRUE;
}

IZ_UINT CXFileJoint::GetJointNum()
{
    IZ_UINT ret = static_cast<IZ_UINT>(m_JointList.size());
    return ret;
}

IZ_PCSTR CXFileJoint::GetJointName(IZ_UINT nIdx)
{
    IZ_ASSERT(nIdx < GetJointNum());

    const SJoint& sJoint = m_JointList[nIdx];

    return sJoint.name.c_str();
}

IZ_INT CXFileJoint::GetJointParent(IZ_UINT nIdx)
{
    IZ_ASSERT(nIdx < GetJointNum());

    const SJoint& sJoint = m_JointList[nIdx];

    return sJoint.parent;
}

void CXFileJoint::GetJointInvMtx(
    IZ_UINT nIdx,
    izanagi::math::SMatrix& mtx)
{
    IZ_ASSERT(nIdx < GetJointNum());

    const SJoint& sJoint = m_JointList[nIdx];

    izanagi::math::SMatrix::Copy(
        mtx,
        sJoint.mtxInv);
}

void CXFileJoint::GetJointTransform(
    IZ_UINT nIdx,
    std::vector<SJointTransform>& tvTransform)
{
    IZ_ASSERT(nIdx < GetJointNum());

    const SJoint& sJoint = m_JointList[nIdx];

    // NOTE
    // Can't use SJointTransform::name

    // For quat.
    if (sJoint.pose.quat.x != 0.0f
        || sJoint.pose.quat.y != 0.0f
        || sJoint.pose.quat.z != 0.0f
        || sJoint.pose.quat.w != 1.0f)
    {
        tvTransform.push_back(SJointTransform());
        SJointTransform& sTransform = tvTransform.back();

        sTransform.type = E_MDL_JOINT_TRANSFORM_QUATERNION;

        sTransform.param.push_back(sJoint.pose.quat.x);
        sTransform.param.push_back(sJoint.pose.quat.y);
        sTransform.param.push_back(sJoint.pose.quat.z);
        sTransform.param.push_back(sJoint.pose.quat.w);
    }

    // For trans.
    if (sJoint.pose.trans[0] != 0.0f
        || sJoint.pose.trans[1] != 0.0f
        || sJoint.pose.trans[2] != 0.0f)
    {
        tvTransform.push_back(SJointTransform());
        SJointTransform& sTransform = tvTransform.back();

        sTransform.type = E_MDL_JOINT_TRANSFORM_TRANSLATE;

        sTransform.param.push_back(sJoint.pose.trans[0]);
        sTransform.param.push_back(sJoint.pose.trans[1]);
        sTransform.param.push_back(sJoint.pose.trans[2]);
    }
}
