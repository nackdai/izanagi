#if !defined(__SAMPLE_CAMERA_H__)
#define __SAMPLE_CAMERA_H__

#include "izSceneGraph.h"

namespace izanagi {
namespace sample {
    /** サンプルカメラ.
     */
    class CSampleCamera : public izanagi::CVectorCamera {
    public:
        CSampleCamera() {}
        ~CSampleCamera() {}

        NO_COPIABLE(CSampleCamera);

    public:
        void Init(
            const izanagi::math::CVector4& vPos,
            const izanagi::math::CVector4& vRef,
            const izanagi::math::CVector4& vUp,
            IZ_FLOAT fNear, IZ_FLOAT fFar,
            IZ_FLOAT fFov,
            IZ_FLOAT fAspect);

        void Update();

    public:
        void Dolly(IZ_FLOAT fDistScale);
        void Rotate(
            const izanagi::CFloatPoint& pt1,
            const izanagi::CFloatPoint& pt2);
        void Move(IZ_FLOAT fOffsetX, IZ_FLOAT fOffsetY);
    };
}
}

#endif    // #if !defined(__SAMPLE_CAMERA_H__)