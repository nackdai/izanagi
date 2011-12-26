#include "debugutil/ui/DebugUIButton.h"
#include "debugutil/ui/DebugUIManager.h"
#include "izGraph.h"

using namespace izanagi;

// 描画
void CDebugUIButton::Render(
	CGraphicsDevice* pDevice,
	IZ_INT16 nX, IZ_INT16 nY)
{
	if (!IsShow()) {
		return;
	}

	IZ_ASSERT(pDevice != IZ_NULL);

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
	pFontLayout->EnableScissor(IZ_TRUE);
	pFontLayout->SetRect(rc);
	pFontLayout->RenderText(
		pDevice,
		m_pText);
}

// 範囲内かどうか
IZ_BOOL CDebugUIButton::IsInRect(IZ_INT nX, IZ_INT nY)
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

IZ_BOOL CDebugUIButton::OnMouseLButtonDown(IZ_INT nX, IZ_INT nY)
{
	// 範囲内かどうか
	if (!IsInRect(nX, nY)) {
		// 範囲内でないので何もしない
		return IZ_FALSE;
	}

	// 押下状態にする
	m_nState = STATE_PRESS;

	return IZ_TRUE;
}

IZ_BOOL CDebugUIButton::OnMouseLButtonUp(IZ_INT nX, IZ_INT nY)
{
	// 範囲内かどうか
	if (!IsInRect(nX, nY)) {
		// 範囲内でないので何もしない
		return IZ_FALSE;
	}

	if (m_nState == STATE_PRESS) {
		if (m_pClickCallback != IZ_NULL) {
			// コールバック呼ぶ
			(*m_pClickCallback)();
		}

		// 通常状態
		m_nState = STATE_NORMAL;

		return IZ_TRUE;
	}

	return IZ_FALSE;
}

IZ_BOOL CDebugUIButton::OnMouseMove(IZ_INT nX, IZ_INT nY)
{
	IZ_BOOL ret = IsInRect(nX, nY);

	if (!ret) {
		if (m_nState = STATE_PRESS) {
			// 範囲外にでた
			m_nState = STATE_NORMAL;
		}
	}

	return ret;
}
