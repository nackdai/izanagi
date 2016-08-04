#if !defined(__IZANAGI_SCENEGRAPH_STEREO_CAMERA_H__)
#define __IZANAGI_SCENEGRAPH_STEREO_CAMERA_H__

#include "izDefs.h"
#include "izMath.h"
#include "izGraph.h"
#include "VectorCamera.h"

namespace izanagi {
    struct HmdInfo {
        IZ_FLOAT HResolution{ 1280.0f };            // HMD horizontal resolution in pixels
        IZ_FLOAT VResolution{ 800.0f };             // HMD vertical resolution in pixels
        IZ_FLOAT HScreenSize{ 0.14976f };           // HMD horizontal size in meters. 0.14976[m].
        IZ_FLOAT VScreenSize{ 0.09356f };           // HMD vertical size in meters. 0.09356f[m].
        IZ_FLOAT VScreenCenter{ 0.04678f };         // HMD screen center in meters. 0.04678f[m].
        IZ_FLOAT EyeToScreenDistance{ 0.041f };     // HMD distance between eye and display in meters. 0.041f[m]
        IZ_FLOAT LensSeparationDistance{ 0.0635f }; // HMD lens separation distance in meters. 0.0635f[m].
        IZ_FLOAT InterpupillaryDistance{ 0.064f };  // HMD IPD (distance between pupils) in meters. 0.064f[m].
        IZ_FLOAT DistortionK[4];                    // HMD lens distortion constants
        IZ_FLOAT ChromaAbCorrection[4];             // HMD chromatic aberration correction parameters

        HmdInfo()
        {
            DistortionK[0] = 1.0f;
            DistortionK[1] = 0.22f;
            DistortionK[2] = 0.24f;
            DistortionK[3] = 0.0f;

            ChromaAbCorrection[0] = 0.996f;
            ChromaAbCorrection[1] = -0.004f;
            ChromaAbCorrection[2] = 1.014f;
            ChromaAbCorrection[3] = 0.0f;
        }
    };

    // NOTE
    // a typical DK1 half-FOV is 46 degrees inwards, 53 degrees outwards
    // https://developer.oculusvr.com/forums/viewtopic.php?p=111699#p111699
    //
    // HFOV = 109 VFOV = 112.5
    // https://forums.oculus.com/community/discussion/19476/oculus-dk1-fov
    // 
    // values for the DK2's default fov
    // 106.19 degrees vertical 
    // 95.06 degrees horizontal
    // 47.53 degrees to the side and 46.63 degrees towards the nose
    // https://forums.oculus.com/community/discussion/23715/fov-of-the-oculus-rift-dk2
    struct FovPort {
        IZ_FLOAT tanLeft;
        IZ_FLOAT tanRight;
        IZ_FLOAT tanTop;
        IZ_FLOAT tanBottom;
    };

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

        static void setFov(
            Eye type,
            IZ_FLOAT degreeInwardsHFov,
            IZ_FLOAT degreeOutwardsHFov,
            IZ_FLOAT degfeeVFov,
            FovPort& fov);

        static void getCamera(
            Eye type,
            const HmdInfo& info,
            const FovPort& fov,
            CVectorCamera& dstCamera,
            const CVectorCamera& srcCamera);
    };

}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SCENEGRAPH_VECTOR_CAMERA_H__)
