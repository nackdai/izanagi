#if !defined(__IZANAGI_SCENEGRAPH_UTIL_H__)
#define __IZANAGI_SCENEGRAPH_UTIL_H__

#include "izMath.h"
#include "graph/GraphDefs.h"
#include "Camera.h"

namespace izanagi {
	/**
	*/
	class CSceneGraphUtil {
	private:
		CSceneGraphUtil();
		~CSceneGraphUtil();

		NO_COPIABLE(CSceneGraphUtil);

	public:
		// Clip - Screen 座標変換マトリクス計算
		void ComputeC2S(
			SMatrix& mtxC2S,
			IZ_FLOAT fScreenWidth,
			IZ_FLOAT fScreenHeight,
			IZ_FLOAT fCenterX,
			IZ_FLOAT fCenterY,
			IZ_FLOAT fMinZ,
			IZ_FLOAT fMaxZ);

		// スクリーン距離計算
		IZ_FLOAT ComputeScreenDistance(
			IZ_FLOAT fScreenWidth,
			IZ_FLOAT fFov);

		// クリップ座標取得
		void Point2Clip(
			SVector& vClip,
			const SCameraParam& sCamera,
			const SViewport& vp,
			IZ_INT nX,
			IZ_INT nY);

		// 光線を取得
		void Point2Ray(
			SVector& vRay,
			const SCameraParam& sCamera,
			const SViewport& vp,
			IZ_INT nX,
			IZ_INT nY);
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_UTIL_H__)
