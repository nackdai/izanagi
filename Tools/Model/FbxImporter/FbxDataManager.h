#if !defined(__MODEL_LIB_FBX_DATA_MANAGER_H__)
#define __MODEL_LIB_FBX_DATA_MANAGER_H__

#include "izToolKit.h"
#include "fbxsdk.h"

#include <map>
#include <set>

struct Vertex {
    // メッシュ内のインデックス.
    IZ_UINT idxInFbxMesh;

    // 全体を通じてのインデックス.
    IZ_UINT allIdx;

    // 所属メッシュ.
    FbxMesh* mesh;

    Vertex() {}

    Vertex(FbxMesh* _mesh, IZ_UINT idx)
        : mesh(_mesh), idxInFbxMesh(idx)
    {}

    bool operator==(const Vertex& rhs)
    {
        return (idxInFbxMesh == rhs.idxInFbxMesh && mesh == rhs.mesh);
    }
};

struct Face {
    Vertex vtx[3];

    Face() {}
};

struct Mesh {
    std::vector<Face> faces;
    FbxSurfaceMaterial* mtrl = nullptr;

    IZ_UINT vtxNum = 0;

    Mesh() {}

    Mesh(FbxSurfaceMaterial* _mtrl)
        : mtrl(_mtrl)
    {}

    bool operator==(const Mesh& rhs)
    {
        return (mtrl == rhs.mtrl);
    }
};

struct VectorData {
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

    IZ_UINT GetFbxMeshNum() const;

    FbxMesh* GetFbxMesh(IZ_UINT idx);

    IZ_UINT GetMeshNum() const;

    Mesh& GetMesh(IZ_UINT idx);
    const Mesh& GetMesh(IZ_UINT idx) const;

    IZ_UINT GetVtxNum() const;

    const Vertex& GetVertex(IZ_UINT idx) const;

    IZ_UINT GetNodeNum() const;

    FbxNode* GetNode(IZ_UINT idx);

    IZ_UINT GetNodeIndex(const FbxNode* node) const;

    FbxCluster* GetClusterByNode(const FbxNode* node);

    IZ_UINT ConvertToEntireVtxIdx(FbxMesh* mesh, IZ_UINT vtxIdxInMesh);

private:
    // ノードを集める.
    void GatherNodes(FbxNode* node);

    void GatherMeshes();

    void GatherClusters();

    void GatherFaces();

    void GatherVertices();

private:
    FbxManager* m_manager{ nullptr };
    FbxScene* m_scene{ nullptr };

    std::vector<FbxNode*> m_nodes;
    std::vector<FbxMesh*> m_fbxMeshes;

    std::vector<FbxCluster*> m_clusters;

    std::vector<Vertex*> m_vertices;

    std::vector<Mesh> m_meshes;

public:
    std::map<FbxMesh*, std::vector<VectorData>> m_vtxDatas;
};

#endif  // #if !defined(__MODEL_LIB_FBX_DATA_MANAGER_H__)
