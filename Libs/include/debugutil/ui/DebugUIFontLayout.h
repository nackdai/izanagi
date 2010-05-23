#if !defined(__URANUS_DEBUG_UTIL_DEBUG_UI_FONT_LAYOUT_H__)
#define __URANUS_DEBUG_UTIL_DEBUG_UI_FONT_LAYOUT_H__

#include "DebugUIFontRenderer.h"

namespace uranus {
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
			UN_PCSTR pszStr);

		// 文字列幅取得
		UN_UINT GetTextWidth(UN_PCSTR* pszStr) const;

		// フォント高さ取得
		UN_UINT GetFontHeight() const;

	public:
		// 範囲指定
		inline void SetRect(const CIntRect& rc);

		// シザリング
		inline void EnableScissor(UN_BOOL bEnable);

		// アラインセット
		inline void SetAlign(ALIGN nAlign);

		// カラーセット
		inline void SetColor(UN_UINT nColor);
		inline void SetColor(const SColor& sColor);
		inline void SetColor(const CColor& cColor);

		// ellipsize
		inline void EnableEllipsize(UN_BOOL bEnable);

		// wrap
		inline void EnableWrap(UN_BOOL bEnable);

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
			UN_UINT is_scissor		: 1;	// シザリング
			UN_UINT is_ellipsize	: 1;	// ellipsize
			UN_UINT is_wrap			: 1;	// wrap
		} m_Flags;
	};

	// inlien **************************************

	// コンストラクタ
	CDebugUIFontLayout::CDebugUIFontLayout()
	{
		m_pFontRenderer = UN_NULL;

		// 適当
		m_Rect.SetRect(0, 0, 100, 100);

		m_nAlign = ALIGN_LEFT;

		m_Flags.is_scissor = UN_FALSE;
		m_Flags.is_ellipsize = UN_FALSE;
		m_Flags.is_wrap = UN_FALSE;
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
	void CDebugUIFontLayout::EnableScissor(UN_BOOL bEnable)
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
	void CDebugUIFontLayout::SetColor(UN_UINT nColor)
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
	void CDebugUIFontLayout::EnableEllipsize(UN_BOOL bEnable)
	{
		m_Flags.is_ellipsize = bEnable;
	}

	/**
	* wrap
	*/
	void CDebugUIFontLayout::EnableWrap(UN_BOOL bEnable)
	{
		m_Flags.is_wrap = bEnable;
	}

	// レンダラセット
	void CDebugUIFontLayout::SetRenderer(IDebugUIFontRenderer* pRenderer)
	{
		SAFE_REPLACE(m_pFontRenderer, pRenderer);
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_DEBUG_UTIL_DEBUG_UI_FONT_LAYOUT_H__)
