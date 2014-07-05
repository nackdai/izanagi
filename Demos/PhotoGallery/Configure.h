#if !defined(__CONFIGURE_H__)
#define __CONFIGURE_H__

#include "izSampleKit.h"

class Configure {
public:
    // Flags for vertex semantics.
    static const IZ_UINT MeshFlags = izanagi::E_DEBUG_MESH_VTX_FORM_POS
        | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR
        | izanagi::E_DEBUG_MESH_VTX_FORM_NORMAL;

    // Slices of seat.
    static const IZ_UINT Slices = 20;

    static const IZ_COLOR DefaultColor = IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff);

    // Raduis.
    static const IZ_FLOAT RadiusDiff;
    static const IZ_FLOAT InnerRadius;
    static const IZ_FLOAT OuterRadius;
    static const IZ_FLOAT MostOuterRadius;

    static const IZ_FLOAT CameraDistance;
    static const IZ_FLOAT CameraRadius;

    // Height.
    static const IZ_FLOAT HeightPerFloor;
    static const IZ_FLOAT SeatHeight;

    // Depth.
    static const IZ_FLOAT Depth;

    static const IZ_FLOAT MaxAngleRate;

    static const IZ_FLOAT ChangeViewDuration;
    static const IZ_FLOAT MoveToItemDuration;

    static const IZ_FLOAT CameraDistanceFromItem;
};

#endif    // #if !defined(__CONFIGURE_H__)