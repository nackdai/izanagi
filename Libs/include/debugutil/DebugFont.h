#if !defined(__IZANAGI_DEBUG_UTIL_FONT_H__)
#define __IZANAGI_DEBUG_UTIL_FONT_H__

#include "izDefs.h"
#include "izStd.h"

namespace izanagi {
	class IMemoryAllocator;
	class CGraphicsDevice;
	class CTexture;

	class CDebugFont : public CObject {
	public:
		// インスタンス作成
		static CDebugFont* CreateDebugFont(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice);
			
		enum {
			FONT_SIZE = 16,			// フォントサイズ
			IMAGE_WIDTH = 256,		// フォントイメージ幅
			IMAGE_HEIGHT = 128,		// フォントイメージ高さ

			// イメージにおいて１ラインあたりのフォント数
			FONT_NUM_PER_LINE_IN_IMAGE = IMAGE_WIDTH / FONT_SIZE,
		};

	protected:
		CDebugFont();
		~CDebugFont();

		CDebugFont(const CDebugFont& rhs);
		const CDebugFont& operator =(const CDebugFont& rhs);

	protected:
		// 解放
		void InternalRelease();

	public:
		void Begin();
		void End();

		void DBPrint(
			const IZ_CHAR* str, ...);
		void DBPrint(
			IZ_DWORD color,
			const IZ_CHAR* str, ...);
		void DBPrint(
			IZ_INT left, IZ_INT top,
			const IZ_CHAR* str, ...);
		void DBPrint(
			IZ_INT left, IZ_INT top,
			IZ_DWORD color,
			const IZ_CHAR* str, ...);

	protected:
		// テクスチャ作成
		IZ_BOOL CreateTexture();

		// 文字描画
		void DrawFont(const IZ_CHAR* str);

	protected:
		// 切り出し矩形をセットする
		inline void SetTexRect(
			IZ_CHAR ch,
			CIntRect& rc);

	public:
		// 描画パラメータセット
		inline void SetRenderParam(
			IZ_INT left, IZ_INT top,
			IZ_DWORD color);

		void SetFontPos(IZ_INT left, IZ_INT top)
		{
			m_nLeft = left; m_nTop = top;
			m_nLeftBase = left;
		}

		void SetFontColor(IZ_DWORD color) { m_nColor = color; }

	public:
		IZ_UINT GetFontSize() { return FONT_SIZE; }
		IZ_INT GetFontPosLeft() { return m_nLeft; }
		IZ_INT GetFontPosTop() { return m_nTop; }
		IZ_DWORD GetFontColor() { return m_nColor; }

	protected:
		IMemoryAllocator* m_pAllocator;
		CGraphicsDevice* m_pDevice;

		CTexture* m_pFontTex;

		// 描画パラメータ
		IZ_INT m_nLeft;			// 文字位置（左）
		IZ_INT m_nTop;			// 文字位置（上）
		IZ_DWORD m_nColor;		// 文字色

		IZ_INT m_nLeftBase;

		IZ_BOOL m_bIsBegin;
	};

	// inline ***********************************

	// 切り出し矩形をセットする
	void CDebugFont::SetTexRect(
		IZ_CHAR ch,
		CIntRect& rc)
	{
		IZ_INT nPosY = ch / FONT_NUM_PER_LINE_IN_IMAGE;
		IZ_INT nPosX = ch % FONT_NUM_PER_LINE_IN_IMAGE;

		rc.left = FONT_SIZE * nPosX;
		rc.top = FONT_SIZE * nPosY;
		rc.right = rc.left + FONT_SIZE;
		rc.bottom = rc.top + FONT_SIZE;
	}

	// 描画パラメータセット
	void CDebugFont::SetRenderParam(
		IZ_INT left, IZ_INT top,
		IZ_DWORD color)
	{
		SetFontPos(left, top);
		SetFontColor(color);
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_DEBUG_UTIL_FONT_H__)
