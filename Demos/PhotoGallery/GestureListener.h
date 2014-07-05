#if !defined(__GESTURE_LISTENER_H__)
#define __GESTURE_LISTENER_H__

#include "izSampleKit.h"
#include "izUI.h"

class PhotoGalleryApp;

/** Listen touch gesture.
 */
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

/** Detect touch gesture.
 */
class GestureDetector
{
private:
    static GestureDetector s_Instance;

public:
    static GestureDetector& Instance();

private:
    GestureDetector();
    ~GestureDetector();

public:
    void Init(izanagi::IMemoryAllocator* allocator);

    void Update();

    IZ_BOOL PostTouchEvent(const izanagi::sys::CTouchEvent& ev);

private:
    izanagi::ui::CGestureDetector m_Detector;
    CGestureListener m_Listener;
};

#endif    // #if !defined(__GESTURE_LISTENER_H__)