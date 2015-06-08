#include "IkApp.h"

// NOTE
// JointIdx : 0       1       2       3
// BoneIdx  : +-- 0 --+-- 1 --+-- 2 --+

struct Joint {
    izanagi::math::SVector4 trans;
    izanagi::math::SQuat quat;
    izanagi::math::SMatrix44 mtx;
    Joint* parent;

    izanagi::math::SVector4 pos;

    void ComputeLocalMatrix();
    void ComputePos();
};

void Joint::ComputeLocalMatrix()
{
    izanagi::math::SMatrix44::SetUnit(mtx);
    izanagi::math::SQuat::MatrixFromQuat(mtx, quat);
    izanagi::math::SMatrix44::Trans(mtx, mtx, trans);
}

void Joint::ComputePos()
{
    pos.Set(0.0f, 0.0f, 0.0f);

    izanagi::math::SMatrix44::Apply(
        pos,
        pos,
        mtx);
}

/////////////////////////////////////////////

Joint joints[5];
izanagi::math::SVector4 s_Target;
IZ_UINT s_RecursiveNum = 1;

IZ_FLOAT angle = 90.0f;
IZ_BOOL enableLimitAngle = IZ_FALSE;

void UpdateJointMatrix();

// 初期化
void InitJoints()
{
    static const IZ_FLOAT LENGTH = 100.0f;

    FILL_ZERO(joints, sizeof(joints));

    // 親子関係
    {
        for (IZ_UINT i = 1; i < COUNTOF(joints); i++) {
            joints[i].parent = &joints[i - 1];
        }
    }

    // 位置
    {
        joints[0].trans.Set(SCREEN_WIDTH >> 1, 0.0f, 0.0f);

        for (IZ_UINT i = 1; i < COUNTOF(joints); i++) {
            joints[i].trans.Set(
                0.0f,
                LENGTH,
                0.0f);
        }
    }

    // 回転
    for (IZ_UINT i = 0; i < COUNTOF(joints); i++) {
        izanagi::math::SQuat::SetQuatFromRadAxis(
            joints[i].quat,
            0.0f,
            0.0f, 0.0f, 1.0f);
    }

    // 行列
    for (IZ_UINT i = 0; i < COUNTOF(joints); i++) {
        izanagi::math::SMatrix44::SetUnit(joints[i].mtx);
    }
}

void UpdateJointMatrix()
{
    for (IZ_UINT i = 0; i < COUNTOF(joints); i++) {
        joints[i].ComputeLocalMatrix();
    }

    for (IZ_UINT i = 1; i < COUNTOF(joints); i++) {
        Joint* parent = joints[i].parent;
        
        if (parent != IZ_NULL) {
            const izanagi::math::SMatrix44 mtxParent = parent->mtx;

            izanagi::math::SMatrix44::Mul(
                joints[i].mtx,
                joints[i].mtx,
                mtxParent);
        }
    }
}

/** 制限角度のチェック
 */
void CheckLimitAngle(Joint* joint)
{
    if (enableLimitAngle) {
        izanagi::math::SVector4 angle;
        izanagi::math::SQuat::GetEuler(angle, joint->quat);

        angle.z = izanagi::math::CMath::Clamp(
            angle.z,
            IZ_DEG2RAD(-90.0f),
            IZ_DEG2RAD(90.0f));

        izanagi::math::SQuat::QuatFromEuler(joint->quat, angle);
    }
}

/** IK適用.
 *
 * @param[in] recursiveNum 再帰回数
 * @param[in] target 目標位置
 */
void ApplyIK(
    IZ_UINT recursiveNum,
    const izanagi::math::SVector4& target)
{
    // 先端
    Joint* effector = &joints[COUNTOF(joints) - 1];

    izanagi::math::SVector4 localEffector;
    izanagi::math::SVector4 localTarget;

    izanagi::math::SVector4 baseCoordEffector;
    izanagi::math::SVector4 baseCoordTarget;

    for (IZ_UINT i = 0; i < recursiveNum; i++) {
        // NOTE
        // 先端から計算を行う
        for (Joint* joint = effector->parent; joint != IZ_NULL; joint = joint->parent) {
            // 関節の行列を更新
            UpdateJointMatrix();
            joint->ComputePos();

            // 基準関節の座標空間に変換するための逆行列
            izanagi::math::SMatrix44 mtx;
            {
                izanagi::math::SMatrix44::Inverse(mtx, joint->mtx);
            }

            // 先端を基準関節の座標空間に変換
            effector->ComputePos();
            izanagi::math::SMatrix44::Apply(localEffector, effector->pos, mtx);
            

            // 目標位置を基準関節の座標空間に変換
            izanagi::math::SMatrix44::Apply(localTarget, target, mtx);

            izanagi::math::SVector4::Normalize(baseCoordEffector, localEffector);
            izanagi::math::SVector4::Normalize(baseCoordTarget, localTarget);

            // ベクトル1 = 先端 - 基準関節
            // ベクトル2 = 目標位置 - 基準関節
            // ベクトル1とベクトル2のなす角度
            IZ_FLOAT dot = izanagi::math::SVector4::Dot(baseCoordEffector, baseCoordTarget);

            IZ_FLOAT angle = ::acosf(dot);

            IZ_FLOAT deg = IZ_RAD2DEG(angle);

            //if (!izanagi::math::CMath::IsNearyEqualZero(angle)) {
            if (angle > 1.0e-5f) {
                // 回転軸
                izanagi::math::SVector4 axis;
                izanagi::math::SVector4::Cross(axis, baseCoordEffector, baseCoordTarget);
                izanagi::math::SVector4::Normalize(axis, axis);

                izanagi::math::SQuat quat;
                izanagi::math::SQuat::SetQuatFromRadAxis(quat, angle, axis);

                // 関節の回転を更新
                izanagi::math::SQuat::Mul(joint->quat, joint->quat, quat);

                CheckLimitAngle(joint);
            }
        }

#if 1
        {
            // 関節の行列を更新
            UpdateJointMatrix();

            // 基準関節の座標空間に変換するための逆行列
            izanagi::math::SMatrix44 mtx;
            {
                izanagi::math::SMatrix44::Inverse(mtx, joints[0].mtx);
            }

            // 先端を基準関節の座標空間に変換
            effector->ComputePos();
            izanagi::math::SMatrix44::Apply(localEffector, effector->pos, mtx);

            // 目標位置を基準関節の座標空間に変換
            izanagi::math::SMatrix44::Apply(localTarget, target, mtx);
        }
#endif

        // 最終位置と目標位置の距離が限りなくゼロになったらもうやめる
        IZ_FLOAT length = izanagi::math::SVector4::Length2(localEffector, localTarget);
        if (izanagi::math::CMath::IsNearyEqualZero(length)) {
            break;
        }
    }
}

