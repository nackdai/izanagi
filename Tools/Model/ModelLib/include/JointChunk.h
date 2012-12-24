#if !defined(__MODEL_LIB_JOINT_CHUNK_H__)
#define __MODEL_LIB_JOINT_CHUNK_H__

#include "izIo.h"
#include "izSceneGraph.h"
#include "Importer.h"

class CJointChunk {
protected:
    CJointChunk();
    ~CJointChunk();

public:
    static IZ_BOOL Export(
        izanagi::IOutputStream* pOut,
        IImporter* pImporter);

protected:
    static void GetJointInfo(
        izanagi::IOutputStream* pOut,
        IImporter* pImporter,
        std::vector<izanagi::S_SKL_JOINT>& tvJoint);

    static IZ_BOOL ExportJoint(
        izanagi::IOutputStream* pOut,
        IImporter* pImporter,
        std::vector<izanagi::S_SKL_JOINT>& tvJoint);
};

#endif  // #if !defined(__MODEL_LIB_JOINT_CHUNK_H__)
