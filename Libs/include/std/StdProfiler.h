#if !defined(__IZANAGI_STD_STD_PROFILER_H__)
#define __IZANAGI_STD_STD_PROFILER_H__

#include "izDefs.h"

namespace izanagi {
	class IMemoryAllocator;

	/**
	 */
	class IStdProfilePrinter {
	protected:
		IStdProfilePrinter() {}
		virtual ~IStdProfilePrinter() {}

		NO_COPIABLE(IStdProfilePrinter);

	public:
		/**
		 */
		virtual void Print(const char* str) = 0;
	};

	/**
	 */
	class IStdProfilerTimer {
	protected:
		IStdProfilerTimer() {}
		virtual ~IStdProfilerTimer() {}

		NO_COPIABLE(IStdProfilerTimer);

	public:
		/** 時間取得.
		 */
		virtual IZ_INT64 GetTime() = 0;

		/** 時間計算.
		 */
		virtual IZ_FLOAT Compute(IZ_INT64 begin, IZ_INT64 end) = 0;
	};

	/** 
	 */
	class CStdProfiler {
		enum {
			STACK_MAX = 64,
		};

	private:
		CStdProfiler();
		~CStdProfiler();

		NO_COPIABLE(CStdProfiler);

	public:
		/** 初期化
		 */
		static IZ_BOOL Init(
			IMemoryAllocator* allocator,
			IZ_UINT maxStackNum,
			IStdProfilerTimer* timer,
			IStdProfilePrinter* printer);

		/** 後始末.
		 */
		static void Release();

		/** 内部データをリセット.
		 */
		static void Reset();

		/** プロファイル結果を出力.
		 */
		static void Dump();

		/** プロファイル開始.
		 */
		static IZ_BOOL Begin(const char* name);

		/** プロファイル終了.
		 */
		static IZ_BOOL End();

		/** プロファイル有効・無効設定.
		 */
		static void Enable(IZ_BOOL enable);
	};
}	// namespace izanagi

/**
 */
#define IZ_BEGIN_PROFILE()	izanagi::CStdProfiler::Begin(__FUNCTION__)

/**
 */
#define IZ_END_PROFILE()	izanagi::CStdProfiler::End()

/**
 */
#define IZ_BEGIN_PROFILE_TAG(tag)	izanagi::CStdProfiler::Begin(tag)

#endif	// #if !defined(__IZANAGI_STD_STD_PROFILER_H__)
