#if !defined(__IZANAGI_DEBUGUTIL_DEBUG_UI_DIALOG_H__)
#define __IZANAGI_DEBUGUTIL_DEBUG_UI_DIALOG_H__

#include "DebugUIWidget.h"

namespace izanagi {
	/**
	* UIダイアログ
	*/
	class CDebugUIDialog : public CDebugUIWidget {
	protected:
		CDebugUIDialog();
		~CDebugUIDialog();

	private:
		CDebugUIDialog(const CDebugUIDialog& rhs);
		const CDebugUIDialog& operator=(const CDebugUIDialog& rhs);

	protected:
		// 描画
		void Render(CGraphicsDevice* pDevice);
		void Render(
			CGraphicsDevice* pDevice,
			IZ_INT16 nX, IZ_INT16 nY);

	public:
		// 追加
		IZ_BOOL Add(CDebugUIWidget* pWidget);

		// 全削除
		void RemoveAll();

	public:
		// 有効フラグセット
		void SetIsEnable(IZ_BOOL bEnable);

		// 表示フラグセット
		void SetIsShow(IZ_BOOL bIsShow);

	protected:
		CStdList<CDebugUIWidget> m_List;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_DEBUGUTIL_DEBUG_UI_DIALOG_H__)
