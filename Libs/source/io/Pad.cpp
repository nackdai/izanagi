#include "io/device/Pad.h"

using namespace uranus;

/**
* �C���X�^���X�쐬
*/
CPad* CPad::CreatePad(IMemoryAllocator* pAllocator)
{
	UN_UINT8* pBuf = UN_NULL;
	CPad* pInstance = UN_NULL;
	UN_BOOL result = UN_FALSE;

	// �������m��
	pBuf = (UN_UINT8*)ALLOC_ZERO(pAllocator, sizeof(CPad));
	result = (pBuf != UN_NULL);
	if (!result) {
		UN_ASSERT(UN_FALSE);
		goto __EXIT__;
	}

	// �C���X�^���X�쐬
	pInstance = new(pBuf) CPad;
	{
		pInstance->AddRef();
		pInstance->m_pAllocator = pAllocator;
	}

__EXIT__:
	if (!result) {
		if (pInstance != UN_NULL) {
			SAFE_RELEASE(pInstance);
		}
		else {
			pAllocator->Free(pBuf);
		}
	}
	return pInstance;
}

namespace {
	D_INPUT* g_pInputTmp = UN_NULL;
	D_INPUT_DEVICE* g_pDevTmp = UN_NULL;
}	// namespace 

// TODO
// �f�o�C�X�񋓃R�[���o�b�N
BOOL CALLBACK CPad::EnumGameCtrlCallback(
	const DIDEVICEINSTANCE* pdidInstance,
	void* pContext)
{
	HRESULT hr;

	// �Q�[���X�e�B�b�N�f�o�C�X���쐬����
	hr = g_pInputTmp->CreateDevice(
			pdidInstance->guidInstance,
			&g_pDevTmp,
			NULL);

	if(FAILED(hr)){
		// �f�o�C�X�̍쐬�Ɏ��s������񋓂𑱂���
		return DIENUM_CONTINUE;
	}

	return DIENUM_STOP;
}

// �p�b�h�ݒ�R�[���o�b�N
BOOL CALLBACK CPad::EnumPadAxisCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef)
{
	HRESULT hr;
	
	// ���͈̔�
	{
		DIPROPRANGE prop; 

		prop.diph.dwSize = sizeof(DIPROPRANGE); 
		prop.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
		prop.diph.dwHow = DIPH_BYID; 
		prop.diph.dwObj = lpddoi->dwType; 
		prop.lMin = -ANALOG_STCIK_MAX; 
		prop.lMax = ANALOG_STCIK_MAX; 

		hr = g_pDevTmp->SetProperty(DIPROP_RANGE, &prop.diph);
		if(FAILED(hr)){
			return DIENUM_STOP;
		}
	}

	// �f�b�h�]�[��
	{
		DIPROPDWORD prop;

		prop.diph.dwSize = sizeof(DIPROPDWORD); 
		prop.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
		prop.diph.dwHow = DIPH_BYID; 
		prop.diph.dwObj = lpddoi->dwType; 
		prop.dwData = ANALOG_STICK_DEAD_ZONE;

		hr = g_pDevTmp->SetProperty(DIPROP_DEADZONE, &prop.diph);
		if(FAILED(hr)){
			return DIENUM_STOP;
		}
	}

	// ���̃��[�h
	{
		DIPROPDWORD prop;

		prop.diph.dwSize = sizeof(DIPROPDWORD);
		prop.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		prop.diph.dwObj = 0;
		prop.diph.dwHow = DIPH_DEVICE;

		// ��Έʒu���[�h
		prop.dwData = DIPROPAXISMODE_ABS;

		hr = g_pDevTmp->SetProperty(DIPROP_AXISMODE, &prop.diph);
		if(FAILED(hr)){
			return DIENUM_STOP;
		}
	}

	return DIENUM_CONTINUE;
}

// �R���X�g���N�^
CPad::CPad()
{
	m_pAllocator = UN_NULL;
	m_pPadDevice = UN_NULL;
	m_hWnd = UN_NULL;

	FILL_ZERO(&m_CurState, sizeof(m_CurState));
	FILL_ZERO(&m_PrevState, sizeof(m_PrevState));

	m_bSucceedUpdate = UN_FALSE;
}

