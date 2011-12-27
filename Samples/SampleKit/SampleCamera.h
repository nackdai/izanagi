#if !defined(__SAMPLE_CAMERA_H__)
#define __SAMPLE_CAMERA_H__

#include "izSceneGraph.h"

namespace izanagi {
namespace sample {
	/** サンプルカメラ.
	 */
	class CSampleCamera {
	public:
		CSampleCamera() {}
		~CSampleCamera() {}

		NO_COPIABLE(CSampleCamera);

	public:
		void Init(
			const izanagi::CVector& vPos,
			const izanagi::CVector& vRef,
			const izanagi::CVector& vUp,
			IZ_FLOAT fNear, IZ_FLOAT fFar,
			IZ_FLOAT fFov,
			IZ_FLOAT fAspect);

		void Update();

	public:
		void Dolly(IZ_FLOAT fDistScale);
		void Rotate(IZ_FLOAT fLatitude, IZ_FLOAT fLongitude);
		void Move(IZ_FLOAT fOffsetX, IZ_FLOAT fOffsetY);

	protected:
		izanagi::CCamera m_cCamera;

		IZ_FLOAT m_fYaw;
		IZ_FLOAT m_fPitch;
		IZ_FLOAT m_fRoll;

	public:
		izanagi::CCamera& GetRawInterface() { return m_cCamera; }
	};
}
}

#endif	// #if !defined(__SAMPLE_CAMERA_H__)