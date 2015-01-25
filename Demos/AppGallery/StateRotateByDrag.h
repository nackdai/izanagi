#if !defined(__STATE_ROTATE_BY_DRAG_H__)
#define __STATE_ROTATE_BY_DRAG_H__

#include "izSceneGraph.h"
#include "StateBase.h"

class StateRotateByDrag : public StateBase {
public:
    StateRotateByDrag(izanagi::CVectorCamera& camera);
    virtual ~StateRotateByDrag();
};

#endif    // #if !defined(__STATE_ROTATE_BY_DRAG_H__)
