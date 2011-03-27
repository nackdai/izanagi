#include "debugutil/ui/DebugUIManager.h"
#include "debugutil/ui/DebugUIDialog.h"

using namespace izanagi;

CDebugUIManager* CDebugUIManager::s_pInstance = IZ_NULL;

/**
* インスタンス作成
*/
CDebugUIManager* CDebugUIManager::CreateDebugUIManager(IMemoryAllocator* pAllocator)
{
	if (s_pInstance != IZ_NULL) {
		return s_pInstance;
	}

	IZ_ASSERT(pAllocator != IZ_NULL);

	// メモリ確保
	IZ_BYTE* pBuf = (IZ_BYTE*)ALLOC_ZERO(pAllocator, sizeof(CDebugUIManager));
	IZ_BOOL result = (pBuf != IZ_NULL);
	VGOTO(!result, __EXIT__);

	// インスタンス作成
	s_pInstance = new(pBuf) CDebugUIManager;
	{
		s_pInstance->AddRef();
		s_pInstance->m_pAllocator = pAllocator;
	}

__EXIT__:
	if (!result) {
		if (s_pInstance != IZ_NULL) {
			SAFE_RELEASE(s_pInstance);
		}
		else if (pBuf != IZ_NULL) {
			pAllocator->Free(pBuf);
		}
	}
	return s_pInstance;
}

// ダイアログ追加
IZ_BOOL CDebugUIManager::Add(CDebugUIDialog* pDlg)
{
	IZ_ASSERT(pDlg != IZ_NULL);

	IZ_BOOL ret = m_DlgList.AddTail(pDlg->GetListItem());
	return ret;
}

// ダイアログ全削除
void CDebugUIManager::RemoveAll()
{
	CStdList<CDebugUIWidget>::Item* pItem = m_DlgList.GetTop();
	while (pItem != IZ_NULL) {
		CStdList<CDebugUIWidget>::Item* pNext = pItem->GetNext();
		pItem->Leave();

		CDebugUIWidget* pDlg = pItem->GetData();
		SAFE_RELEASE(pDlg);

		pItem = pNext;
	}

	m_DlgList.Clear();
}

// ウィジェットイベント処理
template <class _Func>
void CDebugUIManager::OnDoEvent(
	IZ_INT nX, IZ_INT nY,
	_Func& func)
{
	if (m_pCapture != IZ_NULL) {
		func(m_pCapture, nX, nY);
	}
	else {
		CStdList<CDebugUIWidget>::Item* pItem = m_DlgList.GetTop();
		while (pItem != IZ_NULL) {
			CDebugUIWidget* pWidget = pItem->GetData();
			if (pWidget != IZ_NULL) {
				if (func(pWidget, nX, nY)) {
					break;
				}
			}

			pItem = pItem->GetNext();
		}
	}
}

/**
* マウス左ボタン押した
*/
void CDebugUIManager::OnMouseLButtonDown(IZ_INT nX, IZ_INT nY)
{
	OnDoEvent(
		nX, nY,
		FuncOnMouseLButtonDown());
}

/**
* マウス左ボタン離した
*/
void CDebugUIManager::OnMouseLButtonUp(IZ_INT nX, IZ_INT nY)
{
	OnDoEvent(
		nX, nY,
		FuncOnMouseLButtonUp());
}

/**
* マウス右ボタン押した
*/
void CDebugUIManager::OnMouseRButtonDown(IZ_INT nX, IZ_INT nY)
{
	OnDoEvent(
		nX, nY,
		FuncOnMouseRButtonDown());
}

/**
* マウス右ボタン離した
*/
void CDebugUIManager::OnMouseRButtonUp(IZ_INT nX, IZ_INT nY)
{
	OnDoEvent(
		nX, nY,
		FuncOnMouseRButtonUp());
}

/**
* マウス移動
*/
void CDebugUIManager::OnMouseMove(IZ_INT nX, IZ_INT nY)
{
	OnDoEvent(
		nX, nY,
		FuncOnMouseMove());
}
