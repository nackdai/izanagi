#include "Importer.h"
#include "../importer/assimp/AssimpImporter.h"
#include "../importer/x_file/XFileImporter.h"
#include "../importer/pmd/PMDImporter.h"
#include "../importer/json/JSONImporter.h"

IImporter::EnviromentParam IImporter::s_EnvParam;

IImporter* IImporter::CreateImporter(ModelType type)
{
    IImporter* ret = IZ_NULL;

    switch (type)
    {
    case ModelTypeCollada:
        ret = new AssimpImporter();
        break;
    case ModelTypeXFile:
        ret = new CXFileImporter();
        break;
    case ModelTypePMD:
        ret = new CPmdImporter();
        break;
    case ModelTypeJSON:
        ret = new CJsonImporter();
        break;
    case ModelTypeFBX:
        break;
    default:
        IZ_ASSERT(IZ_FALSE);
        break;
    }

    return ret;
}

void IImporter::SetEnableTexIdx(IZ_INT idx)
{
    s_EnvParam.idxEnableTex = idx;
}

void IImporter::SetShaderName(const char* name)
{
    s_EnvParam.nameShader = name;
}
