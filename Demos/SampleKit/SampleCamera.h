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
        void Dolly(IZ_FLOAT fDistScale);
        void Rotate(
            const izanagi::CFloatPoint& pt1,
            const izanagi::CFloatPoint& pt2);
        void Move(IZ_FLOAT fOffsetX, IZ_FLOAT fOffsetY);
    };
}
}

#endif    // #if !defined(__SAMPLE_CAMERA_H__)