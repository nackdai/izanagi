#if !defined(__SEAT_H__)
#define __SEAT_H__

#include "izSampleKit.h"

/** Photos's seat.
 */
class Seat : public izanagi::CObject {
public:
    // Create an instance.
    static Seat* Create(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device);

private:
    Seat();
    virtual ~Seat();

    NO_COPIABLE(Seat);

    IZ_DEFINE_INTERNAL_RELEASE();

private:
    // Initialization.
    IZ_BOOL Init(
        izanagi::IMemoryAllocator* allocator, 
        izanagi::graph::CGraphicsDevice* device);

public:
    /** Render seat.
     */
    void Render(izanagi::graph::CGraphicsDevice* device);

private:
    // Parts of photo's seat.
    class SeatPart;

private:
    izanagi::IMemoryAllocator* m_Allocator;

    izanagi::CDebugMeshRectangle* m_Floor;
    SeatPart* m_FlontSeat;
    SeatPart* m_RearSeat;
};

#endif    // #if !defined(__SEAT_H__)