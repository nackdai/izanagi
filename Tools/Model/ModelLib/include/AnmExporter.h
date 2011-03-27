#if !defined(__MODEL_LIB_ANM_EXPORTER_H__)
#define __MODEL_LIB_ANM_EXPORTER_H__

#include "izIo.h"
#include "Importer.h"

class CAnmExporter {
	static CAnmExporter s_cInstance;

public:
	static CAnmExporter& GetInstance() { return s_cInstance; }

private:
	CAnmExporter();
	~CAnmExporter();

	NO_COPIABLE(CAnmExporter);

public:
	IZ_BOOL Export(
		IZ_PCSTR lpszOutFile,
		IZ_UINT nSetIdx,
		IImporter* pImporter);

private:
	izanagi::CFileOutputStream m_Out;
};

#endif	// #if !defined(__MODEL_LIB_ANM_EXPORTER_H__)
