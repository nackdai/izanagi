#if !defined(__IZ_APPL_H__)
#define __IZ_APPL_H__

#include "izDefs.h"

#define SCREEN_WIDTH	1280
#define SCREEN_HEIGHT	720

namespace izanagi {
	class CTexture;
}

class CMyAppl {
private:
	static CMyAppl s_cInstance;

public:
	static CMyAppl& GetInstance() { return s_cInstance; }

private:
	CMyAppl();
	~CMyAppl() {}

public:
	// 初期化
	IZ_BOOL Init(
		HWND hDeviceWindow,
		HWND hFocusWindow);

	// 開放
	void Release();

	IZ_BOOL Update();
	IZ_BOOL Render();

protected:
	izanagi::CTexture* m_pRT;
};

#endif	// #if !defined(__IZ_APPL_H__)
