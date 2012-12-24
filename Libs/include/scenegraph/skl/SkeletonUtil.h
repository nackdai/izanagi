#if !defined(__IZANAGI_SCENEGRAPH_SKELETON_UTIL_H__)
#define __IZANAGI_SCENEGRAPH_SKELETON_UTIL_H__

#include "SKLFormat.h"

namespace izanagi {
    class CSkeletonUtil {
        friend class CSkeletonInstance;
        friend class CPoseUpdater;

    private:
        CSkeletonUtil();
        ~CSkeletonUtil();

        NO_COPIABLE(CSkeletonUtil);

    private:
        static void UpdatePose(
            S_SKL_JOINT_POSE& pose,
            IZ_UINT transformType,
            IZ_UINT paramType,
            const math::SVector& param);
    };
}   // namespace iznagi

#endif  // #if !defined(__IZANAGI_SCENEGRAPH_SKELETON_INSTANCE_H__)
