#if !defined(__STATE_ROTATE_BY_FLING_H__)
#define __STATE_ROTATE_BY_FLING_H__

#include "izSceneGraph.h"
#include "StateBase.h"

class StateRotateByFling : public StateBase {
public:
    StateRotateByFling(izanagi::CVectorCamera& camera);
    virtual ~StateRotateByFling();

public:
    virtual IZ_BOOL Update();

    virtual IZ_BOOL Leave();
};

#endif    // #if !defined(__STATE_ROTATE_BY_FLING_H__)
