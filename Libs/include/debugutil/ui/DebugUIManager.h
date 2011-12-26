#if !defined(__IZANAGI_DEBUGUTIL_DEBUG_UI_MANAGER_H__)
#define __IZANAGI_DEBUGUTIL_DEBUG_UI_MANAGER_H__

#include "izDefs.h"
#include "izStd.h"
#include "DebugUIWidget.h"
#include "DebugUIFontRenderer.h"
#include "DebugUIFontLayout.h"

namespace izanagi {
	class CDebugUIDialog;

	/**
	*/
	class CDebugUIManager : public CObject {
		friend class CDebugUIDialog;

	private:
		static CDebugUIManager* s_pInstance;

	public:
		// インスタンス取得
		static CDebugUIManager* GetInstance() { return s_pInstance; }

		// インスタンス作成
		CDebugUIManager* CreateDebugUIManager(IMemoryAllocator* pAllocator);

	protected:
		inline CDebugUIManager();
		inline ~CDebugUIManager();

		CDebugUIManager(const CDebugUIManager& rhs);
		const CDebugUIManager& operator=(const CDebugUIManager& rhs);

	protected:
		// 解放
		inline void InternalRelease();

	protected:
		// ダイアログ追加
		IZ_BOOL Add(CDebugUIDialog* pDlg);

		// ダイアログ全削除
		void RemoveAll();

		// リスト取得
		CStdList<CDebugUIWidget>* GetList() { return &m_DlgList; }

	protected:
		template <class _Func>
		void OnDoEvent(
			IZ_INT nX, IZ_INT nY,
			_Func& func);

		struct FuncOnMouseLButtonDown {
			IZ_BOOL operator()(CDebugUIWidget* pWidget, IZ_INT nX, IZ_INT nY) { return pWidget->OnMouseLButtonDown(nX, nY); }
		};
		struct FuncOnMouseLButtonUp {
			IZ_BOOL operator()(CDebugUIWidget* pWidget, IZ_INT nX, IZ_INT nY) { return pWidget->OnMouseLButtonUp(nX, nY); }
		};
		struct FuncOnMouseRButtonDown {
			IZ_BOOL operator()(CDebugUIWidget* pWidget, IZ_INT nX, IZ_INT nY) { return pWidget->OnMouseRButtonDown(nX, nY); }
		};
		struct FuncOnMouseRButtonUp {
			IZ_BOOL operator()(CDebugUIWidget* pWidget, IZ_INT nX, IZ_INT nY) { return pWidget->OnMouseRButtonUp(nX, nY); }
		};
		struct FuncOnMouseMove {
			IZ_BOOL operator()(CDebugUIWidget* pWidget, IZ_INT nX, IZ_INT nY) { return pWidget->OnMouseMove(nX, nY); }
		};

	public:
		void OnMouseLButtonDown(IZ_INT nX, IZ_INT nY);
		void OnMouseLButtonUp(IZ_INT nX, IZ_INT nY);
		void OnMouseRButtonDown(IZ_INT nX, IZ_INT nY);
		void OnMouseRButtonUp(IZ_INT nX, IZ_INT nY);
		void OnMouseMove(IZ_INT nX, IZ_INT nY);

	public:
		// フォントレンダラをセット
		inline IZ_BOOL SetFontRenderer(IDebugUIFontRenderer* pFontRenderer);

		// フォントレイアウタを取得
		inline CDebugUIFontLayout* GetFontLayout();

	protected:
		IMemoryAllocator* m_pAllocator;

		// キャプチャウィジェット
		CDebugUIWidget* m_pCapture;

		// ダイアログリスト
		CStdList<CDebugUIWidget> m_DlgList;

		// フォントレイアウタ
		CDebugUIFontLayout m_FontLayout;
	};

	// inline *******************************

	// コンストラクタ
	CDebugUIManager::CDebugUIManager()
	{
		m_pAllocator = IZ_NULL;
		m_pCapture = IZ_NULL;
	}

	// デストラクタ
	CDebugUIManager::~CDebugUIManager()
	{
		// ダイアログ全削除
		RemoveAll();
	}

	// 解放
	void CDebugUIManager::InternalRelease()
	{
		delete this;
		if (m_pAllocator != IZ_NULL) {
			m_pAllocator->Free(this);
		}
	}

	/**
	* フォントレンダラをセット
	*/
	IZ_BOOL CDebugUIManager::SetFontRenderer(IDebugUIFontRenderer* pFontRenderer)
	{
		m_FontLayout.SetRenderer(pFontRenderer);
	}

	/**
	* フォントレイアウタを取得
	*/
	CDebugUIFontLayout* CDebugUIManager::GetFontLayout()
	{
		return &m_FontLayout;
	}

}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_DEBUGUTIL_DEBUG_UI_MANAGER_H__)
