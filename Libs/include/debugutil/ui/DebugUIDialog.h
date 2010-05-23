#if !defined(__URANUS_DEBUGUTIL_DEBUG_UI_DIALOG_H__)
#define __URANUS_DEBUGUTIL_DEBUG_UI_DIALOG_H__

#include "DebugUIWidget.h"

namespace uranus {
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
			UN_INT16 nX, UN_INT16 nY);

	public:
		// 追加
		UN_BOOL Add(CDebugUIWidget* pWidget);

		// 全削除
		void RemoveAll();

	public:
		// 有効フラグセット
		void SetIsEnable(UN_BOOL bEnable);

		// 表示フラグセット
		void SetIsShow(UN_BOOL bIsShow);

	protected:
		CStdList<CDebugUIWidget> m_List;
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_DEBUGUTIL_DEBUG_UI_DIALOG_H__)
