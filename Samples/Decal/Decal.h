#if !defined(__DECAL_H__)
#define __DECAL_H__

#include "izGraph.h"
#include "izMath.h"

#include <vector>

#define ENABLE_STL

class CDecal : public izanagi::CObject
{
public:
    static CDecal* Create(
        izanagi::IMemoryAllocator* allocator,
        const izanagi::math::SVector& point,
        const izanagi::math::SVector& normal,
        IZ_FLOAT rectangleLengthX,
        IZ_FLOAT rectangleLengthZ);

private:
    CDecal();
    virtual ~CDecal();

    NO_COPIABLE(CDecal);

    IZ_DEFINE_INTERNAL_RELEASE();

public:
    void SetRectangle(
        const izanagi::math::SVector& point,
        const izanagi::math::SVector& normal,
        IZ_FLOAT rectangleLengthX,
        IZ_FLOAT rectangleLengthZ);

    void DoScissoring(
        const izanagi::math::CTriangle tri[],
        IZ_UINT triNum);

    void CreateGraphicsObject(izanagi::graph::CGraphicsDevice* device);

    void Draw(izanagi::graph::CGraphicsDevice* device);

    const izanagi::math::SVector& GetCenter() const;

private:
    void ComputeNewTriNumByScissoring(
        const izanagi::math::CTriangle tri[],
        IZ_UINT triNum);

private:
    typedef std::vector<izanagi::math::CTriangle, izanagi::STLMemoryAllocator<izanagi::math::CTriangle> > TriangleVector;

    struct STriListProxy
    {
        TriangleVector triList_0;
        TriangleVector triList_1;

        TriangleVector& Get(IZ_UINT idx)
        {
            if (idx == 0)
            {
                return triList_0;
            }
            else
            {
                return triList_1;
            }
        }

        void Clear()
        {
            triList_0.clear();
            triList_1.clear();
        }
    };

    izanagi::IMemoryAllocator* m_Allocator;

    izanagi::math::CRectangle m_Rectangle;

    enum {
        PLANE_LEFT,
        PLANE_RIGHT,
        PLANE_BOTTOM,
        PLANE_TOP,
        PLANE_FRONT,
        PLANE_BACK,

        PLANE_NUM,
    };

    izanagi::math::CPlane m_Planes[PLANE_NUM];

    IZ_UINT m_TriNum;
    izanagi::math::CTriangle* m_Triangles;

    struct SVtx {
        izanagi::math::SVector point;
        IZ_COLOR color;
        IZ_FLOAT uv[2];
    };

    IZ_BOOL m_NeedCreateGraphicsObject;

    izanagi::graph::CVertexBuffer* m_VB;
    izanagi::graph::CVertexDeclaration* m_VD;
};


#endif    // #if !defined(__DECAL_H__)