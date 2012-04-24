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
		static void ComputeC2S(
			SMatrix& mtxC2S,
			IZ_INT screenWidth,
			IZ_INT screenHeight,
			IZ_FLOAT minZ,
			IZ_FLOAT maxZ);

		// スクリーン距離計算
		static IZ_FLOAT ComputeScreenDistance(
			IZ_FLOAT screenHeight,
			IZ_FLOAT fov);

		// クリップ座標取得
		static void Screen2Clip(
			SVector& vClip,
			const SCameraParam& sCamera,
			const SViewport& vp,
			IZ_INT nX, IZ_INT nY,
			IZ_FLOAT fZ);

		// 光線を取得
		static void Point2Ray(
			SVector& vRay,
			const SCameraParam& sCamera,
			const SViewport& vp,
			IZ_INT nX,
			IZ_INT nY);
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_UTIL_H__)
