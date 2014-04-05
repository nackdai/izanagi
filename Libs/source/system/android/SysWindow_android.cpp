#include <android/asset_manager_jni.h>
#include "system/SysWindow.h"

namespace izanagi
{
namespace sys
{
    // ウインドのもろもろを保持しておく用
    class CWindowAndroid : public CPlacementNew {
    public:
        static CWindowAndroid* s_Instance;

    public:
        // インスタンス作成
        static CWindowAndroid* Create(
            IMemoryAllocator* allocator,
            CMessageHandler* handler);

        // インスタンス破棄
        static void Destroy(CWindowAndroid* window);

    protected:
        CWindowAndroid() {}
        ~CWindowAndroid() {}

    public:
        CMessageHandler* GetHandler() { return m_MsgHandler; }

        IZ_BOOL InitAsset(const WindowParams& param);
        inline AAssetManager* GetAssetManagaer();

    private:
        IMemoryAllocator* m_Allocator;
        CMessageHandler* m_MsgHandler;

        AAssetManager* m_AndroidAssetMgr;
    };

    CWindowAndroid* CWindowAndroid::s_Instance = IZ_NULL;

    // インスタンス作成
    CWindowAndroid* CWindowAndroid::Create(
        IMemoryAllocator* allocator,
        CMessageHandler* handler)
    {
        if (s_Instance == IZ_NULL) {
            void* buf = ALLOC(allocator, sizeof(CWindowAndroid));
            VRETURN_NULL(buf != IZ_NULL);

            CWindowAndroid* window = new(buf) CWindowAndroid();

            window->m_Allocator = allocator;
            window->m_MsgHandler = handler;

            s_Instance = window;
        }

        return s_Instance;
    }

    // インスタンス破棄
    void CWindowAndroid::Destroy(CWindowAndroid* window)
    {
        IMemoryAllocator* allocator = window->m_Allocator;

        delete window;
        FREE(allocator, window);

        s_Instance = IZ_NULL;
    }

    IZ_BOOL CWindowAndroid::InitAsset(const WindowParams& param)
    {
        WindowParams& windowParam = const_cast<WindowParams&>(param);

        IzAndroidJniParam* androidParam = (IzAndroidJniParam*)windowParam.platformParam;
        m_AndroidAssetMgr = AAssetManager_fromJava(androidParam->env, (jobject)androidParam->obj);

        IZ_ASSERT(m_AndroidAssetMgr != IZ_NULL);

        return (m_AndroidAssetMgr != IZ_NULL);
    }

    AAssetManager* CWindowAndroid::GetAssetManagaer()
    {
        return m_AndroidAssetMgr;
    }

    ///////////////////////////////////////////////////////////////////

    E_KEYBOARD_BUTTON CSysWindow::GetKeyMap(IZ_UINT key)
    {
        IZ_ASSERT(IZ_FALSE);
        return E_KEYBOARD_BUTTON_UNDEFINED;
    }

    WindowHandle CSysWindow::Create(
        IMemoryAllocator* allocator,
        const WindowParams& param)
    {
        CWindowAndroid* window = CWindowAndroid::Create(allocator, param.handler);
        VRETURN_NULL(window != IZ_NULL);

        VRETURN_NULL(window->InitAsset(param));

        if (param.handler) {
            param.handler->OnInit(IZ_NULL);
        }

        return window;
    }

    // ウインドウ破棄.
    void CSysWindow::Destroy(WindowHandle handle)
    {
        // Nothing...
    }

    // ループ実行.
    void CSysWindow::RunLoop(WindowHandle handle)
    {
        // Nothing...
    }

    void* CSysWindow::GetNativeWindowHandle(const WindowHandle& handle)
    {
        return IZ_NULL;
    }

    void* CSysWindow::GetNativeDisplayHandle(const WindowHandle& handle)
    {
        return IZ_NULL;
    }

    ///////////////////////////////////////////////////////////////////

    extern "C" {
        JNIEXPORT void JNICALL Java_izanagi_android_lib_IzanagiProxy_runLoop(JNIEnv * env, jobject obj);
        JNIEXPORT void JNICALL Java_izanagi_android_lib_IzanagiProxy_destroy(JNIEnv * env, jobject obj);
    };

    JNIEXPORT void JNICALL Java_izanagi_android_lib_IzanagiProxy_runLoop(JNIEnv * env, jobject obj)
    {
        IZ_ASSERT(CWindowAndroid::s_Instance != IZ_NULL);
        CWindowAndroid::s_Instance->GetHandler()->OnIdle();
    }

    JNIEXPORT void JNICALL Java_izanagi_android_lib_IzanagiProxy_destroy(JNIEnv * env, jobject obj)
    {
        IZ_ASSERT(CWindowAndroid::s_Instance != IZ_NULL);

        CMessageHandler* handler = CWindowAndroid::s_Instance->GetHandler();

        handler->OnTerminate();
        
        CWindowAndroid::Destroy(CWindowAndroid::s_Instance);

        handler->OnDestroy();
    }

}   // namespace sys
}   // namespace izanagi

AAssetManager* GetAssetManager()
{
    IZ_ASSERT(izanagi::sys::CWindowAndroid::s_Instance != IZ_NULL);
    return izanagi::sys::CWindowAndroid::s_Instance->GetAssetManagaer();
}
