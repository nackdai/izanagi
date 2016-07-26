#if !defined(__IZANAGI_SCENEGRAPH_VECTOR_CAMERA_H__)
#define __IZANAGI_SCENEGRAPH_VECTOR_CAMERA_H__

#include "izDefs.h"
#include "izMath.h"
#include "Camera.h"

namespace izanagi {
    /**
     */
    class CVectorCamera : public CCamera {
        friend class StereoCamera;

    public:
        CVectorCamera();
        virtual ~CVectorCamera();

        NO_COPIABLE(CVectorCamera);

    public:
        /** 初期化.
         */
        void Init(
            const math::SVector4& pos,
            const math::SVector4& at,
            IZ_FLOAT fNear, IZ_FLOAT fFar,
            IZ_FLOAT verticalFOV,
            IZ_FLOAT aspect);

        /** カメラ更新.
         */
        virtual void Update() override;

        /** 移動.
         */
        void Translate(const math::SVector4& trans);

        /** X軸回転.
         */
        void RotateByX(IZ_FLOAT rad);

        /** Y軸回転.
         */
        void RotateByY(IZ_FLOAT rad);

        /** Z軸回転.
         */
        void RotateByZ(IZ_FLOAT rad);

        /** 任意軸回転.
         */
        void Rotate(
            const math::SVector4& axis,
            IZ_FLOAT rad);

		void Rotate(const math::SQuat& quat);

        /** 位置指定.
         */
        void SetPos(const math::SVector4& pos);

        void SetTransform(const math::SMatrix44& mtx);

    public:
        /** カメラのX軸取得
         */
        const math::SVector4& GetX() const { return m_X; }

        /** カメラのY軸取得
         */
        const math::SVector4& GetY() const { return m_Y; }

        /** カメラのZ軸取得
         */
        const math::SVector4& GetZ() const { return m_Z; }

        /** カメラの位置取得
         */
        const math::SVector4& GetPos() const { return m_Pos; }

        /**
         */
        const math::SMatrix44& GetTransform() const { return m_Transform; }

    private:
        // 使わせない

        void Init(
            const math::SVector4& vecPos,
            const math::SVector4& vecRef,
            const math::SVector4& vecUp,
            IZ_FLOAT fNear, IZ_FLOAT fFar,
            IZ_FLOAT fFov,
            IZ_FLOAT fAspect)
        {
        }

        void Init(const SCameraParam& sParam) {}

        void SetAt(const math::SVector4& vecAt) {}

        void SetUp(const math::SVector4& vecUp) {}

    protected:
        math::SVector4 m_X;
        
        // NOTE
        // m_Param.up の参照とすることで m_Param.up と同期するようにする
        math::SVector4& m_Y;

        math::SVector4 m_Z;

        // NOTE
        // m_Param.pos の参照とすることで m_Param.pos と同期するようにする
        math::SVector4& m_Pos;

        // カメラの変換マトリクス
        math::SMatrix44 m_Transform;

        IZ_BOOL m_NeedUpdateByTransform;
    };

}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SCENEGRAPH_VECTOR_CAMERA_H__)
