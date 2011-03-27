#if !defined(__IZANAGI_SCENEGRAPH_CAMERA_BASE_H__)
#define __IZANAGI_SCENEGRAPH_CAMERA_BASE_H__

#include "izDefs.h"
#include "izMath.h"

namespace izanagi {
	class CGraphicsDevice;

	struct SCameraParam {
		IZ_FLOAT cameraNear;
		IZ_FLOAT cameraFar;

		// Vertical FOV
		IZ_FLOAT fov;

		// aspect = width / height
		IZ_FLOAT aspect;

		SVector pos;		// 視点
		SVector ref;		// 注視点

		SVector up;

		SMatrix mtxW2V;	// World - View
		SMatrix mtxV2C;	// View - Clip
		SMatrix mtxW2C;	// World - Clip
	};

	class CCamera : public SCameraParam {
		friend class CStereoCamera;

	public:
		CCamera() {}
		~CCamera() {}

		NO_COPIABLE(CCamera);

	public:
		// 初期化
		void Init(
			const SVector& vecPos,
			const SVector& vecRef,
			const SVector& vecUp,
			IZ_FLOAT fNear, IZ_FLOAT fFar,
			IZ_FLOAT fFov,
			IZ_FLOAT fAspect);

		void Init(const SCameraParam& sParam);

		// カメラ更新
		void UpdateCamera();

#if 0
		// カメラセット
		void RenderCamera(CGraphicsDevice* pDevice);
#endif

	protected:
		// World - View
		void ComputeW2V();

		// View - Clip
		void ComputeV2C();
	};

}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_CAMERA_BASE_H__)
