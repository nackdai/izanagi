#if !defined(__GESTURE_LISTENER_H__)
#define __GESTURE_LISTENER_H__

#include "izSampleKit.h"
#include "izUI.h"

class CGestureListener : public izanagi::ui::CGestureListenerImplement
{
public:
    CGestureListener();
    virtual ~CGestureListener();

public:
    virtual void OnTapUp(const izanagi::sys::CTouchEvent& ev);

    virtual void OnFling(
        const izanagi::sys::CTouchEvent& ev,
        IZ_FLOAT velocityX, IZ_FLOAT velocityY);
};


#endif    // #if !defined(__GESTURE_LISTENER_H__)