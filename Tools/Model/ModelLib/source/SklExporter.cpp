#include "SklExporter.h"
#include "JointChunk.h"

// フォーマット
// +--------------------+
// |   ファイルヘッダ   |
// +--------------------+
// | ジオメトリチャンク |
// +--------------------+
// | ジョイントチャンク |
// +--------------------+
// | マテリアルチャンク |
// +--------------------+

CSklExporter CSklExporter::s_cInstance;

CSklExporter::CSklExporter()
{
}

CSklExporter::~CSklExporter()
{
	m_Out.Finalize();
}

IZ_BOOL CSklExporter::Export(
	IZ_PCSTR lpszOutFile,
	IImporter* pImporter)
{
	IZ_BOOL ret = IZ_TRUE;

	VRETURN(m_Out.Open(lpszOutFile));

	ret = CJointChunk::Export(
			&m_Out,
			pImporter);

	m_Out.Finalize();

	return ret;
}
