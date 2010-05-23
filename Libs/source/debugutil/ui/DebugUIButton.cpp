#include "debugutil/ui/DebugUIButton.h"
#include "debugutil/ui/DebugUIManager.h"
#include "unGraph.h"

using namespace uranus;

// 描画
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

	// 下地描画
	pDevice->Draw2DRect(
		rc,
		(m_nState == STATE_NORMAL
			? CColor::WHITE
			: CColor::GRAY));

	// フォントレイアウタ取得
	CDebugUIFontLayout* pFontLayout = CDebugUIManager::GetInstance()->GetFontLayout();

	// 文字描画
	pFontLayout->EnableScissor(UN_TRUE);
	pFontLayout->SetRect(rc);
	pFontLayout->RenderText(
		pDevice,
		m_pText);
}

// 範囲内かどうか
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
	// 範囲内かどうか
	if (!IsInRect(nX, nY)) {
		// 範囲内でないので何もしない
		return UN_FALSE;
	}

	// 押下状態にする
	m_nState = STATE_PRESS;

	return UN_TRUE;
}

UN_BOOL CDebugUIButton::OnMouseLButtonUp(UN_INT nX, UN_INT nY)
{
	// 範囲内かどうか
	if (!IsInRect(nX, nY)) {
		// 範囲内でないので何もしない
		return UN_FALSE;
	}

	if (m_nState == STATE_PRESS) {
		if (m_pClickCallback != UN_NULL) {
			// コールバック呼ぶ
			(*m_pClickCallback)();
		}

		// 通常状態
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
			// 範囲外にでた
			m_nState = STATE_NORMAL;
		}
	}

	return ret;
}
