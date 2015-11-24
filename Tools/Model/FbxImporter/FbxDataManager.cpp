#include "FbxDataManager.h"

FbxDataManager FbxDataManager::s_Instance;

IZ_BOOL FbxDataManager::Open(const char* path)
{
    m_manager = FbxManager::Create();

    FbxIOSettings* ios = FbxIOSettings::Create(m_manager, IOSROOT);
    m_manager->SetIOSettings(ios);

    m_scene = FbxScene::Create(m_manager, "");

    auto importer = FbxImporter::Create(m_manager, "");

    int fileFormat = -1;

    if (m_manager->GetIOPluginRegistry()->DetectReaderFileFormat(path, fileFormat)) {
        if (importer->Initialize(path, fileFormat)) {
            if (importer->Import(m_scene)) {
                importer->Destroy(true);

#if 0
                // Convert Axis System to what is used in this example, if needed
                FbxAxisSystem SceneAxisSystem = m_scene->GetGlobalSettings().GetAxisSystem();
                FbxAxisSystem axisSystem(
                    FbxAxisSystem::eYAxis,
                    FbxAxisSystem::eParityOdd,
                    FbxAxisSystem::eLeftHanded);

                if (SceneAxisSystem != axisSystem)
                {
                    axisSystem.ConvertScene(m_scene);
                }
#endif

                // Convert Unit System to what is used in this example, if needed
                FbxSystemUnit SceneSystemUnit = m_scene->GetGlobalSettings().GetSystemUnit();
                if (SceneSystemUnit.GetScaleFactor() != 1.0)
                {
                    //The unit in this tool is centimeter.
                    FbxSystemUnit::cm.ConvertScene(m_scene);
                }

                // Convert mesh, NURBS and patch into triangle mesh
                FbxGeometryConverter geomConverter(m_manager);
                geomConverter.Triangulate(m_scene, true);

                GatherNodes(m_scene->GetRootNode());
                GatherMeshes();
                GatherClusters();
                GatherFaces();

                GatherVertices();

                return IZ_TRUE;
            }
        }
    }

    IZ_ASSERT(IZ_FALSE);
    return IZ_FALSE;
}

void FbxDataManager::Close()
{
    if (m_scene) {
        m_scene->Destroy(true);
    }

    if (m_manager) {
        // Delete the FBX SDK manager. All the objects that have been allocated 
        // using the FBX SDK manager and that haven't been explicitly destroyed 
        // are automatically destroyed at the same time.
        m_manager->Destroy();
    }
}

IZ_UINT FbxDataManager::GetFbxMeshNum() const
{
    IZ_UINT ret = m_fbxMeshes.size();
    return ret;
}

FbxMesh* FbxDataManager::GetFbxMesh(IZ_UINT idx)
{
    IZ_ASSERT(idx < m_fbxMeshes.size());

    FbxMesh* ret = m_fbxMeshes[idx];
    return ret;
}

IZ_UINT FbxDataManager::GetMeshNum() const
{
    IZ_UINT ret = m_meshes.size();
    return ret;
}

Mesh& FbxDataManager::GetMesh(IZ_UINT idx)
{
    IZ_ASSERT(idx < m_meshes.size());
    return m_meshes[idx];
}


const Mesh& FbxDataManager::GetMesh(IZ_UINT idx) const
{
    IZ_ASSERT(idx < m_meshes.size());
    return m_meshes[idx];
}

IZ_UINT FbxDataManager::GetVtxNum() const
{
    IZ_UINT ret = m_vertices.size();
    return ret;
}

const Vertex& FbxDataManager::GetVertex(IZ_UINT idx) const
{
    return *m_vertices[idx];
}

IZ_UINT FbxDataManager::GetNodeNum() const
{
    return m_nodes.size();
}

FbxNode* FbxDataManager::GetNode(IZ_UINT idx)
{
    return m_nodes[idx];
}

IZ_UINT FbxDataManager::GetNodeIndex(const FbxNode* node) const
{
    auto it = std::find(m_nodes.begin(), m_nodes.end(), node);

    if (it == m_nodes.end()) {
        return -1;
    }

    auto dist = std::distance(m_nodes.begin(), it);

    return dist;
}

