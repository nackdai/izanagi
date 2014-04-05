#if !defined(__IZANAGI_DEFS_ANDROID_H__)
#define __IZANAGI_DEFS_ANDROID_H__

#include <jni.h>
#include <sys/time.h>
#include <android/log.h>
#include <android/asset_manager.h>

#ifndef _UNICODE
    #define IZ_VSPRINTF     vsnprintf
    #define IZ_SPRINTF      snprintf
    #define IZ_FPRINTF      fprintf
#endif  // #ifdef _UNICODE

/////////////////////////////////////////////////////////////
// いろいろ便利系

#define IZ_LOG_TAG  "izanagi"

#define IZ_PRINTF(...)   __android_log_print(ANDROID_LOG_VERBOSE, IZ_LOG_TAG, __VA_ARGS__)

#ifdef __IZ_DEBUG__
    #define IZ_ASSERT(b)\
        if (!(b)) {\
            __android_log_assert("", IZ_LOG_TAG, "assert : %s(%d)\n", __FILE__, __LINE__);\
        }
#else
    #define IZ_ASSERT(b)
#endif

typedef timeval IZ_TIME;

#define IzMain(screenWidth, screenHeight) \
    extern "C" {\
        JNIEXPORT int JNICALL Java_izanagi_android_lib_IzanagiProxy_init(JNIEnv* env, jobject obj, jobject assetManager, jint width, jint height);\
    };\
    JNIEXPORT int JNICALL Java_izanagi_android_lib_IzanagiProxy_init(JNIEnv* env, jobject obj, jobject assetManager, jint width, jint height)

#define IzGetSystemDataForMainFunc()  izanagi::IzAndroidJniParam(env, assetManager)
#define IzGetScreenWidth() width
#define IzGetScreenHeight() height

#define IZ_FILE_HANDLE AAsset*

namespace izanagi {
    struct IzAndroidJniParam {
        JNIEnv* env;
        jobject obj;

        IzAndroidJniParam()
        {
            env = NULL;
            obj = NULL;
        }

        IzAndroidJniParam(JNIEnv* e, jobject o)
        {
            env = e;
            obj = o;
        }
    };
}

#define IZ_PLATFORM_PARAM   izanagi::IzAndroidJniParam

#endif  // #if !defined(__IZANAGI_DEFS_ANDROID_H__)
