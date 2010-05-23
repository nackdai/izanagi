#if !defined(__URANUS_DEBUGUTIL_DEBUG_UI_DIALOG_H__)
#define __URANUS_DEBUGUTIL_DEBUG_UI_DIALOG_H__

#include "DebugUIWidget.h"

namespace uranus {
	/**
	* UI�_�C�A���O
	*/
	class CDebugUIDialog : public CDebugUIWidget {
	protected:
		CDebugUIDialog();
		~CDebugUIDialog();

	private:
		CDebugUIDialog(const CDebugUIDialog& rhs);
		const CDebugUIDialog& operator=(const CDebugUIDialog& rhs);

	protected:
		// �`��
		void Render(CGraphicsDevice* pDevice);
		void Render(
			CGraphicsDevice* pDevice,
			UN_INT16 nX, UN_INT16 nY);

	public:
		// �ǉ�
		UN_BOOL Add(CDebugUIWidget* pWidget);

		// �S�폜
		void RemoveAll();

	public:
		// �L���t���O�Z�b�g
		void SetIsEnable(UN_BOOL bEnable);

		// �\���t���O�Z�b�g
		void SetIsShow(UN_BOOL bIsShow);

	protected:
		CStdList<CDebugUIWidget> m_List;
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_DEBUGUTIL_DEBUG_UI_DIALOG_H__)
