#if !defined(__MODEL_LIB_MTRL_EXPORTER_H__)
#define __MODEL_LIB_MTRL_EXPORTER_H__

#include "izIo.h"
#include "Importer.h"

class CMtrlExporter {
	static CMtrlExporter s_cInstance;

public:
	static CMtrlExporter& GetInstance() { return s_cInstance; }

private:
	CMtrlExporter();
	~CMtrlExporter();

	NO_COPIABLE(CMtrlExporter);

public:
	IZ_BOOL Export(
		IZ_PCSTR lpszOutFile,
		IImporter* pImporter,
		IZ_UINT nMtrlIdx);

private:
	IZ_BOOL ExportMaterial(
		IZ_UINT nMtrlIdx,
		IImporter* pImporter);

private:
	izanagi::CFileOutputStream m_Out;
};

#endif	// #if !defined(__MODEL_LIB_MTRL_EXPORTER_H__)
