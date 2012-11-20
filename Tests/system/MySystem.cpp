#include "MySystem.h"

CMySystem CMySystem::s_cInstance;

// コンストラクタ
CMySystem::CMySystem()
{
	m_pDevice = IZ_NULL;

	m_pDebugFont = IZ_NULL;
	m_pPad = IZ_NULL;
	m_pKeyboard = IZ_NULL;
}

// デストラクタ
CMySystem::~CMySystem()
{
	SAFE_RELEASE(m_pDebugFont);
	SAFE_RELEASE(m_pPad);
	SAFE_RELEASE(m_pKeyboard);

	SAFE_RELEASE(m_pDevice);
}

// グラフィックスデバイス用
static const IZ_UINT DEVICE_BUF_SIZE = 8 * 1024 * 1024;	// 8M
static IZ_UINT8 DEVICE_BUF[DEVICE_BUF_SIZE];

/**
* 初期化
*/
IZ_BOOL CMySystem::Init(
	IZ_UINT nBufSize,
	IZ_UINT8* pBuf)
{
	IZ_BOOL result = IZ_TRUE;

	// TODO
#if 1
	// アロケータ初期化
	m_cAllocator.Init(nBufSize, pBuf);
#endif

	m_GraphcAllocator.Init(DEVICE_BUF_SIZE, DEVICE_BUF);

	// グラフィックスデバイス作成
	m_pDevice = izanagi::graph::CGraphicsDevice::CreateGrapicsDevice(
					&m_GraphcAllocator);
	if (!(result = (m_pDevice != IZ_NULL))) {
		IZ_ASSERT(IZ_FALSE);
		goto __EXIT__;
	}

__EXIT__:
	if (!result) {
		Release();
	}

	return result;
}

/**
* デバッグフォント初期化
*/
IZ_BOOL CMySystem::InitDebugFont()
{
	IZ_ASSERT(m_pDevice != IZ_NULL);

	IZ_BOOL ret = IZ_TRUE;
	
	m_pDebugFont = izanagi::CDebugFont::CreateDebugFont(
					&m_cAllocator,
					m_pDevice);
	if (m_pDebugFont == IZ_NULL) {
		ret = IZ_FALSE;
		goto __EXIT__;
	}

__EXIT__:

	return ret;
}

/**
* パッド初期化
*/
IZ_BOOL CMySystem::InitPad()
{
	m_pPad = izanagi::CPad::CreatePad(&m_cAllocator);
	VRETURN(m_pPad != IZ_NULL);

	return IZ_TRUE;
}

IZ_BOOL CMySystem::InitKeyboard()
{
	m_pKeyboard = izanagi::CKeyboard::CreateKeyboard(&m_cAllocator);
	VRETURN(m_pKeyboard != IZ_NULL);

	return IZ_TRUE;
}

/**
* 開放
*/
void CMySystem::Release()
{
	SAFE_RELEASE(m_pDebugFont);
	SAFE_RELEASE(m_pPad);
	SAFE_RELEASE(m_pKeyboard);

	SAFE_RELEASE(m_pDevice);

	// アロケータのダンプ
	m_cAllocator.Dump();

	m_GraphcAllocator.Dump();
}
