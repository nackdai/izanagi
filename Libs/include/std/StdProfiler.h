#if !defined(__IZANAGI_STD_STD_PROFILER_H__)
#define __IZANAGI_STD_STD_PROFILER_H__

#include "izDefs.h"
#include "std/collection//StdList.h"

namespace izanagi {
    class IMemoryAllocator;

    /**
     */
    class IStdProfileWriter {
    protected:
        IStdProfileWriter() {}
        virtual ~IStdProfileWriter() {}

        NO_COPIABLE(IStdProfileWriter);

    public:
        /**
         */
        virtual void Write(const char* format, ...) const = 0;
    };

    /**
     */
    class IStdProfilerTimer {
    protected:
        IStdProfilerTimer() {}
        virtual ~IStdProfilerTimer() {}

        NO_COPIABLE(IStdProfilerTimer);

    public:
        /** 計測開始.
         */
        virtual void Begin() = 0;

        /** 経過時間取得.
         */
        virtual IZ_FLOAT GetElapsedTime() = 0;
    };

    /** 
     */
    class CStdProfiler {
    private:
        CStdProfiler();
        ~CStdProfiler();

        NO_COPIABLE(CStdProfiler);

    public:
        /** 初期化
         */
        static void Init(
            IMemoryAllocator* allocator,
            IStdProfilerTimer* timer,
            const IStdProfileWriter* writer);

        /** 後始末.
         */
        static void Terminate();

        /** プロファイル開始.
         */
        static void Begin(const char* tag);

        /** プロファイル終了.
         */
        static void End();

        /** プロファイル有効・無効設定.
         */
        static void Enable(IZ_BOOL enable);

    private:
        class CProfileItem;

        typedef CStdList<CProfileItem>::Item CListItem;

        static IZ_BOOL s_Enable;

        static CStdList<CProfileItem> s_ItemList;

        static IMemoryAllocator* s_Allocator;
        static IStdProfilerTimer* s_Timer;
        static const IStdProfileWriter* s_Writer;

        static CProfileItem* s_Current;

        static IZ_INT s_Depth;
    };
}   // namespace izanagi

/**
 */
#define IZ_BEGIN_PROFILE()  izanagi::CStdProfiler::Begin(__FUNCTION__)

/**
 */
#define IZ_END_PROFILE()    izanagi::CStdProfiler::End()

/**
 */
#define IZ_BEGIN_PROFILE_TAG(tag)   izanagi::CStdProfiler::Begin(tag)

#endif  // #if !defined(__IZANAGI_STD_STD_PROFILER_H__)
