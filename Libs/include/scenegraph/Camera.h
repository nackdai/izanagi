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

	class CCamera {
	public:
		CCamera()
		{
			m_IsDirtyW2V = IZ_FALSE;
			m_IsDirtyV2C = IZ_FALSE;
		}
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
		void Update();

		IZ_BOOL IsUpdated() const { return (!m_IsDirtyW2V && !m_IsDirtyV2C); }

		const SCameraParam& GetParam() const { return m_Param; }

		void SetPos(const SVector& vecPos)
		{
			SVector::Copy(m_Param.pos, vecPos);
			m_IsDirtyW2V = IZ_TRUE;
		}

		void SetAt(const SVector& vecAt)
		{
			SVector::Copy(m_Param.ref, vecAt);
			m_IsDirtyW2V = IZ_TRUE;
		}

		void SetUp(const SVector& vecUp)
		{
			SVector::Copy(m_Param.up, vecUp);
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

}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_CAMERA_BASE_H__)
