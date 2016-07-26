#if !defined(__IZANAGI_SCENEGRAPH_STEREO_CAMERA_H__)
#define __IZANAGI_SCENEGRAPH_STEREO_CAMERA_H__

#include "izDefs.h"
#include "izMath.h"
#include "izGraph.h"
#include "VectorCamera.h"

namespace izanagi {
    /**
     */
    class StereoCamera {
    private:
        StereoCamera();
        ~StereoCamera();

    public:

        enum Eye {
            Left,
            Right,
        };

        static void getCamera(
            Eye type,
            IZ_FLOAT ipd,   // 瞳孔間距離
            CVectorCamera& dstCamera,
            const CVectorCamera& srcCamera);
    };

}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SCENEGRAPH_VECTOR_CAMERA_H__)