// �f�X�g���N�^
CPad::~CPad()
{
	// �O�̂���
	if (m_pPadDevice != UN_NULL) {
		m_pPadDevice->Unacquire();
	}

	SAFE_RELEASE(m_pPadDevice);
	m_hWnd = UN_NULL;
}

// ���
void CPad::InternalRelease()
{
	delete this;

	if (m_pAllocator != UN_NULL) {
		m_pAllocator->Free(this);
	}
}

/**
* ������
*/
UN_BOOL CPad::Init(SInputDeviceInitParam* pInitParam)
{
	g_pInputTmp = pInitParam->pInput;

	HRESULT hr;

	// ���p�\�ȃp�b�h�̗񋓊֐������s
	hr = pInitParam->pInput->EnumDevices(
			DI8DEVCLASS_GAMECTRL,
			EnumGameCtrlCallback,
			NULL,
			DIEDFL_ATTACHEDONLY);
	VRETURN(SUCCEEDED(hr));

	m_pPadDevice = g_pDevTmp;
	VRETURN(m_pPadDevice != UN_NULL);
	
	// �f�o�C�X���p�b�h�ɐݒ�
	// �f�t�H���g�̐ݒ���g�p
#if 0
	hr = m_pPadDevice->SetDataFormat(&c_dfDIJoystick2);
#else
	hr = m_pPadDevice->SetDataFormat(&c_dfDIJoystick);
#endif
	VRETURN(SUCCEEDED(hr));

	// �������x���̐ݒ�
	hr = m_pPadDevice->SetCooperativeLevel(
			pInitParam->hWnd,
			DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	VRETURN(SUCCEEDED(hr));

	// �p�b�h�̃v���p�e�B��񋓂��Đݒ�

	// �A�i���O�X�e�B�b�N�ݒ�
	hr = m_pPadDevice->EnumObjects(
			EnumPadAxisCallback,
			(void*)pInitParam->hWnd,
			DIDFT_AXIS);
	VRETURN(SUCCEEDED(hr));

	// �ꉞ�E�E�E
	m_hWnd = pInitParam->hWnd;

	return UN_TRUE;
}

/**
* �X�V
*/
UN_BOOL CPad::Update()
{
	UN_BOOL ret = (m_pPadDevice != UN_NULL);

	if (ret) {
		// �f�o�C�X�̎擾
		HRESULT hr = m_pPadDevice->Acquire();

		ret = SUCCEEDED(hr);

		// �O�̏�Ԃ�ێ�
		memcpy(&m_PrevState, &m_CurState, sizeof(m_CurState));

		if (ret) {
			m_bSucceedUpdate = UN_TRUE;
			m_pPadDevice->GetDeviceState(sizeof(m_CurState), &m_CurState);

#if 1
			// ���L�[�̏�Ԃ� POV -> Button �Ɉڂ�
			switch (m_CurState.rgdwPOV[0]) {
			case 0:
				m_CurState.rgbButtons[E_PAD_BUTTON_L_UP] = 128;
				break;
			case 4500:
				m_CurState.rgbButtons[E_PAD_BUTTON_L_UP] = 128;
				m_CurState.rgbButtons[E_PAD_BUTTON_L_RIGHT] = 128;
				break;
			case 9000:
				m_CurState.rgbButtons[E_PAD_BUTTON_L_RIGHT] = 128;
				break;
			case 13500:
				m_CurState.rgbButtons[E_PAD_BUTTON_L_RIGHT] = 128;
				m_CurState.rgbButtons[E_PAD_BUTTON_L_DOWN] = 128;
				break;
			case 18000:
				m_CurState.rgbButtons[E_PAD_BUTTON_L_DOWN] = 128;
				break;
			case 22500:
				m_CurState.rgbButtons[E_PAD_BUTTON_L_DOWN] = 128;
				m_CurState.rgbButtons[E_PAD_BUTTON_L_LEFT] = 128;
				break;
			case 27000:
				m_CurState.rgbButtons[E_PAD_BUTTON_L_LEFT] = 128;
				break;
			case 31500:
				m_CurState.rgbButtons[E_PAD_BUTTON_L_LEFT] = 128;
				m_CurState.rgbButtons[E_PAD_BUTTON_L_UP] = 128;
				break;
			}
#endif
		}
		else {
			FILL_ZERO(&m_CurState, sizeof(m_CurState));
		}
	}

	return ret;
}
