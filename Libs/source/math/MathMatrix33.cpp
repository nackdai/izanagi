#include "math/MathMatrix33.h"
#include "math/MathQuaternion.h"

namespace izanagi
{
namespace math
{
    void SMatrix33::GetRotMatrixFromVector(SMatrix33& mtx, const SVector3& vec, const SVector3& up)
    {
        SVector3& vX = mtx.v[0];
        SVector3& vY = mtx.v[1];
        SVector3& vZ = mtx.v[2];

        SVector3::Normalize(vZ, vec);

        SVector3 vUp;
        if (CMath::IsNearyEqualZero(vZ.x) && CMath::IsNearyEqualZero(vZ.z)) {
            // UPベクトルとの外積を計算できないので、
            // 新しいUPベクトルをでっちあげる・・・
            if (up.y > 0.0f) {
                vUp.Set(0.0f, 0.0f, -vZ.y);
            }
            else {
                vUp.Set(0.0f, 0.0f, vZ.y);
            }
        }
        else {
            SVector3::Copy(vUp, up);
        }

        SVector3::Cross(vX, vUp, vZ);
        SVector3::Normalize(vX, vX);

        SVector3::Cross(vY, vZ, vX);
    }

    // マトリクスからオイラー角を取得する
    void _GetEulerFromMatrixXYZ(SVector3& angle, const SMatrix33& mtx)
    {
        // NOTE
        // X軸、Y軸、Z軸回転の行列を掛け合わせると以下のようになる
        //
        // m[0][0] = CyCz;          m[0][1] = CySz;          m[0][2] = -Sy;  m[0][3] = 0.0f;
        // m[1][0] = SxSyCz - CxSz; m[1][1] = SxSySz + CxCz  m[1][2] = SxCy; m[1][3] = 0.0f;
        // m[2][0] = CxSyCz + SxSz; m[2][1] = CxSySz - SxCz; m[2][2] = CxCy; m[2][3] = 0.0f;
        // m[3][0] = 0.0f;          m[3][1] = 0.0f;          m[3][2] = 0.0f; m[3][3] = 1.0f;
        //
        // Cx = X軸回転行列での cosθ
        // Sx = X軸回転行列での sinθ
        //  以下、Cy, Sy Cz, Sz も同様

        IZ_FLOAT Sy = -mtx._02;
        IZ_FLOAT Cy = ::sqrtf(1.0f - Sy * Sy);

        if (Cy != 0.0f) {
            //angle.y = ::atan2f(Sy, Cy);
            angle.y = ::acosf(Cy);

            IZ_FLOAT Sx = mtx._12 / Cy;
            IZ_FLOAT Cx = mtx._22 / Cy;
            angle.x = ::atan2f(Sx, Cx);

            IZ_FLOAT Sz = mtx._01 / Cy;
            IZ_FLOAT Cz = mtx._00 / Cy;
            angle.z = ::atan2f(Sz, Cz);
        }
        else {
            // NOTE
            // Yの回転角度が 90度 or 270度

            // NOTE
            // Sy = 1.0f として、行列を整理すると
            //
            // m[0][0] = 0.0f;        m[0][1] = 0.0f;        m[0][2] = -Sy;  m[0][3] = 0.0f;
            // m[1][0] = SxCz - CxSz; m[1][1] = SxSz + CxCz  m[1][2] = 0.0f; m[1][3] = 0.0f;
            // m[2][0] = CxCz + SxSz; m[2][1] = CxSz - SxCz; m[2][2] = 0.0f; m[2][3] = 0.0f;
            // m[3][0] = 0.0f;        m[3][1] = 0.0f;        m[3][2] = 0.0f; m[3][3] = 1.0f;
            //
            // m[0][0] = 0.0f;       m[0][1] = 0.0f;        m[0][2] = -1.0f; m[0][3] = 0.0f;
            // m[1][0] = sin(α-β); m[1][1] =  cos(α-β); m[1][2] =  0.0f; m[1][3] = 0.0f;
            // m[2][0] = cos(α-β); m[2][1] = -sin(α-β); m[2][2] =  0.0f; m[2][3] = 0.0f;
            // m[3][0] = 0.0f;       m[3][1] = 0.0f;        m[3][2] =  0.0f; m[3][3] = 1.0f;
            //
            // α = X軸回転時の角度
            // β = Z軸回転時の角度
            // ex) SxCz - CxSz = sinαcosβ - cosαsinβ = sin(α-β)
            //
            // 自由度がα-βのみに依存した行列となる
            // つまり、ジンバルロックの状態となる

            // そこで、Z軸の回転については回転無し（θ = 0) という制約を付けることで計算を解く

            if (Sy > 0.0f)
            {
                // Yの回転角度が -90度(=270度)
                angle.y = IZ_DEG2RAD(-90.0f);
            }
            else
            {
                // Yの回転角度が 90度
                angle.y = IZ_DEG2RAD(90.0f);
            }

            angle.z = 0.0f;

            IZ_FLOAT Cx = mtx._11;
            IZ_FLOAT Sx = mtx._10;
            angle.x = ::atan2f(Sx, Cx);
        }
    }

