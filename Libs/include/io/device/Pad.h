#if !defined(__URANUS_IO_PAD_H__)
#define __URANUS_IO_PAD_H__

#include "../../unStd.h"
#include "InputDeviceDefs.h"

namespace uranus {
	class CPad : public CObject {
	public:
#if 0
		typedef DIJOYSTATE2 PadState;
#else
		typedef DIJOYSTATE PadState;
#endif

	public:
		// �C���X�^���X�쐬
		static CPad* CreatePad(IMemoryAllocator* pAllocator);

	protected:
		enum {
			ANALOG_STCIK_MAX = 65535,

			ANALOG_STICK_DEAD_ZONE = 1500,	// 15%
		};

	protected:
		// �f�o�C�X�񋓃R�[���o�b�N
		static BOOL CALLBACK EnumGameCtrlCallback(
			const DIDEVICEINSTANCE* pdidInstance,
			void* pContext);

		// �p�b�h�ݒ�R�[���o�b�N
		static BOOL CALLBACK EnumPadAxisCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef);

	protected:
		CPad();
		~CPad();

		CPad(const CPad& rhs);
		const CPad& operator=(const CPad& rhs);

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
		inline UN_BOOL IsPushKey(E_PAD_BUTTON key);

		// �L�[����x�������������ǂ���
		inline UN_BOOL IsPushOneShotKey(E_PAD_BUTTON key);

		// �A�i���O�X�e�B�b�N�ʒu�擾
		inline void GetLeftAnalogStick(UN_FLOAT* x, UN_FLOAT* y);
		inline void GetRightAnalogStick(UN_FLOAT* x, UN_FLOAT* y);

	public:
		inline D_INPUT_DEVICE* GetRawInterface();
		inline HWND GetHWND();

		inline const PadState& GetCurState() const;
		inline const PadState& GetPrevState() const;

	protected:
		IMemoryAllocator* m_pAllocator;

		D_INPUT_DEVICE* m_pPadDevice;
		HWND m_hWnd;

		PadState m_CurState;
		PadState m_PrevState;

		// �X�V�ɐ����������ǂ���
		UN_BOOL m_bSucceedUpdate;
	};

	// inlien *******************************

	/**
	* �L�[�����������Ă��邩
	*/
	UN_BOOL CPad::IsPushKey(E_PAD_BUTTON key)
	{
		UN_BOOL ret = UN_FALSE;
		if (m_bSucceedUpdate) {
			ret = (m_CurState.rgbButtons[key] > 0);
		}
		return ret;
	}

	/**
	* �L�[����x�������������ǂ���
	*/
	UN_BOOL CPad::IsPushOneShotKey(E_PAD_BUTTON key)
	{
		UN_BOOL ret = UN_FALSE;
		if (m_bSucceedUpdate) {
			ret = ((m_CurState.rgbButtons[key] > 0) && (m_PrevState.rgbButtons[key] == 0));
		}
		return ret;
	}

	/**
	* �A�i���O�X�e�B�b�N�ʒu�擾�i���j
	*/
	void CPad::GetLeftAnalogStick(UN_FLOAT* x, UN_FLOAT* y)
	{
		if (x != UN_NULL) {
			*x = (UN_FLOAT)m_CurState.lX / ANALOG_STCIK_MAX;
		}
		if (y != UN_NULL) {
			*y = (UN_FLOAT)m_CurState.lX / ANALOG_STCIK_MAX;
		}
	}

	/**
	* �A�i���O�X�e�B�b�N�ʒu�擾�i�E�j
	*/
	void CPad::GetRightAnalogStick(UN_FLOAT* x, UN_FLOAT* y)
	{
		if (x != UN_NULL) {
			*x = (UN_FLOAT)m_CurState.lZ / ANALOG_STCIK_MAX;
		}
		if (y != UN_NULL) {
			*y = (UN_FLOAT)m_CurState.lRz / ANALOG_STCIK_MAX;
		}
	}

	D_INPUT_DEVICE* CPad::GetRawInterface()
	{
		return m_pPadDevice;
	}

	HWND CPad::GetHWND()
	{
		return m_hWnd;
	}

	const CPad::PadState& CPad::GetCurState() const
	{
		return m_CurState;
	}

	const CPad::PadState& CPad::GetPrevState() const
	{
		return m_PrevState;
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_IO_PAD_H__)
