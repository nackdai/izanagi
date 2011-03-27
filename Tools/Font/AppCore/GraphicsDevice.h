#if !defined(__GRAPHICS_DEVICE_H__)
#define __GRAPHICS_DEVICE_H__

#include <d3d9.h>
#include <d3dx9.h>
//#include <dxerr9.h>

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)\
	if (p) {\
		p->Release();\
		p = NULL;\
	}
#endif	// #ifndef SAFE_RELEASE

#ifndef VRETURN
#define VRETURN(b)\
	if (! b) {\
		ASSERT(FALSE);\
		return FALSE;\
	}
#endif	// #ifndef VRETURN

struct SOption;

///////////////////////////////////////////////////////////
// GraphicsDevice簡易管理クラス

class CGraphicsDevice {
protected:
	static CGraphicsDevice s_cInstance;

public:
	static CGraphicsDevice& GetInstance() { return s_cInstance; }

protected:
	CGraphicsDevice();
	~CGraphicsDevice();

	CGraphicsDevice(const CGraphicsDevice& rhs);
	CGraphicsDevice& operator =(const CGraphicsDevice& rhs);

public:
	// 開放
	void Release();

public:
	// 初期化
	BOOL Init(HWND hWnd);

	// テクスチャ作成
	BOOL CreateTexture(
		UINT nWidth, UINT nHeight,
		D3DFORMAT fmt,
		LPDIRECT3DTEXTURE9* pTex,
		BOOL bIsRT = FALSE);

public:
	void BeginScene();
	void EndScene();

	void Clear(DWORD color);

	BOOL SetRenderTarget(LPDIRECT3DSURFACE9 pSurf);

	// レンダーターゲット作成
	BOOL CreateRenderTarget(
		UINT nWidth, UINT nHeight,
		D3DFORMAT fmt,
		LPDIRECT3DSURFACE9* pSurf,
		INT nMSAANumber = 0);

	// フォント作成
	BOOL CreateFont(
		LPD3DXFONT* pFont,
		const SOption& sOption);

public:
	LPDIRECT3DDEVICE9 GetRawInterface() { return m_pDevice; }

protected:
	LPDIRECT3D9 m_pD3D;
	LPDIRECT3DDEVICE9 m_pDevice;

	D3DPRESENT_PARAMETERS m_sPresentParameters;
};

#endif	// #if !defined(__GRAPHICS_DEVICE_H__)