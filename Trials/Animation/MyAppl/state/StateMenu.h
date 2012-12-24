#if !defined(__STATE_MENU_H__)
#define __STATE_MENU_H__

#include "izDefs.h"
#include "izStd.h"
#include "izSceneGraph.h"

class CStateMenu : public izanagi::CSceneStateBase {
public:
    CStateMenu();
    ~CStateMenu();

public:
    IZ_BOOL Init();
    IZ_BOOL Render(izanagi::graph::CGraphicsDevice* device);
    IZ_BOOL Update();
    IZ_BOOL Destroy();
    IZ_BOOL Enter(izanagi::IMemoryAllocator* allocator, void* val);
    IZ_BOOL Leave();

    IZ_BOOL OnKeyDown(IZ_UINT nChar);
    //IZ_BOOL OnMouseMove(IZ_INT x, IZ_INT y);
    //IZ_BOOL OnMouseWheel(IZ_SHORT zDelta); 

protected:
    void Render2D();

protected:
    IZ_INT m_nPos;
    IZ_BOOL m_bDecide;
};

#endif    // #if !defined(__STATE_MENU_H__)