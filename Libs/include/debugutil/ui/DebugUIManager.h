#if !defined(__URANUS_DEBUGUTIL_DEBUG_UI_MANAGER_H__)
#define __URANUS_DEBUGUTIL_DEBUG_UI_MANAGER_H__

#include "unDefs.h"
#include "unStd.h"
#include "DebugUIWidget.h"
#include "DebugUIFontRenderer.h"
#include "DebugUIFontLayout.h"

namespace uranus {
	class CDebugUIDialog;

	/**
	*/
	class CDebugUIManager : public CObject {
		friend class CDebugUIDialog;

	private:
		static CDebugUIManager* s_pInstance;

	public:
		// �C���X�^���X�擾
		static CDebugUIManager* GetInstance() { return s_pInstance; }

		// �C���X�^���X�쐬
		CDebugUIManager* CreateDebugUIManager(IMemoryAllocator* pAllocator);

	protected:
		inline CDebugUIManager();
		inline ~CDebugUIManager();

		CDebugUIManager(const CDebugUIManager& rhs);
		const CDebugUIManager& operator=(const CDebugUIManager& rhs);

	protected:
		// ���
		inline void InternalRelease();

	protected:
		// �_�C�A���O�ǉ�
		UN_BOOL Add(CDebugUIDialog* pDlg);

		// �_�C�A���O�S�폜
		void RemoveAll();

		// ���X�g�擾
		CStdList<CDebugUIWidget>* GetList() { return &m_DlgList; }

	protected:
		template <class _Func>
		void OnDoEvent(
			UN_INT nX, UN_INT nY,
			_Func& func);

		struct FuncOnMouseLButtonDown {
			UN_BOOL operator()(CDebugUIWidget* pWidget, UN_INT nX, UN_INT nY) { return pWidget->OnMouseLButtonDown(nX, nY); }
		};
		struct FuncOnMouseLButtonUp {
			UN_BOOL operator()(CDebugUIWidget* pWidget, UN_INT nX, UN_INT nY) { return pWidget->OnMouseLButtonUp(nX, nY); }
		};
		struct FuncOnMouseRButtonDown {
			UN_BOOL operator()(CDebugUIWidget* pWidget, UN_INT nX, UN_INT nY) { return pWidget->OnMouseRButtonDown(nX, nY); }
		};
		struct FuncOnMouseRButtonUp {
			UN_BOOL operator()(CDebugUIWidget* pWidget, UN_INT nX, UN_INT nY) { return pWidget->OnMouseRButtonUp(nX, nY); }
		};
		struct FuncOnMouseMove {
			UN_BOOL operator()(CDebugUIWidget* pWidget, UN_INT nX, UN_INT nY) { return pWidget->OnMouseMove(nX, nY); }
		};

	public:
		void OnMouseLButtonDown(UN_INT nX, UN_INT nY);
		void OnMouseLButtonUp(UN_INT nX, UN_INT nY);
		void OnMouseRButtonDown(UN_INT nX, UN_INT nY);
		void OnMouseRButtonUp(UN_INT nX, UN_INT nY);
		void OnMouseMove(UN_INT nX, UN_INT nY);

	public:
		// �t�H���g�����_�����Z�b�g
		inline UN_BOOL SetFontRenderer(IDebugUIFontRenderer* pFontRenderer);

		// �t�H���g���C�A�E�^���擾
		inline CDebugUIFontLayout* GetFontLayout();

	protected:
		IMemoryAllocator* m_pAllocator;

		// �L���v�`���E�B�W�F�b�g
		CDebugUIWidget* m_pCapture;

		// �_�C�A���O���X�g
		CStdList<CDebugUIWidget> m_DlgList;

		// �t�H���g���C�A�E�^
		CDebugUIFontLayout m_FontLayout;
	};

	// inline *******************************

	// �R���X�g���N�^
	CDebugUIManager::CDebugUIManager()
	{
		m_pAllocator = UN_NULL;
		m_pCapture = UN_NULL;
	}

	// �f�X�g���N�^
	CDebugUIManager::~CDebugUIManager()
	{
		// �_�C�A���O�S�폜
		RemoveAll();
	}

	// ���
	void CDebugUIManager::InternalRelease()
	{
		delete this;
		if (m_pAllocator != UN_NULL) {
			m_pAllocator->Free(this);
		}
	}

	/**
	* �t�H���g�����_�����Z�b�g
	*/
	UN_BOOL CDebugUIManager::SetFontRenderer(IDebugUIFontRenderer* pFontRenderer)
	{
		m_FontLayout.SetRenderer(pFontRenderer);
	}

	/**
	* �t�H���g���C�A�E�^���擾
	*/
	CDebugUIFontLayout* CDebugUIManager::GetFontLayout()
	{
		return &m_FontLayout;
	}

}	// namespace uranus

#endif	// #if !defined(__URANUS_DEBUGUTIL_DEBUG_UI_MANAGER_H__)
