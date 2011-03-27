#if !defined(__IZANAGI_DEBUGUTIL_DEBUG_UI_WIDGET_H__)
#define __IZANAGI_DEBUGUTIL_DEBUG_UI_WIDGET_H__

#include "izDefs.h"
#include "izStd.h"

namespace izanagi {
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
			IZ_INT16 nX, IZ_INT16 nY) = 0;

		virtual IZ_BOOL OnMouseLButtonDown(IZ_INT nX, IZ_INT nY) { return IZ_FALSE; }
		virtual IZ_BOOL OnMouseLButtonUp(IZ_INT nX, IZ_INT nY) { return IZ_FALSE; }
		virtual IZ_BOOL OnMouseRButtonDown(IZ_INT nX, IZ_INT nY) { return IZ_FALSE; }
		virtual IZ_BOOL OnMouseRButtonUp(IZ_INT nX, IZ_INT nY) { return IZ_FALSE; }
		virtual IZ_BOOL OnMouseMove(IZ_INT nX, IZ_INT nY) { return IZ_FALSE; }

	public:
		// 位置セット
		virtual inline void SetPosition(IZ_INT16 nX, IZ_INT16 nY);
		virtual inline void SetPosX(IZ_INT16 nX);
		virtual inline void SetPosY(IZ_INT16 nY);

		// サイズセット
		virtual inline void SetSize(IZ_UINT16 nW, IZ_UINT16 nH);
		virtual inline void SetWidth(IZ_UINT16 nW);
		virtual inline void SetHeight(IZ_UINT16 nH);

		// 有効フラグセット
		virtual inline void SetIsEnable(IZ_BOOL bEnable);

		// 表示フラグセット
		virtual inline void SetIsShow(IZ_BOOL bIsShow);

	public:
		// 有効かどうか
		inline IZ_BOOL IsEnable() const;

		// 表示されているかどうか
		inline IZ_BOOL IsShow() const;

		// フォーカスされているかどうか
		inline IZ_BOOL IsFocused() const;

	protected:
		// リストアイテム取得
		CStdList<CDebugUIWidget>::Item* GetListItem() { return &m_ListItem; }

	protected:
		IMemoryAllocator* m_pAllocator;

		// リストアイテム
		CStdList<CDebugUIWidget>::Item m_ListItem;

		// 位置
		IZ_INT16 m_nX;
		IZ_INT16 m_nY;

		// サイズ
		IZ_UINT16 m_nWidth;
		IZ_UINT16 m_nHeight;

		struct {
			IZ_UINT32 enable	: 1;	// 有効・無効
			IZ_UINT32 is_show	: 1;	// 表示・非表示
			IZ_UINT32 is_focus	: 1;	// フォーカス
		} m_Flags;
	};

	// inline *********************************

	// コンストラクタ
	CDebugUIWidget::CDebugUIWidget()
	{
		m_pAllocator = IZ_NULL;

		m_ListItem.Init(this);

		m_nX = 0;
		m_nY = 0;

		m_nWidth = 0;
		m_nHeight = 0;

		m_Flags.enable = IZ_TRUE;
		m_Flags.is_show = IZ_TRUE;
		m_Flags.is_focus = IZ_FALSE;
	}

	// 解放
	void CDebugUIWidget::ReleaseIntenral()
	{
		delete this;
		if (m_pAllocator != IZ_NULL) {
			m_pAllocator->Free(this);
		}
	}

	/**
	* 有効フラグセット
	*/
	void CDebugUIWidget::SetIsEnable(IZ_BOOL bEnable)
	{
		m_Flags.enable = bEnable;
	}

	/**
	* 表示フラグセット
	*/
	void CDebugUIWidget::SetIsShow(IZ_BOOL bIsShow)
	{
		m_Flags.is_show = bIsShow;
	}

	/**
	* 有効かどうか
	*/
	IZ_BOOL CDebugUIWidget::IsEnable() const
	{
		return m_Flags.enable;
	}

	/**
	* 表示されているかどうか
	*/
	IZ_BOOL CDebugUIWidget::IsShow() const
	{
		return m_Flags.is_show;
	}

	/**
	* フォーカスされているかどうか
	*/
	IZ_BOOL CDebugUIWidget::IsFocused() const
	{
		return m_Flags.is_focus;
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_DEBUGUTIL_DEBUG_UI_WND_H__)
