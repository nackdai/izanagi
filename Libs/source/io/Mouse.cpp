#include "io/device/Mouse.h"

using namespace uranus;

/**
* �C���X�^���X�쐬
*/
CMouse* CMouse::CreateMouse(IMemoryAllocator* pAllocator)
{
	UN_UINT8* pBuf = UN_NULL;
	CMouse* pInstance = UN_NULL;
	UN_BOOL result = UN_FALSE;

	// �������m��
	pBuf = (UN_UINT8*)ALLOC_ZERO(pAllocator, sizeof(CMouse));
	result = (pBuf != UN_NULL);
	if (!result) {
		UN_ASSERT(UN_FALSE);
		goto __EXIT__;
	}

	// �C���X�^���X�쐬
	pInstance = new(pBuf) CMouse;
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

// �R���X�g���N�^
CMouse::CMouse()
{
	m_pAllocator = UN_NULL;
	m_pMouseDevice = UN_NULL;
	m_hWnd = UN_NULL;

	FILL_ZERO(&m_MouseState, sizeof(m_MouseState));

	m_bSucceedUpdate = UN_FALSE;
}

// �f�X�g���N�^
CMouse::~CMouse()
{
	// �O�̂���
	if (m_pMouseDevice != UN_NULL) {
		m_pMouseDevice->Unacquire();
	}

	SAFE_RELEASE(m_pMouseDevice);
	m_hWnd = UN_NULL;
}

// ���
void CMouse::InternalRelease()
{
	delete this;

	if (m_pAllocator != UN_NULL) {
		m_pAllocator->Free(this);
	}
}

/**
* ������
*/
UN_BOOL CMouse::Init(SInputDeviceInitParam* pInitParam)
{
	// �{�̍쐬
	HRESULT hr = pInitParam->pInput->CreateDevice(
					GUID_SysMouse,
					&m_pMouseDevice,
					UN_NULL);
	VRETURN(SUCCEEDED(hr));
	
	// �f�o�C�X���}�E�X�ɐݒ�
	// �f�t�H���g�̐ݒ���g�p
	hr = m_pMouseDevice->SetDataFormat(&c_dfDIMouse2);
	VRETURN(SUCCEEDED(hr));

	// �������x���̐ݒ�
	hr = m_pMouseDevice->SetCooperativeLevel(
			pInitParam->hWnd,
			DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	VRETURN(SUCCEEDED(hr));

	// �����[�h�̐ݒ�
	DIPROPDWORD diprop;
	{
		diprop.diph.dwSize = sizeof(DIPROPDWORD);
		diprop.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		diprop.diph.dwObj = 0;
		diprop.diph.dwHow = DIPH_DEVICE;

#if 1
		// ���Έʒu���[�h
		diprop.dwData = DIPROPAXISMODE_REL;
#else
		// ��Έʒu���[�h
		diprop.dwData = DIPROPAXISMODE_ABS;
#endif
	}
	hr = m_pMouseDevice->SetProperty(
			DIPROP_AXISMODE,
			&diprop.diph);
	VRETURN(SUCCEEDED(hr));

	// �ꉞ�E�E�E
	m_hWnd = pInitParam->hWnd;

	//hr = m_pMouseDevice->Acquire();

	return UN_TRUE;
}

/**
* �X�V
*/
UN_BOOL CMouse::Update()
{
	UN_BOOL ret = (m_pMouseDevice != UN_NULL);

	if (ret) {
		// �f�o�C�X�̎擾
		HRESULT hr = m_pMouseDevice->Acquire();

		ret = SUCCEEDED(hr);

		if (ret) {
			m_bSucceedUpdate = UN_TRUE;
			m_pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &m_MouseState);
		}
	}

	return ret;
}
