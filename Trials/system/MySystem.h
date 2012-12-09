#if !defined(__IZ_SYSTEM_H__)
#define __IZ_SYSTEM_H__

#include "izStd.h"
#include "izGraph.h"
#include "izIo.h"
#include "izDebugUtil.h"
#include "izSystem.h"

// システム
class CMySystem {
protected:
	static CMySystem s_cInstance;

public:
	static CMySystem& GetInstance() { return s_cInstance; }

protected:
	CMySystem();
	~CMySystem();

public:
	// 初期化
	IZ_BOOL Init(
		IZ_UINT nBufSize,
		IZ_UINT8* pBuf);

	// デバッグフォント初期化
	IZ_BOOL InitDebugFont();

	// パッド初期化
	IZ_BOOL InitPad();
	IZ_BOOL InitKeyboard();

	// 開放
	void Release();

public:
	izanagi::graph::CGraphicsDevice* GetGraphicsDevice() { return m_pDevice; }
	izanagi::IMemoryAllocator* GetMemoryAllocator() { return &m_cAllocator; }

	izanagi::CDebugFont* GetDebugFont() { return m_pDebugFont; }

	izanagi::sys::CPad* GetPad() { return m_pPad; }
	izanagi::sys::CKeyboard* GetKeyboard() { return m_pKeyboard; }

	izanagi::sys::CTimer& GetTimer(IZ_UINT idx);

protected:
	enum {
		TIMER_NUM = 2,
	};

protected:
	izanagi::graph::CGraphicsDevice* m_pDevice;
	
	izanagi::CStandardMemoryAllocator m_cAllocator;
	izanagi::CStandardMemoryAllocator m_GraphcAllocator;

	izanagi::CDebugFont* m_pDebugFont;

	izanagi::sys::CPad* m_pPad;
	izanagi::sys::CKeyboard* m_pKeyboard;

	izanagi::sys::CTimer m_cTimer[TIMER_NUM];
};

/**
* タイマ取得
*/
inline izanagi::sys::CTimer& CMySystem::GetTimer(IZ_UINT idx)
{
	IZ_ASSERT(idx < TIMER_NUM);
	return m_cTimer[idx];
}

#endif	// #if !defined(__IZ_SYSTEM_H__)