//////////////////////////////////////

CIkApp::CIkApp()
{
}

CIkApp::~CIkApp()
{
}

// 初期化.
IZ_BOOL CIkApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    InitJoints();
    
    UpdateJointMatrix();
    joints[COUNTOF(joints) - 1].ComputePos();
    izanagi::math::SVector4::Copy(s_Target, joints[COUNTOF(joints) - 1].pos);

    return IZ_TRUE;
}

// 解放.
void CIkApp::ReleaseInternal()
{
}

// 更新.
void CIkApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    InitJoints();
    ApplyIK(s_RecursiveNum, s_Target);
}

// 描画.
void CIkApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    static const IZ_COLOR bgColor = IZ_COLOR_RGBA(0, 128, 255, 255);

    izanagi::math::SVector4 pos[COUNTOF(joints)];

    if (device->Begin2D()) {
        for (IZ_UINT i = 0; i < COUNTOF(joints); i++) {
            joints[i].ComputePos();
            izanagi::math::SVector4::Copy(pos[i], joints[i].pos);
        }

        for (IZ_UINT i = 0; i < COUNTOF(joints) - 1; i++) {
            device->Draw2DLine(
                izanagi::CIntPoint(pos[i].x, pos[i].y),
                izanagi::CIntPoint(pos[i + 1].x, pos[i + 1].y),
                IZ_COLOR_RGBA(0xff, 0, 0, 0xff));
        }

        device->Draw2DLine(
            izanagi::CIntPoint(pos[0].x, pos[0].y),
            izanagi::CIntPoint(s_Target.x, s_Target.y),
            IZ_COLOR_RGBA(0, 0xff, 0, 0xff));

        GetDebugFont()->Begin();
        GetDebugFont()->DBPrint(
            0, 300,
            "(%d) [%d][%d]\n(%.2f)",
            s_RecursiveNum,
            (IZ_UINT)s_Target.x, (IZ_UINT)s_Target.y,
            angle);
        GetDebugFont()->DBPrint(
            0, 340,
            "LimitAngle[%s]",
            enableLimitAngle ? "ON" : "OFF");
        GetDebugFont()->End();

        device->End2D();
    }
}

IZ_BOOL CIkApp::OnMouseLBtnDown(const izanagi::CIntPoint& point)
{
    s_Target.x = (IZ_FLOAT)point.x;
    s_Target.y = (IZ_FLOAT)point.y;
    s_Target.z = 0.0f;

    return IZ_TRUE;
}

IZ_BOOL CIkApp::OnKeyDown(IZ_UINT nChar)
{
    if (nChar == VK_UP) {
        s_RecursiveNum++;
    }
    else if (nChar == VK_DOWN) {
        if (s_RecursiveNum > 1) {
            s_RecursiveNum--;
        }
    }
    else if (nChar == VK_LEFT) {
        angle -= 1.0f;
        IZ_FLOAT x = 200.0f * cosf(IZ_DEG2RAD(angle));
        IZ_FLOAT y = 200.0f * sinf(IZ_DEG2RAD(angle));

        s_Target.x = 640.0f - x;
        s_Target.y = y;
    }
    else if (nChar == VK_RIGHT) {
        angle += 1.0f;
        IZ_FLOAT x = 200.0f * cosf(IZ_DEG2RAD(angle));
        IZ_FLOAT y = 200.0f * sinf(IZ_DEG2RAD(angle));

        s_Target.x = 640.0f - x;
        s_Target.y = y;
    }
    else if (nChar = 'B') {
        enableLimitAngle = !enableLimitAngle;
    }

    return IZ_TRUE;
}
