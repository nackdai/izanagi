#if !defined(__MODEL_LIB_MDL_EXPORTER_H__)
#define __MODEL_LIB_MDL_EXPORTER_H__

#include "izToolKit.h"
#include "Importer.h"

class CMdlExporter {
    static CMdlExporter s_cInstance;

public:
    static CMdlExporter& GetInstance() { return s_cInstance; }

private:
    CMdlExporter();
    ~CMdlExporter();

    NO_COPIABLE(CMdlExporter);

public:
    IZ_BOOL Export(
        IZ_UINT maxJointMtxNum,
        IZ_PCSTR lpszOutFile,
        IImporter* pImporter);

private:
    izanagi::CFileOutputStream m_Out;
};

#endif  // #if !defined(__MODEL_LIB_MDL_EXPORTER_H__)
