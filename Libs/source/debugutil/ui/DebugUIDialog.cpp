#include "debugutil/ui/DebugUIDialog.h"
#include "debugutil/ui/DebugUIManager.h"
#include "unGraph.h"

using namespace uranus;

// �R���X�g���N�^
CDebugUIDialog::CDebugUIDialog()
{
	m_List.AddTail(GetListItem());

	// �}�l�[�W���ɓo�^
	CDebugUIManager::GetInstance()->Add(this);
}

// �f�X�g���N�^
CDebugUIDialog::~CDebugUIDialog()
{
	RemoveAll();
}

// �`��
void CDebugUIDialog::Render(CGraphicsDevice* pDevice)
{
	Render(
		pDevice,
		m_nX, m_nY);
}

// �`��
void CDebugUIDialog::Render(
	CGraphicsDevice* pDevice,
	UN_INT16 nX, UN_INT16 nY)
{
	UN_ASSERT(pDevice != UN_NULL);

	// TODO
	// ���n

	// �p�[�c
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
* �ǉ�
*/
UN_BOOL CDebugUIDialog::Add(CDebugUIWidget* pWidget)
{
	UN_ASSERT(pWidget != UN_NULL);

	UN_BOOL ret = m_List.AddTail(pWidget->GetListItem());
	return ret;
}

/**
* �S�폜
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
* �L���t���O�Z�b�g
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
* �\���t���O�Z�b�g
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
