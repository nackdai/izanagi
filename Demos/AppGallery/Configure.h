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

    static const IZ_FLOAT ItemWidth;
    static const IZ_FLOAT ItemHeight;
    static const IZ_FLOAT ItemDepth;

    static const IZ_FLOAT ItemBoardMargin;

    static const IZ_FLOAT DefaultBevelR;

    /** Flags for vertex semantics.
     */
    static const IZ_UINT BoxMeshFlags;
    static const IZ_UINT BoardMeshFlags;

    /** Default color.
     */
    static const IZ_COLOR DefaultColor;

    /** Max angle rate by fling.
     */
    static const IZ_FLOAT MaxAngleRate;

    static const IZ_FLOAT Gradient;
};

#endif    // #if !defined(__CONFIGURE_H__)