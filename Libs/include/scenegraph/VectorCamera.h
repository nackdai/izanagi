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
         *
         * vecUp は無視される.
         */
        virtual void Init(
            const math::SVector4& vecPos,
            const math::SVector4& vecRef,
            const math::SVector4& vecUp,
            IZ_FLOAT fNear, IZ_FLOAT fFar,
            IZ_FLOAT fFov,
            IZ_FLOAT fAspect) override;

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
        void RotatePos(
            const math::SVector4& axis,
            IZ_FLOAT rad);

		void RotatePos(const math::SQuat& quat);

        void RotateAt(
            const math::SVector4& axis,
            IZ_FLOAT rad);

        void RotateAt(const math::SQuat& quat);

        /** 位置指定.
         */
        virtual void SetPos(const math::SVector4& pos) override;

        virtual void SetAt(const math::SVector4& vecAt) override;

        virtual void SetUp(const math::SVector4& vecUp) override
        {
            // Nothing is done...
        }

        virtual void Dump() const override;

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
        inline void computeLenghtBetweenPosAndAt();

    protected:
        math::SVector4 m_X;
        
        // NOTE
        // m_Param.up の参照とすることで m_Param.up と同期するようにする
        math::SVector4& m_Y;

        math::SVector4 m_Z;

        // NOTE
        // m_Param.pos の参照とすることで m_Param.pos と同期するようにする
        math::SVector4& m_Pos;

        // NOTE
        // m_Param.ref の参照とすることで m_Param.ref と同期するようにする
        math::SVector4& m_At;

        // カメラの変換マトリクス
        math::SMatrix44 m_Transform;

        IZ_BOOL m_NeedUpdateByTransform;

        IZ_FLOAT m_lenghtBetweenPosAndAt{ 0.0f };
    };

}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SCENEGRAPH_VECTOR_CAMERA_H__)
