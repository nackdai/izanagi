#if !defined(__URANUS_DEBUG_UTIL_DEBUG_UI_FONT_LAYOUT_H__)
#define __URANUS_DEBUG_UTIL_DEBUG_UI_FONT_LAYOUT_H__

#include "DebugUIFontRenderer.h"

namespace uranus {
	class CDebugUIFontLayout {
		friend class CDebugUIManager;

	public:
		/**
		* �A���C��
		*/
		enum ALIGN {
			ALIGN_LEFT = 0,	// ������
			ALIGN_CENTER,	// ��������
			ALIGN_RIGHT,	// �E����
		};

	private:
		inline CDebugUIFontLayout();
		inline ~CDebugUIFontLayout();

		CDebugUIFontLayout(const CDebugUIFontLayout& rhs);
		const CDebugUIFontLayout& operator=(const CDebugUIFontLayout& rhs);

	public:
		// ������`��
		void RenderText(
			CGraphicsDevice* pDevice,
			UN_PCSTR pszStr);

		// �����񕝎擾
		UN_UINT GetTextWidth(UN_PCSTR* pszStr) const;

		// �t�H���g�����擾
		UN_UINT GetFontHeight() const;

	public:
		// �͈͎w��
		inline void SetRect(const CIntRect& rc);

		// �V�U�����O
		inline void EnableScissor(UN_BOOL bEnable);

		// �A���C���Z�b�g
		inline void SetAlign(ALIGN nAlign);

		// �J���[�Z�b�g
		inline void SetColor(UN_UINT nColor);
		inline void SetColor(const SColor& sColor);
		inline void SetColor(const CColor& cColor);

		// ellipsize
		inline void EnableEllipsize(UN_BOOL bEnable);

		// wrap
		inline void EnableWrap(UN_BOOL bEnable);

	private:
		// �����_���Z�b�g
		inline void SetRenderer(IDebugUIFontRenderer* pRenderer);

	private:
		IDebugUIFontRenderer* m_pFontRenderer;

		// �\���͈�
		CIntRect m_Rect;
		
		// �A���C��
		ALIGN m_nAlign;

		// �J���[
		CColor m_cColor;

		struct {
			UN_UINT is_scissor		: 1;	// �V�U�����O
			UN_UINT is_ellipsize	: 1;	// ellipsize
			UN_UINT is_wrap			: 1;	// wrap
		} m_Flags;
	};

	// inlien **************************************

	// �R���X�g���N�^
	CDebugUIFontLayout::CDebugUIFontLayout()
	{
		m_pFontRenderer = UN_NULL;

		// �K��
		m_Rect.SetRect(0, 0, 100, 100);

		m_nAlign = ALIGN_LEFT;

		m_Flags.is_scissor = UN_FALSE;
		m_Flags.is_ellipsize = UN_FALSE;
		m_Flags.is_wrap = UN_FALSE;
	}

	// �f�X�g���N�^
	CDebugUIFontLayout::~CDebugUIFontLayout()
	{
		SAFE_RELEASE(m_pFontRenderer);
	}

	/**
	* �͈͎w��
	*/
	void CDebugUIFontLayout::SetRect(const CIntRect& rc)
	{
		m_Rect = rc;
	}

	/**
	* �V�U�����O
	*/
	void CDebugUIFontLayout::EnableScissor(UN_BOOL bEnable)
	{
		m_Flags.is_scissor = bEnable;
	}

	/**
	* �A���C���Z�b�g
	*/
	void CDebugUIFontLayout::SetAlign(ALIGN nAlign)
	{
		m_nAlign = nAlign;
	}

	/**
	* �J���[�Z�b�g
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

	// �����_���Z�b�g
	void CDebugUIFontLayout::SetRenderer(IDebugUIFontRenderer* pRenderer)
	{
		SAFE_REPLACE(m_pFontRenderer, pRenderer);
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_DEBUG_UTIL_DEBUG_UI_FONT_LAYOUT_H__)
