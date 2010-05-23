#if !defined(__URANUS_DEBUGUTIL_DEBUG_UI_WIDGET_H__)
#define __URANUS_DEBUGUTIL_DEBUG_UI_WIDGET_H__

#include "unDefs.h"
#include "unStd.h"

namespace uranus {
	class CGraphicsDevice;

	/**
	* UI�̃x�[�X
	*/
	class CDebugUIWidget : public CObject {
		friend class CDebugUIManager;
		friend class CDebugUIDialog;

	protected:
		inline CDebugUIWidget();
		virtual ~CDebugUIWidget() {}

	private:
		CDebugUIWidget(const CDebugUIWidget& rhs);
		const CDebugUIWidget& operator=(const CDebugUIWidget& rhs);

	protected:
		// ���
		inline void ReleaseIntenral();

		// �`��
		virtual void Render(
			CGraphicsDevice* pDevice,
			UN_INT16 nX, UN_INT16 nY) = 0;

		virtual UN_BOOL OnMouseLButtonDown(UN_INT nX, UN_INT nY) { return UN_FALSE; }
		virtual UN_BOOL OnMouseLButtonUp(UN_INT nX, UN_INT nY) { return UN_FALSE; }
		virtual UN_BOOL OnMouseRButtonDown(UN_INT nX, UN_INT nY) { return UN_FALSE; }
		virtual UN_BOOL OnMouseRButtonUp(UN_INT nX, UN_INT nY) { return UN_FALSE; }
		virtual UN_BOOL OnMouseMove(UN_INT nX, UN_INT nY) { return UN_FALSE; }

	public:
		// �ʒu�Z�b�g
		virtual inline void SetPosition(UN_INT16 nX, UN_INT16 nY);
		virtual inline void SetPosX(UN_INT16 nX);
		virtual inline void SetPosY(UN_INT16 nY);

		// �T�C�Y�Z�b�g
		virtual inline void SetSize(UN_UINT16 nW, UN_UINT16 nH);
		virtual inline void SetWidth(UN_UINT16 nW);
		virtual inline void SetHeight(UN_UINT16 nH);

		// �L���t���O�Z�b�g
		virtual inline void SetIsEnable(UN_BOOL bEnable);

		// �\���t���O�Z�b�g
		virtual inline void SetIsShow(UN_BOOL bIsShow);

	public:
		// �L�����ǂ���
		inline UN_BOOL IsEnable() const;

		// �\������Ă��邩�ǂ���
		inline UN_BOOL IsShow() const;

		// �t�H�[�J�X����Ă��邩�ǂ���
		inline UN_BOOL IsFocused() const;

	protected:
		// ���X�g�A�C�e���擾
		CStdList<CDebugUIWidget>::Item* GetListItem() { return &m_ListItem; }

	protected:
		IMemoryAllocator* m_pAllocator;

		// ���X�g�A�C�e��
		CStdList<CDebugUIWidget>::Item m_ListItem;

		// �ʒu
		UN_INT16 m_nX;
		UN_INT16 m_nY;

		// �T�C�Y
		UN_UINT16 m_nWidth;
		UN_UINT16 m_nHeight;

		struct {
			UN_UINT32 enable	: 1;	// �L���E����
			UN_UINT32 is_show	: 1;	// �\���E��\��
			UN_UINT32 is_focus	: 1;	// �t�H�[�J�X
		} m_Flags;
	};

	// inline *********************************

	// �R���X�g���N�^
	CDebugUIWidget::CDebugUIWidget()
	{
		m_pAllocator = UN_NULL;

		m_ListItem.Init(this);

		m_nX = 0;
		m_nY = 0;

		m_nWidth = 0;
		m_nHeight = 0;

		m_Flags.enable = UN_TRUE;
		m_Flags.is_show = UN_TRUE;
		m_Flags.is_focus = UN_FALSE;
	}

	// ���
	void CDebugUIWidget::ReleaseIntenral()
	{
		delete this;
		if (m_pAllocator != UN_NULL) {
			m_pAllocator->Free(this);
		}
	}

	/**
	* �L���t���O�Z�b�g
	*/
	void CDebugUIWidget::SetIsEnable(UN_BOOL bEnable)
	{
		m_Flags.enable = bEnable;
	}

	/**
	* �\���t���O�Z�b�g
	*/
	void CDebugUIWidget::SetIsShow(UN_BOOL bIsShow)
	{
		m_Flags.is_show = bIsShow;
	}

	/**
	* �L�����ǂ���
	*/
	UN_BOOL CDebugUIWidget::IsEnable() const
	{
		return m_Flags.enable;
	}

	/**
	* �\������Ă��邩�ǂ���
	*/
	UN_BOOL CDebugUIWidget::IsShow() const
	{
		return m_Flags.is_show;
	}

	/**
	* �t�H�[�J�X����Ă��邩�ǂ���
	*/
	UN_BOOL CDebugUIWidget::IsFocused() const
	{
		return m_Flags.is_focus;
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_DEBUGUTIL_DEBUG_UI_WND_H__)
