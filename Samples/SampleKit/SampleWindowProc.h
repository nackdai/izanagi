#if !defined(__IZ_SAMPLE_SAMPLE_WINDOW_H__)
#define __IZ_SAMPLE_SAMPLE_WINDOW_H__

#include "izSystem.h"

namespace izanagi {
namespace sample {
	class CSampleApp;

	/** サンプルウインドウメッセージハンドラ.
	 */
	class CSampleWndProc : public izanagi::CMessageHandler {
		friend class CSampleApp;

	public:
		CSampleWndProc();
		virtual ~CSampleWndProc();

	private:
		void Init(CSampleApp* app);

	public:
		virtual void OnKeyDown(IZ_UINT nChar);

		virtual void OnMouseLBtnDown();
		virtual void OnMouseLBtnUp();
		virtual void OnMouseRBtnDown();
		virtual void OnMouseRBtnUp();
		virtual void OnMouseMove(const izanagi::CIntPoint& point);
		virtual void OnMouseWheel(IZ_INT delta);

		virtual void OnPaint();

		virtual void OnIdle();

	private:
		CSampleApp* m_App;

		izanagi::CIntPoint m_PrevPoint;

		struct {
			IZ_UINT onLBtn	: 1;
			IZ_UINT onRBtn	: 1;
		} m_Flags;
	};
}
}

#endif	// #if !defined(__IZ_SAMPLE_SAMPLE_WINDOW_H__)
