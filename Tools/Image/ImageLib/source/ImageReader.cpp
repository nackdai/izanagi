#include "ImageReader.h"
#include "IMGBody.h"

using namespace izanagi;
using namespace izanagi_tk;

CImageReader CImageReader::s_cInstance;

/**
* 読み込み
*/
CIMGBody* CImageReader::Read(
	LPCSTR lpszPath,
	E_GRAPH_TEX_TYPE nType/*= E_GRAPH_TEX_TYPE_PLANE*/)
{
	// とりあえず作る
	CIMGBody* pIMGBody = new CIMGBody();
	IZ_ASSERT(pIMGBody != IZ_NULL);

	// 読み込み
	IZ_BOOL result = pIMGBody->Read(lpszPath);

	if (!result) {
		// もうひとつの可能性にかける
		if (m_pReadFunc != IZ_NULL) {
			CIMGTexture* pTex = (*m_pReadFunc)(lpszPath, nType);
			result = (pTex != IZ_NULL);

			IZ_ASSERT(result);

			if (result) {
				pIMGBody->Add(pTex);
			}
		}
	}

	if (!result) {
		SAFE_DELETE(pIMGBody);
		IZ_ASSERT(IZ_FALSE);
	}

	return pIMGBody;
}
