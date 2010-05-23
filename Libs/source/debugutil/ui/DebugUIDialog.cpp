#include "debugutil/ui/DebugUIDialog.h"
#include "debugutil/ui/DebugUIManager.h"
#include "unGraph.h"

using namespace uranus;

// コンストラクタ
CDebugUIDialog::CDebugUIDialog()
{
	m_List.AddTail(GetListItem());

	// マネージャに登録
	CDebugUIManager::GetInstance()->Add(this);
}

// デストラクタ
CDebugUIDialog::~CDebugUIDialog()
{
	RemoveAll();
}

// 描画
void CDebugUIDialog::Render(CGraphicsDevice* pDevice)
{
	Render(
		pDevice,
		m_nX, m_nY);
}

// 描画
void CDebugUIDialog::Render(
	CGraphicsDevice* pDevice,
	UN_INT16 nX, UN_INT16 nY)
{
	UN_ASSERT(pDevice != UN_NULL);

	// TODO
	// 下地

	// パーツ
	CStdList<CDebugUIWidget>::Item* pItem = m_List.GetTop();
	while (pItem != UN_NULL) {
		CDebugUIWidget* pWidget = pItem->GetData();

		if (pWidget->IsShow()) {
			pWidget->Render(
				pDevice,
				nX, nY);
		}

		pItem = pItem->GetNext();
	}
}

/**
* 追加
*/
UN_BOOL CDebugUIDialog::Add(CDebugUIWidget* pWidget)
{
	UN_ASSERT(pWidget != UN_NULL);

	UN_BOOL ret = m_List.AddTail(pWidget->GetListItem());
	return ret;
}

/**
* 全削除
*/
void CDebugUIDialog::RemoveAll()
{
	CStdList<CDebugUIWidget>::Item* pItem = m_List.GetTop();
	while (pItem != UN_NULL) {
		CStdList<CDebugUIWidget>::Item* pNext = pItem->GetNext();
		pItem->Leave();

		CDebugUIWidget* pWidget = pItem->GetData();
		SAFE_RELEASE(pWidget);

		pItem = pNext;
	}
}

/**
* 有効フラグセット
*/
void CDebugUIDialog::SetIsEnable(UN_BOOL bEnable)
{
	CStdList<CDebugUIWidget>::Item* pItem = m_List.GetTop();
	while (pItem != NULL) {
		CDebugUIWidget* pWidget = pItem->GetData();
		if (pWidget != UN_NULL) {
			pWidget->SetIsEnable(bEnable);
		}
		pItem = pItem->GetNext();
	}
}

/**
* 表示フラグセット
*/
void CDebugUIDialog::SetIsShow(UN_BOOL bIsShow)
{
	CStdList<CDebugUIWidget>::Item* pItem = m_List.GetTop();
	while (pItem != NULL) {
		CDebugUIWidget* pWidget = pItem->GetData();
		if (pWidget != UN_NULL) {
			pWidget->SetIsShow(bIsShow);
		}
		pItem = pItem->GetNext();
	}
}
