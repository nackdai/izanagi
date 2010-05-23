#if !defined(__URANUS_DEBUGUTIL_DEBUG_UI_WIDGET_H__)
#define __URANUS_DEBUGUTIL_DEBUG_UI_WIDGET_H__

#include "unDefs.h"
#include "unStd.h"

namespace uranus {
	class CGraphicsDevice;

	/**
	* UIのベース
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
		// 解放
		inline void ReleaseIntenral();

		// 描画
		virtual void Render(
			CGraphicsDevice* pDevice,
			UN_INT16 nX, UN_INT16 nY) = 0;

		virtual UN_BOOL OnMouseLButtonDown(UN_INT nX, UN_INT nY) { return UN_FALSE; }
		virtual UN_BOOL OnMouseLButtonUp(UN_INT nX, UN_INT nY) { return UN_FALSE; }
		virtual UN_BOOL OnMouseRButtonDown(UN_INT nX, UN_INT nY) { return UN_FALSE; }
		virtual UN_BOOL OnMouseRButtonUp(UN_INT nX, UN_INT nY) { return UN_FALSE; }
		virtual UN_BOOL OnMouseMove(UN_INT nX, UN_INT nY) { return UN_FALSE; }

	public:
		// 位置セット
		virtual inline void SetPosition(UN_INT16 nX, UN_INT16 nY);
		virtual inline void SetPosX(UN_INT16 nX);
		virtual inline void SetPosY(UN_INT16 nY);

		// サイズセット
		virtual inline void SetSize(UN_UINT16 nW, UN_UINT16 nH);
		virtual inline void SetWidth(UN_UINT16 nW);
		virtual inline void SetHeight(UN_UINT16 nH);

		// 有効フラグセット
		virtual inline void SetIsEnable(UN_BOOL bEnable);

		// 表示フラグセット
		virtual inline void SetIsShow(UN_BOOL bIsShow);

	public:
		// 有効かどうか
		inline UN_BOOL IsEnable() const;

		// 表示されているかどうか
		inline UN_BOOL IsShow() const;

		// フォーカスされているかどうか
		inline UN_BOOL IsFocused() const;

	protected:
		// リストアイテム取得
		CStdList<CDebugUIWidget>::Item* GetListItem() { return &m_ListItem; }

	protected:
		IMemoryAllocator* m_pAllocator;

		// リストアイテム
		CStdList<CDebugUIWidget>::Item m_ListItem;

		// 位置
		UN_INT16 m_nX;
		UN_INT16 m_nY;

		// サイズ
		UN_UINT16 m_nWidth;
		UN_UINT16 m_nHeight;

		struct {
			UN_UINT32 enable	: 1;	// 有効・無効
			UN_UINT32 is_show	: 1;	// 表示・非表示
			UN_UINT32 is_focus	: 1;	// フォーカス
		} m_Flags;
	};

	// inline *********************************

	// コンストラクタ
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

	// 解放
	void CDebugUIWidget::ReleaseIntenral()
	{
		delete this;
		if (m_pAllocator != UN_NULL) {
			m_pAllocator->Free(this);
		}
	}

	/**
	* 有効フラグセット
	*/
	void CDebugUIWidget::SetIsEnable(UN_BOOL bEnable)
	{
		m_Flags.enable = bEnable;
	}

	/**
	* 表示フラグセット
	*/
	void CDebugUIWidget::SetIsShow(UN_BOOL bIsShow)
	{
		m_Flags.is_show = bIsShow;
	}

	/**
	* 有効かどうか
	*/
	UN_BOOL CDebugUIWidget::IsEnable() const
	{
		return m_Flags.enable;
	}

	/**
	* 表示されているかどうか
	*/
	UN_BOOL CDebugUIWidget::IsShow() const
	{
		return m_Flags.is_show;
	}

	/**
	* フォーカスされているかどうか
	*/
	UN_BOOL CDebugUIWidget::IsFocused() const
	{
		return m_Flags.is_focus;
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_DEBUGUTIL_DEBUG_UI_WND_H__)
