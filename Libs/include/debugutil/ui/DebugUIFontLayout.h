#if !defined(__IZANAGI_DEBUG_UTIL_DEBUG_UI_FONT_LAYOUT_H__)
#define __IZANAGI_DEBUG_UTIL_DEBUG_UI_FONT_LAYOUT_H__

#include "DebugUIFontRenderer.h"

namespace izanagi {
	class CDebugUIFontLayout {
		friend class CDebugUIManager;

	public:
		/**
		* アライン
		*/
		enum ALIGN {
			ALIGN_LEFT = 0,	// 左揃え
			ALIGN_CENTER,	// 中央揃え
			ALIGN_RIGHT,	// 右揃え
		};

	private:
		inline CDebugUIFontLayout();
		inline ~CDebugUIFontLayout();

		CDebugUIFontLayout(const CDebugUIFontLayout& rhs);
		const CDebugUIFontLayout& operator=(const CDebugUIFontLayout& rhs);

	public:
		// 文字列描画
		void RenderText(
			CGraphicsDevice* pDevice,
			IZ_PCSTR pszStr);

		// 文字列幅取得
		IZ_UINT GetTextWidth(IZ_PCSTR* pszStr) const;

		// フォント高さ取得
		IZ_UINT GetFontHeight() const;

	public:
		// 範囲指定
		inline void SetRect(const CIntRect& rc);

		// シザリング
		inline void EnableScissor(IZ_BOOL bEnable);

		// アラインセット
		inline void SetAlign(ALIGN nAlign);

		// カラーセット
		inline void SetColor(IZ_UINT nColor);
		inline void SetColor(const SColor& sColor);
		inline void SetColor(const CColor& cColor);

		// ellipsize
		inline void EnableEllipsize(IZ_BOOL bEnable);

		// wrap
		inline void EnableWrap(IZ_BOOL bEnable);

	private:
		// レンダラセット
		inline void SetRenderer(IDebugUIFontRenderer* pRenderer);

	private:
		IDebugUIFontRenderer* m_pFontRenderer;

		// 表示範囲
		CIntRect m_Rect;
		
		// アライン
		ALIGN m_nAlign;

		// カラー
		CColor m_cColor;

		struct {
			IZ_UINT is_scissor		: 1;	// シザリング
			IZ_UINT is_ellipsize	: 1;	// ellipsize
			IZ_UINT is_wrap			: 1;	// wrap
		} m_Flags;
	};

	// inlien **************************************

	// コンストラクタ
	CDebugUIFontLayout::CDebugUIFontLayout()
	{
		m_pFontRenderer = IZ_NULL;

		// 適当
		m_Rect.SetRect(0, 0, 100, 100);

		m_nAlign = ALIGN_LEFT;

		m_Flags.is_scissor = IZ_FALSE;
		m_Flags.is_ellipsize = IZ_FALSE;
		m_Flags.is_wrap = IZ_FALSE;
	}

	// デストラクタ
	CDebugUIFontLayout::~CDebugUIFontLayout()
	{
		SAFE_RELEASE(m_pFontRenderer);
	}

	/**
	* 範囲指定
	*/
	void CDebugUIFontLayout::SetRect(const CIntRect& rc)
	{
		m_Rect = rc;
	}

	/**
	* シザリング
	*/
	void CDebugUIFontLayout::EnableScissor(IZ_BOOL bEnable)
	{
		m_Flags.is_scissor = bEnable;
	}

	/**
	* アラインセット
	*/
	void CDebugUIFontLayout::SetAlign(ALIGN nAlign)
	{
		m_nAlign = nAlign;
	}

	/**
	* カラーセット
	*/
	void CDebugUIFontLayout::SetColor(IZ_UINT nColor)
	{
		m_cColor.Set(nColor);
	}

	void CDebugUIFontLayout::SetColor(const SColor& sColor)
	{
		m_cColor.Set(sColor);
	}

	void CDebugUIFontLayout::SetColor(const CColor& cColor)
	{
		m_cColor.Set(cColor);
	}

	/**
	* ellipsize
	*/
	void CDebugUIFontLayout::EnableEllipsize(IZ_BOOL bEnable)
	{
		m_Flags.is_ellipsize = bEnable;
	}

	/**
	* wrap
	*/
	void CDebugUIFontLayout::EnableWrap(IZ_BOOL bEnable)
	{
		m_Flags.is_wrap = bEnable;
	}

	// レンダラセット
	void CDebugUIFontLayout::SetRenderer(IDebugUIFontRenderer* pRenderer)
	{
		SAFE_REPLACE(m_pFontRenderer, pRenderer);
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_DEBUG_UTIL_DEBUG_UI_FONT_LAYOUT_H__)
