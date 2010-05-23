#if !defined(__URANUS_IO_KEYBOARD_H__)
#define __URANUS_IO_KEYBOARD_H__

#include "../../unStd.h"
#include "InputDeviceDefs.h"

namespace uranus {
	/**
	* �L�[�{�[�h
	*/
	class CKeyboard : public CObject {
	public:
		// �C���X�^���X�쐬
		static CKeyboard* CreateKeyboard(IMemoryAllocator* pAllocator);

	protected:
		CKeyboard();
		~CKeyboard();

		CKeyboard(const CKeyboard& rhs);
		const CKeyboard& operator=(const CKeyboard& rhs);

	protected:
		// ���
		void InternalRelease();

	public:
		// ������
		UN_BOOL Init(SInputDeviceInitParam* pInitParam);

		// �X�V
		UN_BOOL Update();

	public:
		// �L�[�����������Ă��邩
		inline UN_BOOL IsPushKey(E_KEYBOARD_BUTTON key);

		// �L�[����x�������������ǂ���
		inline UN_BOOL IsPushOneShotKey(E_KEYBOARD_BUTTON key);

	public:
		inline D_INPUT_DEVICE* GetRawInterface();
		inline HWND GetHWND();

	protected:
		enum {
			KEY_NUM = 256,
		};

	protected:
		IMemoryAllocator* m_pAllocator;

		D_INPUT_DEVICE* m_pKeyDevice;
		HWND m_hWnd;

		// �L�[�o�b�t�@
		UN_UINT8 m_CurState[KEY_NUM];
		UN_UINT8 m_PrevState[KEY_NUM];

		// �X�V�ɐ����������ǂ���
		UN_BOOL m_bSucceedUpdate;
	};

	// inline *****************************

	/**
	* �L�[�����������Ă��邩
	*/
	UN_BOOL CKeyboard::IsPushKey(E_KEYBOARD_BUTTON key)
	{
		UN_BOOL ret = UN_FALSE;
		if (m_bSucceedUpdate) {
			ret = (m_CurState[key] > 0);
		}
		return ret;
	}

	/**
	* �L�[����x�������������ǂ���
	*/
	UN_BOOL CKeyboard::IsPushOneShotKey(E_KEYBOARD_BUTTON key)
	{
		UN_BOOL ret = UN_FALSE;
		if (m_bSucceedUpdate) {
			ret = ((m_CurState[key] > 0) && (m_PrevState[key] == 0));
		}
		return ret;
	}

	D_INPUT_DEVICE* CKeyboard::GetRawInterface()
	{
		return m_pKeyDevice;
	}

	HWND CKeyboard::GetHWND()
	{
		return m_hWnd;
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_IO_KEYBOARD_H__)
