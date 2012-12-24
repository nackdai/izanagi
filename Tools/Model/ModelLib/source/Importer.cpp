#include "Importer.h"
#include "../importer/collada/ColladaImporter.h"
#include "../importer/x_file/XFileImporter.h"
#include "../importer/pmd/PMDImporter.h"

IImporter* IImporter::CreateImporter(ModelType type)
{
    IImporter* ret = IZ_NULL;

    switch (type)
    {
    case ModelTypeCollada:
        ret = new CColladaImporter();
        break;
    case ModelTypeXFile:
        ret = new CXFileImporter();
        break;
    case ModelTypeFBX:
        IZ_ASSERT(IZ_FALSE);
        break;
    case ModelTypePMD:
        ret = new CPmdImporter();
        break;
    default:
        IZ_ASSERT(IZ_FALSE);
        break;
    }

    return ret;
}
