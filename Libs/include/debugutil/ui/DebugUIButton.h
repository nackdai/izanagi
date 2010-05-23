#if !defined(__URANUS_DEBUGUTIL_DEBUG_UI_BUTTON_H__)
#define __URANUS_DEBUGUTIL_DEBUG_UI_BUTTON_H__

#include "DebugUIWidget.h"

namespace uranus {
	/**
	* UI�{�^��
	*/
	class CDebugUIButton : public CDebugUIWidget {
	public:
		// �N���b�N���̏����R�[���o�b�N
		typedef void (*CallbackOnClick)();

	protected:
		inline CDebugUIButton();
		virtual ~CDebugUIButton() {}

	private:
		CDebugUIButton(const CDebugUIButton& rhs);
		const CDebugUIButton& operator=(const CDebugUIButton& rhs);

	protected:
		// �`��
		void Render(
			CGraphicsDevice* pDevice,
			UN_INT16 nX, UN_INT16 nY);

		// �͈͓����ǂ���
		UN_BOOL IsInRect(UN_INT nX, UN_INT nY);

		UN_BOOL OnMouseLButtonDown(UN_INT nX, UN_INT nY);
		UN_BOOL OnMouseLButtonUp(UN_INT nX, UN_INT nY);
		UN_BOOL OnMouseMove(UN_INT nX, UN_INT nY);

	public:
		// �{�^�����N���b�N�����Ƃ��̃R�[���o�b�N���Z�b�g����
		inline void SetClickCallback(CallbackOnClick* pCallback);

		// ��������Z�b�g����
		inline void SetText(UN_PCSTR pText);

	public:
		// ��������擾����
		inline UN_PCSTR GetText() const;

	protected:
		enum STATE {
			STATE_NORMAL = 0,
			STATE_PRESS,
		};

	protected:
		// �N���b�N���R�[���o�b�N
		CallbackOnClick* m_pClickCallback;

		// �\��������
		UN_PCSTR m_pText; 

		// ���
		STATE m_nState;
	};

	// inline ***************************

	// �R���X�g���N�^
	CDebugUIButton::CDebugUIButton()
	{
		m_pClickCallback = UN_NULL;
		m_pText = UN_NULL;
		m_nState = STATE_NORMAL;
	}

	/**
	* �{�^�����N���b�N�����Ƃ��̃R�[���o�b�N���Z�b�g����
	*/
	void CDebugUIButton::SetClickCallback(CallbackOnClick* pCallback)
	{
		m_pClickCallback = pCallback;
	}

	/**
	* ��������Z�b�g����
	*/
	void CDebugUIButton::SetText(UN_PCSTR pText)
	{
		m_pText = pText;
	}

	/**
	* ��������擾����
	*/
	UN_PCSTR CDebugUIButton::GetText() const
	{
		return m_pText;
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_DEBUGUTIL_DEBUG_UI_BUTTON_H__)
