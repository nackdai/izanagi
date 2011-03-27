#if !defined(__IZANAGI_TOOL_KIT_GRAPH_GRAPHICS_DEVICE_LIGHT_H__)
#define __IZANAGI_TOOL_KIT_GRAPH_GRAPHICS_DEVICE_LIGHT_H__

#include <d3d9.h>
#include <d3dx9.h>
//#include <dxerr9.h>

#include <vector>

#include "izDefs.h"
#include "izStd.h"
#include "../std/SimpleMemoryAllocator.h"

namespace izanagi {
namespace izanagi_tk {
	class CTextureLite;

	/**
	* 簡易グラフィックスデバイス
	*/
	class CGraphicsDeviceLite : public CObject {
	private:
		static CGraphicsDeviceLite* s_pInstance;

		// メモリアロケータ
		static CSimpleMemoryAllocator s_cAllocator;

	public:
		// インスタンス作成
		static CGraphicsDeviceLite* CreateGraphicsDeviceLight(HWND hWnd);

		// インスタンス取得
		static CGraphicsDeviceLite* GetInstance() { return s_pInstance; }

		// メモリアロケータ取得
		static IMemoryAllocator* GetAllocator() { return &s_cAllocator; }

	protected:
		CGraphicsDeviceLite();
		~CGraphicsDeviceLite();

		NO_COPIABLE(CGraphicsDeviceLite);
		
	protected:
		// 解放
		void InternalRelease();
	
		// 初期化
		BOOL Init(HWND hWnd);

	public:
		// テクスチャ読み込み
		CTextureLite* CreateTextureFromFile(LPCSTR lpszPath);

		// テクスチャ作成
		CTextureLite* CreateTexture(
			IZ_UINT nWidth,
			IZ_UINT nHeight,
			D3DFORMAT fmt);

	public:
		LPDIRECT3DDEVICE9 GetRawInterface() { return m_pDevice; }

	protected:
		LPDIRECT3D9 m_pD3D;
		LPDIRECT3DDEVICE9 m_pDevice;

		D3DPRESENT_PARAMETERS m_sPresentParameters;
	};
}	// namespace izanagi_tk
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_TOOL_KIT_GRAPH_GRAPHICS_DEVICE_LIGHT_H__)
