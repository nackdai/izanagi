#if !defined(__MODEL_LIB_TOOL_OPTION_H__)
#define __MODEL_LIB_TOOL_OPTION_H__

#include "Importer.h"

template <typename T>
class CToolOption : public T
{
public:
	ModelType modelType;

public:
	CToolOption()
	{
		modelType = ModelTypeUnknown;
	}

	~CToolOption() {}

public:
	IZ_BOOL AnalysisModelType(const std::string& ext)
	{
		IZ_BOOL ret = IZ_FALSE;

		if (ret = (ext == "dae")) {
			modelType = ModelTypeCollada;
		}
		else if (ret = (ext == "x")) {
			modelType = ModelTypeXFile;
		}
		else if (ret = (ext == "fbx")) {
			modelType = ModelTypeFBX;
		}
		else if (ret = ((ext == "pmd") || (ext == "vmd"))) {
			modelType = ModelTypePMD;
		}

		return ret;
	}

	IZ_BOOL IsValidModelType()
	{
		return (modelType != ModelTypeUnknown);
	}
};

#endif	// #if !defined(__MODEL_LIB_TOOL_OPTION_H__)
