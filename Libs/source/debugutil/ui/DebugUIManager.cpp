#include "debugutil/ui/DebugUIManager.h"
#include "debugutil/ui/DebugUIDialog.h"

using namespace uranus;

CDebugUIManager* CDebugUIManager::s_pInstance = UN_NULL;

/**
* インスタンス作成
*/
CDebugUIManager* CDebugUIManager::CreateDebugUIManager(IMemoryAllocator* pAllocator)
{
	if (s_pInstance != UN_NULL) {
		return s_pInstance;
	}

	UN_ASSERT(pAllocator != UN_NULL);

	// メモリ確保
	UN_BYTE* pBuf = (UN_BYTE*)ALLOC_ZERO(pAllocator, sizeof(CDebugUIManager));
	UN_BOOL result = (pBuf != UN_NULL);
	VGOTO(!result, __EXIT__);

	// インスタンス作成
	s_pInstance = new(pBuf) CDebugUIManager;
	{
		s_pInstance->AddRef();
		s_pInstance->m_pAllocator = pAllocator;
	}

__EXIT__:
	if (!result) {
		if (s_pInstance != UN_NULL) {
			SAFE_RELEASE(s_pInstance);
		}
		else if (pBuf != UN_NULL) {
			pAllocator->Free(pBuf);
		}
	}
	return s_pInstance;
}

// ダイアログ追加
UN_BOOL CDebugUIManager::Add(CDebugUIDialog* pDlg)
{
	UN_ASSERT(pDlg != UN_NULL);

	UN_BOOL ret = m_DlgList.AddTail(pDlg->GetListItem());
	return ret;
}

// ダイアログ全削除
void CDebugUIManager::RemoveAll()
{
	CStdList<CDebugUIWidget>::Item* pItem = m_DlgList.GetTop();
	while (pItem != UN_NULL) {
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
	UN_INT nX, UN_INT nY,
	_Func& func)
{
	if (m_pCapture != UN_NULL) {
		func(m_pCapture, nX, nY);
	}
	else {
		CStdList<CDebugUIWidget>::Item* pItem = m_DlgList.GetTop();
		while (pItem != UN_NULL) {
			CDebugUIWidget* pWidget = pItem->GetData();
			if (pWidget != UN_NULL) {
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
void CDebugUIManager::OnMouseLButtonDown(UN_INT nX, UN_INT nY)
{
	OnDoEvent(
		nX, nY,
		FuncOnMouseLButtonDown());
}

/**
* マウス左ボタン離した
*/
void CDebugUIManager::OnMouseLButtonUp(UN_INT nX, UN_INT nY)
{
	OnDoEvent(
		nX, nY,
		FuncOnMouseLButtonUp());
}

/**
* マウス右ボタン押した
*/
void CDebugUIManager::OnMouseRButtonDown(UN_INT nX, UN_INT nY)
{
	OnDoEvent(
		nX, nY,
		FuncOnMouseRButtonDown());
}

/**
* マウス右ボタン離した
*/
void CDebugUIManager::OnMouseRButtonUp(UN_INT nX, UN_INT nY)
{
	OnDoEvent(
		nX, nY,
		FuncOnMouseRButtonUp());
}

/**
* マウス移動
*/
void CDebugUIManager::OnMouseMove(UN_INT nX, UN_INT nY)
{
	OnDoEvent(
		nX, nY,
		FuncOnMouseMove());
}
