#if !defined(__IZANAGI_SYSTEM_SYS_WINDOW_H__)
#define __IZANAGI_SYSTEM_SYS_WINDOW_H__

#include "izDefs.h"
#include "izStd.h"

namespace izanagi {
	class CMsgHandlerManager;

	/**
	 */
	typedef void*	WindowHandle;

	/**
	 */
	class CMessageHandler {
		friend class CMsgHandlerManager;

	protected:
		CMessageHandler() {}
		virtual ~CMessageHandler() {}

		NO_COPIABLE(CMessageHandler);

	public:
		virtual void OnKeyDown(IZ_UINT nChar) {}

		virtual void OnKeyUp(IZ_UINT nChar) {}

		virtual void OnMouseLBtnDown() {}

		virtual void OnMouseLBtnUp() {}

		virtual void OnMouseRBtnDown() {}

		virtual void OnMouseRBtnUp() {}

		virtual void OnMouseMove(const CIntPoint& point) {}

		virtual void OnMouseWheel(IZ_INT delta) {}

		virtual void OnPaint() {}

		virtual void OnIdle() {}

	private:
		CStdHash<IZ_UINT64, CMessageHandler, 4>::Item mHashItem;
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
		static void RunLoop(const WindowHandle& handle);

		static void* GetNativeWindowHandle(const WindowHandle& handle);

		static void* GetNativeDisplayHandle(const WindowHandle& handle);
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SYSTEM_SYS_WINDOW_H__)
