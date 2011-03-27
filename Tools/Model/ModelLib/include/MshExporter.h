#if !defined(__MODEL_LIB_MSH_EXPORTER_H__)
#define __MODEL_LIB_MDL_EXPORTER_H__

#include "izIo.h"
#include "Importer.h"

class CMshExporter {
	static CMshExporter s_cInstance;

public:
	static CMshExporter& GetInstance() { return s_cInstance; }

private:
	CMshExporter();
	~CMshExporter();

	NO_COPIABLE(CMshExporter);

public:
	IZ_BOOL Export(
		IZ_PCSTR lpszOutFile,
		IImporter* pImporter);

private:
	izanagi::CFileOutputStream m_Out;
};

#endif	// #if !defined(__MODEL_LIB_MDL_EXPORTER_H__)
