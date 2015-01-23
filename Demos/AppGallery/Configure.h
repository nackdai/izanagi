#if !defined(__CONFIGURE_H__)
#define __CONFIGURE_H__

#include "izSampleKit.h"

/** Configuration parameters.
 */
class Configure {
public:
    /** Screen width.
     */
    static const IZ_UINT SCREEN_WIDTH;

    /** Screen height.
     */
    static const IZ_UINT SCREEN_HEIGHT;

    static const IZ_UINT RING_SLICES;

    static const IZ_FLOAT INNER_RADIUS;
    static const IZ_FLOAT OUTER_RADIUS;

    static const IZ_FLOAT RING_HEIGHT;

    static const IZ_COLOR RING_COLOR;

    static const IZ_FLOAT Radius;

    /** Number of itemss.
     */
    static const IZ_UINT ItemNum;

    /** Flags for vertex semantics.
     */
    static const IZ_UINT MeshFlags;

    /** Default color.
     */
    static const IZ_COLOR DefaultColor;

    /** Side and top face color of photo item.
     */
    static const IZ_COLOR PhotoItemSideAndTopFaceColor;

    /** Seat color.
     */
    static const IZ_COLOR SeatColor;

    /** Background color.
     */
    static const IZ_COLOR BGColor;

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
    static const IZ_UINT MaxTextureSize;

    /** Starting height position when fading in animation start.
     */
    static const IZ_FLOAT FadeInHeight;

    /** Range if photo item is shown.
     */
    static const IZ_FLOAT PhotoItemShownRange;
};

#endif    // #if !defined(__CONFIGURE_H__)