#if !defined(__MODEL_LIB_FBX_DATA_MANAGER_H__)
#define __MODEL_LIB_FBX_DATA_MANAGER_H__

#include "izToolKit.h"
#include "fbxsdk.h"

#include <map>

struct Face {
    IZ_UINT idx[3];
};

struct Mesh {
    FbxMesh* mesh;
    std::map <std::string, std::vector<Face>> subsets;
};

struct Vertex {
    IZ_FLOAT pos[3];
    IZ_FLOAT nml[3];
    IZ_FLOAT uv[2];
};

class FbxDataManager {
private:
    static FbxDataManager s_Instance;

public:
    static FbxDataManager& Instance()
    {
        return s_Instance;
    }

private:
    FbxDataManager() {}
    ~FbxDataManager() {}

public:
    IZ_BOOL Open(const char* path);

    void Close();

    IZ_UINT GetMeshSetsNum() const;

    IZ_UINT GetVertexNum() const;

    IZ_UINT CovertMeshVtxIdxToEntireVtxIdx(
        FbxMesh* mesh,
        IZ_UINT idx) const;

    std::vector<FbxMesh*>& GetMeshes();

    IZ_UINT GetNodeIndex(FbxNode* node) const;

    std::vector<Face>& GetFaces(IZ_UINT idx);

private:
    // ノードを集める.
    void GatherNodes(FbxNode* node);

    // メッシュを集める
    void GatherMeshes();

    void GatherFaces();

private:
    FbxManager* m_manager{ nullptr };
    FbxScene* m_scene{ nullptr };

    std::vector<FbxNode*> m_nodes;
    std::vector<Mesh> m_meshes;
    std::map<std::string, FbxSurfaceMaterial*> m_mtrls;

    std::vector<Vertex> m_vtx;
};

#endif  // #if !defined(__MODEL_LIB_FBX_DATA_MANAGER_H__)
