#if !defined(__BG_H__)
#define __BG_H__

#include "izSampleKit.h"

/** Photos's seat.
 */
class BG : public izanagi::CObject {
public:
    // Create an instance.
    static BG* Create(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device);

private:
    BG();
    virtual ~BG();

    NO_COPIABLE(BG);

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

    void SetShaderParam(izanagi::shader::CShaderBasic* shader);

private:
    izanagi::IMemoryAllocator* m_Allocator;

    izanagi::CDebugMeshRectangle* m_BG;
    izanagi::math::SMatrix m_L2W;
};

#endif    // #if !defined(__BG_H__)