FbxCluster* FbxDataManager::GetClusterByNode(const FbxNode* node)
{
    for each (auto cluster in m_clusters)
    {
        FbxNode* targetNode = cluster->GetLink();

        if (node == targetNode) {
            return cluster;
        }
    }

    return nullptr;
}

IZ_UINT FbxDataManager::ConvertToEntireVtxIdx(FbxMesh* mesh, IZ_UINT vtxIdxInMesh)
{
    Vertex vtx(mesh, vtxIdxInMesh);

    for each (auto v in m_vertices)
    {
        if (*v == vtx)
        {
            return v->allIdx;
        }
    }

    IZ_ASSERT(IZ_FALSE);

    return vtxIdxInMesh;
}

// ノードを集める.
void FbxDataManager::GatherNodes(FbxNode* node)
{
    // NOTE
    // RootNodeは除外する.
    if (node != m_scene->GetRootNode()) {
        m_nodes.push_back(node);
    }

    for (IZ_UINT i = 0; i < node->GetChildCount(); i++) {
        FbxNode* child = node->GetChild(i);
        GatherNodes(child);
    }
}

// メッシュを集める
void FbxDataManager::GatherMeshes()
{
    // シーンに含まれるメッシュの解析
    auto meshCount = m_scene->GetMemberCount<FbxMesh>();

    for (int i = 0; i < meshCount; ++i)
    {
        FbxMesh* mesh = m_scene->GetMember<FbxMesh>(i);
        m_fbxMeshes.push_back(mesh);
    }
}

void FbxDataManager::GatherClusters()
{
    for each (FbxMesh* mesh in m_fbxMeshes)
    {
        // メッシュに含まれるスキニング情報数.
        int skinCount = mesh->GetDeformerCount(FbxDeformer::EDeformerType::eSkin);

        for (int n = 0; n < skinCount; n++) {
            // スキニング情報を取得.
            FbxDeformer* deformer = mesh->GetDeformer(n, FbxDeformer::EDeformerType::eSkin);

            FbxSkin* skin = (FbxSkin*)deformer;

            // スキニングに影響を与えるボーンの数.
            int boneCount = skin->GetClusterCount();

            for (int b = 0; b < boneCount; b++) {
                FbxCluster* cluster = skin->GetCluster(b);

                auto it = std::find(m_clusters.begin(), m_clusters.end(), cluster);
                if (it == m_clusters.end()) {
                    m_clusters.push_back(cluster);
                }
            }
        }
    }
}

