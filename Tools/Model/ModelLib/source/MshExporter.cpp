#include "MshExporter.h"
#include "GeometryChunk.h"

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

CMshExporter CMshExporter::s_cInstance;

CMshExporter::CMshExporter()
{
}

CMshExporter::~CMshExporter()
{
	m_Out.Finalize();
}

IZ_BOOL CMshExporter::Export(
	IZ_UINT maxJointMtxNum,
	IZ_PCSTR lpszOutFile,
	IImporter* pImporter)
{
	IZ_BOOL ret = IZ_TRUE;

	VRETURN(m_Out.Open(lpszOutFile));

	ret = CGeometryChunk::GetInstance().Export(
			maxJointMtxNum,
			&m_Out,
			pImporter);

	m_Out.Finalize();

	return ret;
}

// トライアングルリストで出力するかどうかを設定.
void CMshExporter::SetIsExportTriList(IZ_BOOL flag)
{
	CGeometryChunk::GetInstance().SetIsExportTriList(flag);
}
