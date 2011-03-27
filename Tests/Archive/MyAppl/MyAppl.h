#if !defined(__IZ_APPL_H__)
#define __IZ_APPL_H__

#include "izDefs.h"

#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480

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
		HINSTANCE hInst,
		HWND hDeviceWindow,
		HWND hFocusWindow);

	// 開放
	void Release();

	IZ_BOOL Update();
	IZ_BOOL Render();
};

#endif	// #if !defined(__IZ_APPL_H__)
