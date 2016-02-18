#if !defined(__IZANAGI_SCENEGRAPH_CAMERA_BASE_H__)
#define __IZANAGI_SCENEGRAPH_CAMERA_BASE_H__

#include "izDefs.h"
#include "izMath.h"

namespace izanagi {
	/** カメラパラメータ.
	 */
    struct SCameraParam {
        IZ_FLOAT cameraNear;    ///< near
        IZ_FLOAT cameraFar;     ///< far

        IZ_FLOAT horizontalFOV; ///< 縦画角(Horizontal Field Of View)

        IZ_FLOAT aspect;        ///< アスペクト比(aspect = width / height)

        math::SVector4 pos;     ///< 視点
        math::SVector4 ref;     ///< 注視点

        math::SVector4 up;      ///< 上方向ベクトル

        math::SMatrix44 mtxW2V; ///< World - View
        math::SMatrix44 mtxV2C; ///< View - Clip
        math::SMatrix44 mtxW2C; ///< World - Clip
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
        /** 初期化.
		 */
        void Init(
            const math::SVector4& vecPos,
            const math::SVector4& vecRef,
            const math::SVector4& vecUp,
            IZ_FLOAT fNear, IZ_FLOAT fFar,
            IZ_FLOAT fFov,
            IZ_FLOAT fAspect);

		/** 初期化.
		 */
        void Init(const SCameraParam& sParam);

        /** カメラ更新.
		 */
        virtual void Update();

        /** カメラ座標系でのオフセットを考慮にいれたV2Cマトリクスを取得.
         */
        void GetOffsetV2C(
            math::SMatrix44& mtxV2C,
            const math::SVector4& pos,
            IZ_FLOAT delta);

        /** カメラ座標系でのオフセットを考慮にいれたV2Cマトリクスを取得.
         */
        void GetOffsetV2C(
            math::SMatrix44& mtxV2C,
            IZ_FLOAT viewZ,
            IZ_FLOAT delta);

		/** 更新済みかどうか.
		 */
        IZ_BOOL IsUpdated() const;

		/** カメラパラメータを取得.
		 */
        const SCameraParam& GetParam() const;

		/** 視点をセット.
		 */
        virtual void SetPos(const math::SVector4& vecPos);

		/** 注視点をセット.
		 */
        virtual void SetAt(const math::SVector4& vecAt);

		/** 上方向ベクトルをセット.
		 */
        void SetUp(const math::SVector4& vecUp);

		/** カメラのnear値をセット.
		 */
        void SetNear(IZ_FLOAT fNear);

		/** カメラのfar値をセット.
		 */
        void SetFar(IZ_FLOAT fFar);

		/** カメラの画角をセット.
		 */
        void SetHorizontalFOV(IZ_FLOAT _horizontalFOV);

		/** カメラのアスペクト比をセット.
		 */
        void SetAspect(IZ_FLOAT fAspect);

		/** 視点から注視点への方向ベクトルを取得.
		 */
        math::SVector4 GetDir();

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
