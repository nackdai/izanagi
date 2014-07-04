#if !defined(__STATE_BASE_H__)
#define __STATE_BASE_H__

#include "izSceneGraph.h"

class StateBase : public izanagi::CSceneStateBase {
public:
    StateBase(izanagi::CCamera& camera);
    virtual ~StateBase() {}

public:
    virtual IZ_BOOL Render(izanagi::graph::CGraphicsDevice* device);

protected:
    izanagi::CCamera& m_Camera;
};

#endif    // #if !defined(__STATE_BASE_H__)
