#if !defined(__IZANAGI_SCENEGRAPH_UTIL_H__)
#define __IZANAGI_SCENEGRAPH_UTIL_H__

#include <vector>
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
        /** Clip - Screen 座標変換マトリクス計算.
         */
        static void ComputeC2S(
            math::SMatrix44& mtxC2S,
            IZ_INT screenWidth,
            IZ_INT screenHeight,
            IZ_FLOAT minZ,
            IZ_FLOAT maxZ);

        /** スクリーン距離計算.
         */
        static IZ_FLOAT ComputeScreenDistance(
            IZ_FLOAT screenHeight,
            IZ_FLOAT verticalFOV);

        /** クリップ座標取得.
         */
        static void Screen2Clip(
            math::SVector4& vClip,
            const graph::SViewport& vp,
            IZ_INT nX, IZ_INT nY,
            IZ_FLOAT fZ);

        /** 光線を取得.
         */
        static void Point2Ray(
            math::SVector4& vRay,
            const SCameraParam& sCamera,
            const graph::SViewport& vp,
            IZ_INT nX,
            IZ_INT nY);
    };

    /**
     */
    class CScissoring {
    private:
        CScissoring();
        ~CScissoring();

        NO_COPIABLE(CScissoring);

    public:
        /** シザリングで作成される三角形の数を計算する.
         */
        static IZ_UINT ComputeTriNumByScissoring(
            const math::CPlane& sissorPlane,
            const math::CTriangle triangle[],
            IZ_UINT triNum);

        /** 平面と交差する点の数を計算する.
         */
        static IZ_UINT GetCrossNum(
            const math::CPlane& sissorPlane,
            const math::CTriangle& triangle);

        /** シザリング.
         */
        static IZ_UINT Scissor(
            const math::CPlane& sissorPlane,
            const math::CTriangle triangle[],
            IZ_UINT triNum,
            math::CTriangle newTriangle[],
            IZ_UINT newTriNum);

        /** シザリング.
         */
        static IZ_UINT Scissor(
            const math::CPlane& sissorPlane,
            const math::CTriangle triangle[],
            IZ_UINT triNum,
            std::vector<math::CTriangle, STLMemoryAllocator<math::CTriangle> >& newTriangle);
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SCENEGRAPH_UTIL_H__)
