#include "debugutil/ui/DebugUIFontLayout.h"
#include "unGraph.h"

using namespace uranus;

/**
* 文字列描画
*/
void CDebugUIFontLayout::RenderText(
	CGraphicsDevice* pDevice,
	UN_PCSTR pszStr)
{
	UN_ASSERT(pDevice != UN_NULL);
	UN_ASSERT(m_pFontRenderer != UN_NULL);

	// シザリング
	{
		pDevice->SetRenderState(
			E_GRAPH_RS_SCISSORTESTENABLE, 
			m_Flags.is_scissor);
	
		if (m_Flags.is_scissor) {
			pDevice->SetScissorTestRect(m_Rect);
		}
	}

	int nX = m_Rect.left;
	int nY = m_Rect.top;
	
	// TODO
	m_pFontRenderer->RenderText(
		pDevice,
		nX, nY,
		m_cColor,
		pszStr);
}

// 文字列幅取得
UN_UINT CDebugUIFontLayout::GetTextWidth(UN_PCSTR* pszStr) const
{
	UN_ASSERT(m_pFontRenderer != UN_NULL);

	UN_UINT ret = m_pFontRenderer->GetTextWidth(pszStr);
	return ret;
}

// フォント高さ取得
UN_UINT CDebugUIFontLayout::GetFontHeight() const
{
	UN_ASSERT(m_pFontRenderer != UN_NULL);

	UN_UINT ret = m_pFontRenderer->GetFontHeight();
	return ret;
}
