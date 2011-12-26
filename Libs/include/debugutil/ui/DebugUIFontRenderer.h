#if !defined(__IZANAGI_DEBUGUTIL_DEBUG_UI_FONT_RENDERER_H__)
#define __IZANAGI_DEBUGUTIL_DEBUG_UI_FONT_RENDERER_H__

#include "izStd.h"

namespace izanagi {
	class CGraphicsDevice;

	/**
	*/
	class IDebugUIFontRenderer : public CObject {
		friend class CDebugUIFontLayout;

	protected:
		IDebugUIFontRenderer();
		virtual ~IDebugUIFontRenderer();

		IDebugUIFontRenderer(const IDebugUIFontRenderer& rhs);
		const IDebugUIFontRenderer& operator=(const IDebugUIFontRenderer& rhs);

	protected:
		// 文字列描画
		virtual void RenderText(
			CGraphicsDevice* pDevice,
			IZ_INT nX, IZ_INT nY,
			IZ_COLOR nColor,
			IZ_PCSTR pszStr) = 0;

		// 文字列幅取得
		virtual IZ_UINT GetTextWidth(IZ_PCSTR* pszStr) const = 0;

		// フォント高さ取得
		virtual IZ_UINT GetFontHeight() const = 0;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_DEBUGUTIL_DEBUG_UI_FONT_RENDERER_H__)
