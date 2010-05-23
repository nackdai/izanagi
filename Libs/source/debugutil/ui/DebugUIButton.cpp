#include "debugutil/ui/DebugUIButton.h"
#include "debugutil/ui/DebugUIManager.h"
#include "unGraph.h"

using namespace uranus;

// �`��
void CDebugUIButton::Render(
	CGraphicsDevice* pDevice,
	UN_INT16 nX, UN_INT16 nY)
{
	if (!IsShow()) {
		return;
	}

	UN_ASSERT(pDevice != UN_NULL);

	CIntRect rc;
	{
		rc.left = nX + m_nX;
		rc.top = nY + m_nY;
		rc.right = rc.left + m_nWidth;
		rc.bottom = rc.left + m_nHeight;
	}

	// ���n�`��
	pDevice->Draw2DRect(
		rc,
		(m_nState == STATE_NORMAL
			? CColor::WHITE
			: CColor::GRAY));

	// �t�H���g���C�A�E�^�擾
	CDebugUIFontLayout* pFontLayout = CDebugUIManager::GetInstance()->GetFontLayout();

	// �����`��
	pFontLayout->EnableScissor(UN_TRUE);
	pFontLayout->SetRect(rc);
	pFontLayout->RenderText(
		pDevice,
		m_pText);
}

// �͈͓����ǂ���
UN_BOOL CDebugUIButton::IsInRect(UN_INT nX, UN_INT nY)
{
	CIntRect rc;
	{
		rc.left = m_nX;
		rc.top = m_nY;
		rc.right = rc.left + m_nWidth;
		rc.bottom = rc.top + m_nHeight;
	}

	return rc.PtInRect(nX, nY);
}

UN_BOOL CDebugUIButton::OnMouseLButtonDown(UN_INT nX, UN_INT nY)
{
	// �͈͓����ǂ���
	if (!IsInRect(nX, nY)) {
		// �͈͓��łȂ��̂ŉ������Ȃ�
		return UN_FALSE;
	}

	// ������Ԃɂ���
	m_nState = STATE_PRESS;

	return UN_TRUE;
}

UN_BOOL CDebugUIButton::OnMouseLButtonUp(UN_INT nX, UN_INT nY)
{
	// �͈͓����ǂ���
	if (!IsInRect(nX, nY)) {
		// �͈͓��łȂ��̂ŉ������Ȃ�
		return UN_FALSE;
	}

	if (m_nState == STATE_PRESS) {
		if (m_pClickCallback != UN_NULL) {
			// �R�[���o�b�N�Ă�
			(*m_pClickCallback)();
		}

		// �ʏ���
		m_nState = STATE_NORMAL;

		return UN_TRUE;
	}

	return UN_FALSE;
}

UN_BOOL CDebugUIButton::OnMouseMove(UN_INT nX, UN_INT nY)
{
	UN_BOOL ret = IsInRect(nX, nY);

	if (!ret) {
		if (m_nState = STATE_PRESS) {
			// �͈͊O�ɂł�
			m_nState = STATE_NORMAL;
		}
	}

	return ret;
}
