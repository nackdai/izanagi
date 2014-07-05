#if !defined(__IZANAGI_SCENEGRAPH_VECTOR_CAMERA_H__)
#define __IZANAGI_SCENEGRAPH_VECTOR_CAMERA_H__

#include "izDefs.h"
#include "izMath.h"
#include "Camera.h"

namespace izanagi {
    /**
     */
    class CVectorCamera : public CCamera {
    public:
        CVectorCamera();
        virtual ~CVectorCamera();

        NO_COPIABLE(CVectorCamera);

    public:
        /** 初期化.
         */
        void Init(
            const math::SVector& pos,
            const math::SVector& at,
            IZ_FLOAT fNear, IZ_FLOAT fFar,
            IZ_FLOAT fov,
            IZ_FLOAT aspect);

        /** カメラ更新.
         */
        virtual void Update();

        /** 移動.
         */
        void Translate(const math::SVector& trans);

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
            const math::SVector& axis,
            IZ_FLOAT rad);

        /** 位置指定.
         */
        void SetPos(const math::SVector& pos);

        void SetTransform(const math::SMatrix& mtx);

    public:
        /** カメラのX軸取得
         */
        const math::SVector& GetX() const { return m_X; }

        /** カメラのY軸取得
         */
        const math::SVector& GetY() const { return m_Y; }

        /** カメラのZ軸取得
         */
        const math::SVector& GetZ() const { return m_Z; }

        /** カメラの位置取得
         */
        const math::SVector& GetPos() const { return m_Pos; }

        /**
         */
        const math::SMatrix& GetTransform() const { return m_Transform; }

    private:
        // 使わせない

        void Init(
            const math::SVector& vecPos,
            const math::SVector& vecRef,
            const math::SVector& vecUp,
            IZ_FLOAT fNear, IZ_FLOAT fFar,
            IZ_FLOAT fFov,
            IZ_FLOAT fAspect)
        {
        }

        void Init(const SCameraParam& sParam) {}

        void SetAt(const math::SVector& vecAt) {}

        void SetUp(const math::SVector& vecUp) {}

    private:
        math::SVector m_X;
        
        // NOTE
        // m_Param.up の参照とすることで m_Param.up と同期するようにする
        math::SVector& m_Y;

        math::SVector m_Z;

        // NOTE
        // m_Param.pos の参照とすることで m_Param.pos と同期するようにする
        math::SVector& m_Pos;

        // カメラの変換マトリクス
        math::SMatrix m_Transform;

        IZ_BOOL m_NeedUpdateByTransform;
    };

}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SCENEGRAPH_VECTOR_CAMERA_H__)
