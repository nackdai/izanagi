#if !defined(__2D_H__)
#define __2D_H__

#include "izSceneGraph.h"
#include "izAnimation.h"

class C2D {
private:
    static C2D instance;

public:
    static C2D& Instance();

private:
    C2D();
    ~C2D();

public:
    IZ_BOOL Init(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device);

    void Terminate();

    void Update(IZ_FLOAT time);

    void Render(izanagi::graph::CGraphicsDevice* device);

private:
    izanagi::CImage* m_Img;
    izanagi::animation::CEasingInterpolator* m_EasingInterp;
    izanagi::animation::CTimeline m_Timeline;
};

#endif    // #if !defined(__2D_H__)
