#include "io/device/Keyboard.h"

using namespace uranus;

/**
* �C���X�^���X�쐬
*/
CKeyboard* CKeyboard::CreateKeyboard(IMemoryAllocator* pAllocator)
{
	UN_UINT8* pBuf = UN_NULL;
	CKeyboard* pInstance = UN_NULL;
	UN_BOOL result = UN_FALSE;

	// �������m��
	pBuf = (UN_UINT8*)ALLOC_ZERO(pAllocator, sizeof(CKeyboard));
	result = (pBuf != UN_NULL);
	if (!result) {
		UN_ASSERT(UN_FALSE);
		goto __EXIT__;
	}

	// �C���X�^���X�쐬
	pInstance = new(pBuf) CKeyboard;
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
CKeyboard::CKeyboard()
{
	m_pAllocator = UN_NULL;
	m_pKeyDevice = UN_NULL;
	m_hWnd = UN_NULL;

	FILL_ZERO(m_CurState, sizeof(m_CurState));
	FILL_ZERO(m_PrevState, sizeof(m_PrevState));

	m_bSucceedUpdate = UN_FALSE;
}

// �f�X�g���N�^
CKeyboard::~CKeyboard()
{
	// �O�̂���
	if (m_pKeyDevice != UN_NULL) {
		m_pKeyDevice->Unacquire();
	}

	SAFE_RELEASE(m_pKeyDevice);
	m_hWnd = UN_NULL;
}

// ���
void CKeyboard::InternalRelease()
{
	delete this;

	if (m_pAllocator != UN_NULL) {
		m_pAllocator->Free(this);
	}
}

// ������
UN_BOOL CKeyboard::Init(SInputDeviceInitParam* pInitParam)
{
	// �{�̍쐬
	HRESULT hr = pInitParam->pInput->CreateDevice(
					GUID_SysKeyboard,
					&m_pKeyDevice,
					UN_NULL);
	VRETURN(SUCCEEDED(hr));
	
	// �f�o�C�X���L�[�{�[�h�ɐݒ�
	// �f�t�H���g�̐ݒ���g�p
	hr = m_pKeyDevice->SetDataFormat(&c_dfDIKeyboard);
	VRETURN(SUCCEEDED(hr));

	// �������x���̐ݒ�
	hr = m_pKeyDevice->SetCooperativeLevel(
			pInitParam->hWnd,
			DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	VRETURN(SUCCEEDED(hr));

	// �ꉞ�E�E�E
	m_hWnd = pInitParam->hWnd;

	//hr = m_pKeyDevice->Acquire();

	return UN_TRUE;
}

/**
* �X�V
*/
UN_BOOL CKeyboard::Update()
{
	UN_BOOL ret = (m_pKeyDevice != UN_NULL);

	if (ret) {
		// �f�o�C�X�̎擾
		HRESULT hr = m_pKeyDevice->Acquire();

		ret = SUCCEEDED(hr);

		// �O�̏�Ԃ�ێ�
		memcpy(m_PrevState, m_CurState, sizeof(m_PrevState));

		if (ret) {
			m_bSucceedUpdate = UN_TRUE;
			m_pKeyDevice->GetDeviceState(sizeof(m_CurState), m_CurState);
		}
	}

	return ret;
}
