#include "SklExporter.h"
#include "JointChunk.h"
#include "izToolKit.h"

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

IZ_BOOL CSklExporter::ExportBoneInfo(
    IZ_PCSTR outFile,
    IImporter* importer)
{
    IZ_BOOL ret = IZ_TRUE;

    VRETURN(m_Out.Open(outFile, izanagi::E_IO_FILE_MODE_TEXT));

    IZ_UINT num = importer->GetJointNum();

    for (IZ_UINT i = 0; i < num; i++)
    {
        IZ_PCSTR name = importer->GetJointName(i);

        izanagi::tool::CString str;
        str.format("%d:%s\n", i, name);

        m_Out.Write(str.c_str());
    }

    m_Out.Finalize();

    return ret;
}
