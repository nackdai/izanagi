#if !defined(__CONFIGURE_H__)
#define __CONFIGURE_H__

#include "izSampleKit.h"

/** Configuration parameters.
 */
class Configure {
public:
    /** Screen width.
     */
    static const IZ_UINT SCREEN_WIDTH = 1280;

    /** Screen height.
     */
    static const IZ_UINT SCREEN_HEIGHT = 720;

    /** Number of photos.
     */
    static const IZ_UINT PHOTOS = 90;

    /** Flags for vertex semantics.
     */
    static const IZ_UINT MeshFlags = izanagi::E_DEBUG_MESH_VTX_FORM_POS
        | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR
        | izanagi::E_DEBUG_MESH_VTX_FORM_NORMAL;

    /** Slices of seat.
     */
    static const IZ_UINT Slices = 20;

    /** Default color.
     */
    static const IZ_COLOR DefaultColor = IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff);

    /** Seat color.
     */
    static const IZ_COLOR SeatColor = IZ_COLOR_RGBA(0x20, 0x20, 0x20, 0xff);

    /** Background color.
     */
    static const IZ_COLOR BGColor = IZ_COLOR_RGBA(0x20, 0x20, 0x20, 0xff);

    /** Different of each radius..
     */
    static const IZ_FLOAT RadiusDiff;

    /** Most inner radius of seat.
     */
    static const IZ_FLOAT InnerRadius;

    /** Middle radius of seat.
     */
    static const IZ_FLOAT MidRadius;

    /** Most outer radius of seat.
     */
    static const IZ_FLOAT OuterRadius;

    /** Distance between camera and photos.
     */
    static const IZ_FLOAT CameraDistance;

    /** Height per seat floor.
     */
    static const IZ_FLOAT HeightPerFloor;

    /** Seat height.
     */
    static const IZ_FLOAT SeatHeight;

    /** Depth of photo item.
     */
    static const IZ_FLOAT Depth;

    /** Max angle rate by fling.
     */
    static const IZ_FLOAT MaxAngleRate;

    /** Duration for chnaging view animation.
     */
    static const IZ_FLOAT ChangeViewDuration;

    /** Duration for moving to photo item animation.
     */
    static const IZ_FLOAT MoveToItemDuration;

    /** Duration for fading in photo item animation.
     */
    static const IZ_FLOAT FadeInDuration;

    /** Distance between camera and photo item when camemra move front of photo item.
     */
    static const IZ_FLOAT CameraDistanceFromItem;

    /** Max texture size.
     */
    static const IZ_UINT MaxTextureSize = 1280;

    /** Starting height position when fading in animation start.
     */
    static const IZ_FLOAT FadeInHeight;

    /** Range if photo item is shown.
     */
    static const IZ_FLOAT PhotoItemShownRange;
};

#endif    // #if !defined(__CONFIGURE_H__)