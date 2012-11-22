#include "ImageReader.h"
#include "IMGBody.h"

using namespace izanagi;
using namespace tool;

CImageReader CImageReader::s_cInstance;

/**
* 読み込み
*/
CIMGMaster* CImageReader::Read(
	LPCSTR lpszPath,
	graph::E_GRAPH_TEX_TYPE nType/*= graph::E_GRAPH_TEX_TYPE_PLANE*/)
{
	// とりあえず作る
	CIMGMaster* master = new CIMGMaster();
	IZ_ASSERT(master != IZ_NULL);

	// 読み込み
	IZ_BOOL result = master->Read(lpszPath);

	if (!result) {
		// もうひとつの可能性にかける
		if (m_pReadFunc != IZ_NULL) {
			CIMGTexture* pTex = (*m_pReadFunc)(lpszPath, nType);
			result = (pTex != IZ_NULL);

			IZ_ASSERT(result);

			if (result) {
				master->Add(pTex);
			}
		}
	}

	if (!result) {
		SAFE_DELETE(master);
		IZ_ASSERT(IZ_FALSE);
	}

	return master;
}
