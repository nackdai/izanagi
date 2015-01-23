#if !defined(__STATE_BASE_H__)
#define __STATE_BASE_H__

#include "izSceneGraph.h"

class StateBase : public izanagi::CSceneStateBase {
public:
    StateBase(izanagi::CVectorCamera& camera);
    virtual ~StateBase() {}

public:
    virtual IZ_BOOL Render(izanagi::graph::CGraphicsDevice* device);

    virtual IZ_BOOL OnMouseLBtnDown(const izanagi::CIntPoint& point);
    virtual IZ_BOOL OnMouseLBtnUp(const izanagi::CIntPoint& point);
    virtual void OnMouseMove(const izanagi::CIntPoint& point);

    izanagi::CVectorCamera& GetCamera();

private:
    izanagi::CVectorCamera& m_Camera;
};

#endif    // #if !defined(__STATE_BASE_H__)
