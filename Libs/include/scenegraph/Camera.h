#if !defined(__IZANAGI_SCENEGRAPH_CAMERA_BASE_H__)
#define __IZANAGI_SCENEGRAPH_CAMERA_BASE_H__

#include "izDefs.h"
#include "izMath.h"

namespace izanagi {
    struct SCameraParam {
        IZ_FLOAT cameraNear;
        IZ_FLOAT cameraFar;

        // Vertical FOV
        IZ_FLOAT fov;

        // aspect = width / height
        IZ_FLOAT aspect;

        math::SVector pos;      // 視点
        math::SVector ref;      // 注視点

        math::SVector up;

        math::SMatrix mtxW2V;   // World - View
        math::SMatrix mtxV2C;   // View - Clip
        math::SMatrix mtxW2C;   // World - Clip
    };

    class CCamera {
    public:
        CCamera()
        {
            m_IsDirtyW2V = IZ_FALSE;
            m_IsDirtyV2C = IZ_FALSE;
        }
        virtual ~CCamera() {}

        NO_COPIABLE(CCamera);

    public:
        // 初期化
        void Init(
            const math::SVector& vecPos,
            const math::SVector& vecRef,
            const math::SVector& vecUp,
            IZ_FLOAT fNear, IZ_FLOAT fFar,
            IZ_FLOAT fFov,
            IZ_FLOAT fAspect);

        void Init(const SCameraParam& sParam);

        // カメラ更新
        virtual void Update();

        /** カメラ座標系でのオフセットを考慮にいれたV2Cマトリクスを取得.
         */
        void GetOffsetV2C(
            math::SMatrix& mtxV2C,
            const math::SVector& pos,
            IZ_FLOAT delta);

        /** カメラ座標系でのオフセットを考慮にいれたV2Cマトリクスを取得.
         */
        void GetOffsetV2C(
            math::SMatrix& mtxV2C,
            IZ_FLOAT viewZ,
            IZ_FLOAT delta);

        IZ_BOOL IsUpdated() const { return (!m_IsDirtyW2V && !m_IsDirtyV2C); }

        const SCameraParam& GetParam() const { return m_Param; }

        void SetPos(const math::SVector& vecPos)
        {
            math::SVector::Copy(m_Param.pos, vecPos);
            m_IsDirtyW2V = IZ_TRUE;
        }

        void SetAt(const math::SVector& vecAt)
        {
            math::SVector::Copy(m_Param.ref, vecAt);
            m_IsDirtyW2V = IZ_TRUE;
        }

        void SetUp(const math::SVector& vecUp)
        {
            math::SVector::Copy(m_Param.up, vecUp);
            m_IsDirtyW2V = IZ_TRUE;
        }

        void SetNear(IZ_FLOAT fNear)
        {
            m_Param.cameraNear = fNear;
            m_IsDirtyV2C = IZ_TRUE;
        }

        void SetFar(IZ_FLOAT fFar)
        {
            m_Param.cameraFar = fFar;
            m_IsDirtyV2C = IZ_TRUE;
        }

        void SetFOV(IZ_FLOAT fFOV)
        {
            m_Param.fov = fFOV;
            m_IsDirtyV2C = IZ_TRUE;
        }

        void SetAspect(IZ_FLOAT fAspect)
        {
            m_Param.aspect = fAspect;
            m_IsDirtyV2C = IZ_TRUE;
        }

        math::SVector GetDir();

    protected:
        // World - View
        void ComputeW2V();

        // View - Clip
        void ComputeV2C();

    protected:
        SCameraParam m_Param;

        IZ_BOOL m_IsDirtyW2V;
        IZ_BOOL m_IsDirtyV2C;
    };

}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SCENEGRAPH_CAMERA_BASE_H__)
