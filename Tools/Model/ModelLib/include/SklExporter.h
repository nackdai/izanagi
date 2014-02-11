#if !defined(__MODEL_LIB_SKL_EXPORTER_H__)
#define __MODEL_LIB_SKL_EXPORTER_H__

#include "izToolKit.h"
#include "Importer.h"

class CSklExporter {
    static CSklExporter s_cInstance;

public:
    static CSklExporter& GetInstance() { return s_cInstance; }

private:
    CSklExporter();
    ~CSklExporter();

    NO_COPIABLE(CSklExporter);

public:
    IZ_BOOL Export(
        IZ_PCSTR lpszOutFile,
        IImporter* pImporter);

    IZ_BOOL ExportBoneInfo(
        IZ_PCSTR outFile,
        IImporter* importer);

private:
    izanagi::CFileOutputStream m_Out;
};

#endif  // #if !defined(__MODEL_LIB_SKL_EXPORTER_H__)
