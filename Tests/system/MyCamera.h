#if !defined(__IZ_CAMERA_H__)
#define __IZ_CAMERA_H__

#include "izSceneGraph.h"

class CMyCamera {
protected:
	static CMyCamera s_cInstance;

public:
	static CMyCamera& GetInstance() { return s_cInstance; }

protected:
	CMyCamera() {}
	~CMyCamera() {}

	CMyCamera(const CMyCamera& rhs);
	const CMyCamera& operator =(const CMyCamera& rhs);

public:
	void Init(
		const izanagi::CVector& vPos,
		const izanagi::CVector& vRef,
		const izanagi::CVector& vUp,
		IZ_FLOAT fNear, IZ_FLOAT fFar,
		IZ_FLOAT fFov,
		IZ_FLOAT fAspect);

	inline void Update();
#if 0
	inline void RenderCamera(izanagi::graph::CGraphicsDevice* pDevice);
#endif

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

// inline ********************************

void CMyCamera::Update()
{
	m_cCamera.Update();
}

#if 0
void CMyCamera::RenderCamera(izanagi::graph::CGraphicsDevice* pDevice)
{
	m_cCamera.Update();
}
#endif

#endif	// #if !defined(__IZ_CAMERA_H__)