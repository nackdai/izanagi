#if !defined(__SHPERE_H__)
#define __SHPERE_H__

#include "izGraph.h"
#include "izMath.h"
#include "izDebugUtil.h"

class CSphere : public izanagi::CObject {
public:
    static CSphere* Create(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        IZ_FLOAT radius,
        IZ_UINT slices,
        IZ_UINT stacks);

private:
    CSphere();
    virtual ~CSphere();

    NO_COPIABLE(CSphere);

    IZ_DEFINE_INTERNAL_RELEASE();

private:
    IZ_BOOL CreateMesh(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        IZ_FLOAT radius,
        IZ_UINT slices,
        IZ_UINT stacks);

    IZ_BOOL CreateTriangles(
        izanagi::IMemoryAllocator* allocator,
        IZ_FLOAT radius,
        IZ_UINT slices,
        IZ_UINT stacks);

public:
    IZ_BOOL Draw(izanagi::graph::CGraphicsDevice* device);

    IZ_BOOL GetCrossPoint(
        const izanagi::math::CRay& ray,
        izanagi::math::SVector4& refPtr,
        izanagi::math::SVector4* normal);

    IZ_UINT GetTriNum() const { return m_TriNum; }
    const izanagi::math::CTriangle* GetTriangles() const { return m_Triangles; }

private:
    izanagi::IMemoryAllocator* m_Allocator;

    izanagi::CDebugMesh* m_Mesh;

    IZ_UINT m_TriNum;
    izanagi::math::CTriangle* m_Triangles;
};

#endif    // #if !defined(__SHPERE_H__)