void FbxDataManager::GatherFaces()
{
    IZ_UINT offset = 0;

    // シーンに含まれるメッシュの解析
    auto meshCount = m_scene->GetMemberCount<FbxMesh>();

    for (int i = 0; i < meshCount; ++i)
    {
        FbxMesh* fbxMesh = m_scene->GetMember<FbxMesh>(i);

        if (fbxMesh->GetElementMaterial() == NULL) {
            continue;
        }

        // メッシュに含まれるポリゴン（三角形）の数.
        const int polygonCount = fbxMesh->GetPolygonCount();

        auto& materialIndices = fbxMesh->GetElementMaterial()->GetIndexArray();

        if (materialIndices.GetCount() == polygonCount)
        {
            // メッシュに含まれるポリゴン（三角形）ごとにどのマテリアルに所属しているのかを調べる.
            for (int i = 0; i < polygonCount; i++)
            {
                // メッシュに含まれるポリゴン（三角形）が所属しているマテリアルへのインデックス.
                const int materialIdx = materialIndices.GetAt(i);

                // マテリアル本体を取得.
                auto material = m_scene->GetMaterial(materialIdx);

                // 登録済みメッシュを探す.
                auto it = std::find(m_meshes.begin(), m_meshes.end(), Mesh(material));

                Mesh* mesh = nullptr;

                // 未登録だった.
                if (it == m_meshes.end())
                {
                    m_meshes.push_back(Mesh(material));
                    auto rit = m_meshes.rbegin();
                    mesh = &(*rit);
                }
                else
                {
                    mesh = &(*it);
                }
                

                Face face;

                // 三角形なので３点.
                face.vtx[0].idxInFbxMesh = fbxMesh->GetPolygonVertex(i, 0);
                face.vtx[1].idxInFbxMesh = fbxMesh->GetPolygonVertex(i, 1);
                face.vtx[2].idxInFbxMesh = fbxMesh->GetPolygonVertex(i, 2);

                face.vtx[0].mesh = fbxMesh;
                face.vtx[1].mesh = fbxMesh;
                face.vtx[2].mesh = fbxMesh;

                mesh->faces.push_back(face);
            }
        }
        else {
            IZ_ASSERT(IZ_FALSE);
        }
    }

    // 頂点を集める.
    for (IZ_UINT m = 0; m < m_meshes.size(); m++)
    {
        IZ_UINT curVtxNum = m_vertices.size();

        Mesh& mesh = m_meshes[m];

        for (IZ_UINT n = 0; n < mesh.faces.size(); n++)
        {
            Face& face = mesh.faces[n];

            IZ_BOOL found[3] = { IZ_FALSE };
            IZ_UINT foundIdx[3] = { 0 };

            for each (auto& vtx in m_vertices)
            {
                IZ_BOOL isFound = IZ_FALSE;

                if (*vtx == face.vtx[0])
                {
                    found[0] = IZ_TRUE;
                    foundIdx[0] = vtx->allIdx;
                    isFound = IZ_TRUE;
                }
                if (*vtx == face.vtx[1])
                {
                    found[1] = IZ_TRUE;
                    foundIdx[1] = vtx->allIdx;
                    isFound = IZ_TRUE;
                }
                if (*vtx == face.vtx[2])
                {
                    found[2] = IZ_TRUE;
                    foundIdx[2] = vtx->allIdx;
                    isFound = IZ_TRUE;
                }

                if (isFound)
                {
                    break;
                }
            }

            for (IZ_UINT i = 0; i < COUNTOF(found); i++)
            {
                if (!found[i])
                {
                    // 全体を通じた頂点インデックスをセット.
                    face.vtx[i].allIdx = m_vertices.size();

                    m_vertices.push_back(&face.vtx[i]);
                }
                else {
                    face.vtx[i].allIdx = foundIdx[i];
                }
            }
        }

        mesh.vtxNum = m_vertices.size() - curVtxNum;
    }
}

void FbxDataManager::GatherVertices()
{
    auto meshCount = m_scene->GetMemberCount<FbxMesh>();

    for (int i = 0; i < meshCount; ++i)
    {
        FbxMesh* fbxMesh = m_scene->GetMember<FbxMesh>(i);

        FbxLayerElementUV* leUV = fbxMesh->GetLayer(0)->GetUVs();

        auto vtxCnt = fbxMesh->GetControlPointsCount();

        for (int v = 0; v < vtxCnt; v++) {
            IZ_UINT UVIndex = 0;

            auto polygonCnt = fbxMesh->GetPolygonCount();

            for (int p = 0; p < polygonCnt; p++) {
                for (int n = 0; n < 3; n++) {
                    if (v == fbxMesh->GetPolygonVertex(p, n)) {
                        auto it = m_vtxDatas.find(fbxMesh);

                        if (it == m_vtxDatas.end()) {
                            m_vtxDatas.insert(std::make_pair(fbxMesh, std::vector<VectorData>()));
                            it = m_vtxDatas.find(fbxMesh);
                        }

                        auto& list = it->second;

                        if (list.size() == 0) {
                            list.resize(vtxCnt);
                        }

                        int lUVIndex = leUV->GetIndexArray().GetAt(UVIndex);

                        // 取得したインデックスから UV を取得する
                        FbxVector2 lVec2 = leUV->GetDirectArray().GetAt(lUVIndex);

                        list[v].uv[0] = lVec2.mData[0];
                        list[v].uv[1] = lVec2.mData[1];
                    }

                    UVIndex++;
                }
            }
        }
    }
}