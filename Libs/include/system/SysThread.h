#if !defined(__IZANAGI_SYSTEM_SYS_THREAD_H__)
#define __IZANAGI_SYSTEM_SYS_THREAD_H__

#include <thread>
#include <atomic>

#include "izStd.h"

namespace izanagi
{
namespace sys
{
    /** スレッドの名前の型
     */
    typedef CStdString<char, 7> ThreadName;

    /**
     */
    class CThread {
    public:
        /** 実行中のスレッドを、指定されたミリ秒数の間、スリープ.
         */
        static void Sleep(IZ_UINT millisec);

        /** 現在実行中のスレッドを一時的に休止させ、ほかのスレッドが実行できるようにする.
         *
         * 同じプロセッサ上で動いているスレッド間でのみ可能
         *
         * NOTE
         * windowsだとwinbase.hの中で#define Yield()としているため、関数名にYieldが使えない
         */
        static void YieldThread();

    public:
        CThread();
        CThread(const ThreadName& name);

        virtual ~CThread();

        IZ_DECL_PLACEMENT_NEW();

        NO_COPIABLE(CThread);

    public:
		virtual void Init(
			const ThreadName& name,
			std::function<void(void*)> func,
			void* userData);

		virtual void Init(
			std::function<void(void*)> func,
			void* userData);

    public:
        /** このスレッドの実行を開始.
         */
        virtual IZ_BOOL Start(IMemoryAllocator* allocator);

        /** スレッド実行中かどうかを取得.
         */
        IZ_BOOL IsRunning();

		virtual void Run();

        /** このスレッドが終了するのを待機.
         */
		virtual void Join();

        /** 名前取得.
         */
        const ThreadName& GetName() const;

    protected:
        IZ_INT m_ThreadResult;

		IMemoryAllocator* m_allocator;

		std::thread* m_thread;

        ThreadName m_Name;

		std::function<void(void*)> m_func;
		void* m_userData;
    };
}   // namespace sys
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SYSTEM_SYS_THREAD_H__)
