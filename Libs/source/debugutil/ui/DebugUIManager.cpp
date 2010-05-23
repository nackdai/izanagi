#include "debugutil/ui/DebugUIManager.h"
#include "debugutil/ui/DebugUIDialog.h"

using namespace uranus;

CDebugUIManager* CDebugUIManager::s_pInstance = UN_NULL;

/**
* �C���X�^���X�쐬
*/
CDebugUIManager* CDebugUIManager::CreateDebugUIManager(IMemoryAllocator* pAllocator)
{
	if (s_pInstance != UN_NULL) {
		return s_pInstance;
	}

	UN_ASSERT(pAllocator != UN_NULL);

	// �������m��
	UN_BYTE* pBuf = (UN_BYTE*)ALLOC_ZERO(pAllocator, sizeof(CDebugUIManager));
	UN_BOOL result = (pBuf != UN_NULL);
	VGOTO(!result, __EXIT__);

	// �C���X�^���X�쐬
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

// �_�C�A���O�ǉ�
UN_BOOL CDebugUIManager::Add(CDebugUIDialog* pDlg)
{
	UN_ASSERT(pDlg != UN_NULL);

	UN_BOOL ret = m_DlgList.AddTail(pDlg->GetListItem());
	return ret;
}

// �_�C�A���O�S�폜
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

// �E�B�W�F�b�g�C�x���g����
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
* �}�E�X���{�^��������
*/
void CDebugUIManager::OnMouseLButtonDown(UN_INT nX, UN_INT nY)
{
	OnDoEvent(
		nX, nY,
		FuncOnMouseLButtonDown());
}

/**
* �}�E�X���{�^��������
*/
void CDebugUIManager::OnMouseLButtonUp(UN_INT nX, UN_INT nY)
{
	OnDoEvent(
		nX, nY,
		FuncOnMouseLButtonUp());
}

/**
* �}�E�X�E�{�^��������
*/
void CDebugUIManager::OnMouseRButtonDown(UN_INT nX, UN_INT nY)
{
	OnDoEvent(
		nX, nY,
		FuncOnMouseRButtonDown());
}

/**
* �}�E�X�E�{�^��������
*/
void CDebugUIManager::OnMouseRButtonUp(UN_INT nX, UN_INT nY)
{
	OnDoEvent(
		nX, nY,
		FuncOnMouseRButtonUp());
}

/**
* �}�E�X�ړ�
*/
void CDebugUIManager::OnMouseMove(UN_INT nX, UN_INT nY)
{
	OnDoEvent(
		nX, nY,
		FuncOnMouseMove());
}
