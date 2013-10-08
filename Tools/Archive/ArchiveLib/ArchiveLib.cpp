#include <vector>
#include "ArchiveLib.h"
#include "izResource.h"
#include "izToolKit.h"

class CArchiveExporter
{
public:
    CArchiveExporter(
        char* path,
        int fileNum, int maxFileSize);
    ~CArchiveExporter();

public:
    void Register(char* name, char* path);

private:
    void ExportNameChunk();
    void ExportDataChunk();
    void ExportHeaderChunk();

private:
    FILE* m_Dst;
    izanagi::resource::S_ARC_HEADER m_Header;

    std::vector<izanagi::tool::CString> m_NameList;
    std::vector<izanagi::tool::CString> m_PathList;

    std::vector<izanagi::resource::S_ARC_FILE_HEADER> m_HeaderList;
};

CArchiveExporter::CArchiveExporter(
    char* path,
    int fileNum, int maxFileSize)
{
    m_Header.magic = izanagi::resource::ARC_MAGIC_NUMBER;
    m_Header.version = izanagi::resource::E_ARC_VERSION_CURRENT;

    m_Header.numFiles = fileNum;
    m_Header.maxFileSize = maxFileSize;

    m_Dst = NULL;
    ::fopen_s(&m_Dst, path, "wb");
    ::fseek(m_Dst, sizeof(m_Header), SEEK_SET);

    m_HeaderList.resize(fileNum);
}

void CArchiveExporter::Register(char* name, char* path)
{
    izanagi::tool::CString strName(path);
    m_NameList.push_back(strName);

    izanagi::tool::CString strPath(path);
    m_PathList.push_back(strPath);
}

void CArchiveExporter::ExportNameChunk()
{
    for (size_t i = 0; i < m_NameList.size(); i++) {
        izanagi::tool::CString name = m_NameList[i];

        size_t size = name.size();
        size_t padding = (size & 0x03 == 0
            ? 0
            : 4 - (size & 0x03));

        m_HeaderList[i].posName = ::ftell(m_Dst);
        m_HeaderList[i].sizeName = size + padding;
        m_HeaderList[i].key = izanagi::CKey::GenerateValue(name.c_str());

        // TODO
    }
}

void CArchiveExporter::ExportDataChunk()
{
    static std::vector<BYTE> tmp(1 * 1024 * 1024);

    for (size_t i = 0; i < m_PathList.size(); i++) {
        izanagi::tool::CString path = m_PathList[i];

        FILE* fp = NULL;
        ::fopen_s(&fp, path.c_str(), "rb");

        if (fp != NULL) {
            m_HeaderList[i].pos = ::ftell(m_Dst);

            for (;;) {
                size_t readSize = ::fread_s(&tmp[0], tmp.size(), sizeof(BYTE), tmp.size(), fp);

                if (readSize > 0) {
                    m_HeaderList[i].size += readSize;

                    // TODO
                }
                else {
                    // ‚·‚×‚Ä“Ç‚Ýž‚ñ‚¾
                    break;
                }
            }
        }
    }
}

static CArchiveExporter* exporter = IZ_NULL;

void BeginExport(
    char* path,
    int fileNum, int maxFileSize)
{
    SAFE_DELETE(exporter);
    exporter = new CArchiveExporter(fileNum, maxFileSize);
}

void EndExport()
{
    // TODO

    SAFE_DELETE(exporter);
}

void Register(char* name, char* path)
{
    exporter->Register(name, path);
}
