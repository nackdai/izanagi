#include "debugutil/ui/DebugUIFontLayout.h"
#include "izGraph.h"

using namespace izanagi;

/**
* 文字列描画
*/
void CDebugUIFontLayout::RenderText(
	CGraphicsDevice* pDevice,
	IZ_PCSTR pszStr)
{
	IZ_ASSERT(pDevice != IZ_NULL);
	IZ_ASSERT(m_pFontRenderer != IZ_NULL);

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
IZ_UINT CDebugUIFontLayout::GetTextWidth(IZ_PCSTR* pszStr) const
{
	IZ_ASSERT(m_pFontRenderer != IZ_NULL);

	IZ_UINT ret = m_pFontRenderer->GetTextWidth(pszStr);
	return ret;
}

// フォント高さ取得
IZ_UINT CDebugUIFontLayout::GetFontHeight() const
{
	IZ_ASSERT(m_pFontRenderer != IZ_NULL);

	IZ_UINT ret = m_pFontRenderer->GetFontHeight();
	return ret;
}
