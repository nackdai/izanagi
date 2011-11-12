#if !defined(__IZANAGI_SYSTEM_SYS_THREAD_H__)
#define __IZANAGI_SYSTEM_SYS_THREAD_H__

#include "izStd.h"
#include "SysThreadDefs.h"
#include "SysMutex.h"
#include "SysEvent.h"

namespace izanagi {
	class CThreadOp;

	/**
	 */
	class IRunnable {
	protected:
		IRunnable() {}
		virtual ~IRunnable() {}

		NO_COPIABLE(IRunnable);

	public:
		virtual void Run() = 0;
	};

	/** スレッドの名前の型
	 */
	typedef CStdString<char, 7> ThreadName;

	/**
	 */
	class CThread : public IRunnable {
	public:
		/** Get current thread id.
		 */
		static ThreadId GetCurrentThreadId();

		/** Return whethrer two threadid are equal.
		 */
		static IZ_BOOL IsEqualThreadId(
			const ThreadId& t1,
			const ThreadId& t2);

		/** 実行中のスレッドを、指定されたミリ秒数の間、スリープ.
		 */
		void Sleep(IZ_UINT millisec);

	public:
		CThread();
		CThread(ThreadName name);
		CThread(IRunnable* runnable);
		CThread(ThreadName name, IRunnable* runnable);

		CThread(IZ_UINT cpu);
		CThread(IZ_UINT cpu, ThreadName name);
		CThread(IZ_UINT cpu, IRunnable* runnable);
		CThread(IZ_UINT cpu, ThreadName name, IRunnable* runnable);

		virtual ~CThread();

		IZ_DECL_PLACEMENT_NEW();

		NO_COPIABLE(CThread);

	private:
		void Init(
			IZ_UINT cpu,
			ThreadName* name,
			IRunnable* runnable);

	public:
		/** このスレッドの実行を開始.
		 */
		IZ_BOOL Start();

		/** このスレッドが終了するのを待機.
		 */
		void Join();

		/** 現在実行中のスレッドを一時的に休止させ、ほかのスレッドが実行できるようにする.
		 *
		 * 同じプロセッサ上で動いているスレッド間でのみ可能
		 *
		 * NOTE
		 * windowsだとwinbase.hの中で#define Yield()としているため、関数名にYieldが使えない
		 */
		void YieldThread();

		/** 処理実行.
		 *
		 * @return falseを返すとスレッドから抜ける
		 */
		virtual void Run();

		/** スレッドID取得.
		 */
		const ThreadId& GetId() const;

		/** 名前取得.
		 */
		const ThreadName& GetName() const;

		/** 名前取得.
		 */
		const char* GetNameString() const;

		/** 名前設定.
		 */
		void SetName(ThreadName name);

	protected:
		ThreadHandle m_Handle;

		ThreadId m_Id;
		ThreadName m_Name;

		IZ_UINT m_Cpu;
		
		IRunnable* m_Runnable;

		CMutex m_Mutex;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SYSTEM_SYS_THREAD_H__)
