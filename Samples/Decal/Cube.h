#if !defined(__CUBE_H__)
#define __CUBE_H__

#include "izGraph.h"
#include "izMath.h"
#include "izDebugUtil.h"

class CCube : public izanagi::CObject {
public:
    static CCube* Create(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        IZ_FLOAT size);

private:
    CCube();
    virtual ~CCube();

    NO_COPIABLE(CCube);

    IZ_DEFINE_INTERNAL_RELEASE();

private:
    IZ_BOOL CreateMesh(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        IZ_FLOAT size);

    IZ_BOOL CreateTriangles(
        izanagi::IMemoryAllocator* allocator,
        IZ_FLOAT size);

public:
    IZ_BOOL Draw();

    IZ_BOOL GetCrossPoint(
        const izanagi::math::CRay& ray,
        izanagi::math::SVector& refPtr,
        izanagi::math::SVector* normal);

    IZ_UINT GetTriNum() const { return m_TriNum; }
    const izanagi::math::CTriangle* GetTriangles() const { return m_Triangles; }

private:
    izanagi::IMemoryAllocator* m_Allocator;

    izanagi::CDebugMesh* m_Mesh;

    IZ_UINT m_TriNum;
    izanagi::math::CTriangle* m_Triangles;
};

#endif    // #if !defined(__CUBE_H__)