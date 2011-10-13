#if !defined(__IZANAGI_SYSTEM_SYS_WINDOW_H__)
#define __IZANAGI_SYSTEM_SYS_WINDOW_H__

#include "izDefs.h"
#include "izStd.h"

namespace izanagi {
	/**
	 */
	typedef void*	WindowHandle;

	/**
	 */
	class CMessageHandler {
	protected:
		CMessageHandler() {}
		virtual ~CMessageHandler() {}

		NO_COPIABLE(CMessageHandler);

	public:
		virtual void OnKeyDonw(IZ_UINT nChar, IZ_UINT repCnt) {}

		virtual void OnMouseMove(const CIntPoint& point) {}

		virtual void OnMouseWheel(IZ_INT delta, const CIntPoint& point) {}

		virtual void OnPaint() {}

		virtual void OnIdle() {}
	};

	/**
	 */
	struct WindowParams {
		IZ_UINT width;				///< ウインドウ幅
		IZ_UINT height;				///< ウインドウ高さ
		IZ_BOOL isWindowed;			///< ウインドウモードにするかどうか
		IZ_PCSTR title;				///< ウインドウタイトル
		void* platformParam;		///< プラットフォーム固有パラメータ
		CMessageHandler* handler;	///< メッセージハンドラ
	};

	/**
	 */
	class CSysWindow {
	private:
		CSysWindow();
		~CSysWindow();

		NO_COPIABLE(CSysWindow);

	public:
		/** ウインドウ作成.
		 */
		static WindowHandle Create(
			IMemoryAllocator* allocator,
			const WindowParams& param);

		/** ウインドウ破棄.
		 */
		static void Destroy(WindowHandle handle);

		/** ループ実行.
		 */
		static void RunLoop();

		static void* GetNativeWindowHandle(const WindowHandle& handle);

		static void* GetNativeDisplayHandle(const WindowHandle& handle);
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SYSTEM_SYS_WINDOW_H__)