    void _GetEulerFromMatrixZXY(SVector3& angle, const SMatrix33& mtx)
    {
        // NOTE
        // m[0][0] = CzCy + SzSxSy  m[0][1] = SzCx m[0][2] = -CzSy + SzSxCy m[0][3] = 0
        // m[1][0] = -SzCy + CzSxSy m[1][1] = CzCx m[1][2] = SzSy + CzSxCy  m[1][3] = 0
        // m[2][0] = CxSy           m[2][1] = -Sx  m[2][2] = CxCy           m[2][3] = 0
        // m[3][0] = 0              m[3][1] = 0    m[3][2] = 0              m[3][3] = 1

        IZ_FLOAT Sx = -mtx._21;
        IZ_FLOAT Cx = ::sqrtf(1.0f - Sx * Sx);

        if (Cx != 0.0f)
        {
            angle.x = ::acosf(Cx);

            IZ_FLOAT Sy = mtx._20 / Cx;
            IZ_FLOAT Cy = mtx._22 / Cx;
            angle.y = ::atan2f(Sy, Cy);

            IZ_FLOAT Sz = mtx._01 / Cx;
            IZ_FLOAT Cz = mtx._11 / Cx;
            angle.z = ::atan2f(Sz, Cz);
        }
        else
        {
            if (Cx > 0.0f)
            {
                // Xの回転角度が -90度(=270度)
                angle.x = IZ_DEG2RAD(-90.0f);
            }
            else
            {
                // Xの回転角度が 90度
                angle.x = IZ_DEG2RAD(90.0f);
            }

            angle.y = 0.0f;

            IZ_FLOAT Cz = mtx._00;
            IZ_FLOAT Sz = -mtx._10;
            angle.z = ::atan2f(Sz, Cz);
        }
    }

    // マトリクスからオイラー角を取得する
    void SMatrix33::GetEulerFromMatrix(SVector3& angle, const SMatrix33& mtx, E_MATH_ROTATION_ORDER order)
    {
        typedef void (*Func)(SVector3&, const SMatrix33&);
        Func funcs[] =
        {
            _GetEulerFromMatrixXYZ,
            IZ_NULL,    // XZY
            IZ_NULL,    // YXZ
            IZ_NULL,    // YZX
            _GetEulerFromMatrixZXY,
            IZ_NULL,    // ZYX
        };
        IZ_C_ASSERT(COUNTOF(funcs) == E_MATH_ROTATION_ORDER_NUM);

        IZ_ASSERT(funcs[order] != IZ_NULL);

        (*funcs[order])(angle, mtx);
    }
}   // namespace math
}   // namespace izanagi
