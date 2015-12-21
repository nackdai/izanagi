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

MeshSubset& FbxDataManager::GetMesh(IZ_UINT idx)
{
    IZ_ASSERT(idx < m_meshes.size());
    return m_meshes[idx];
}


const MeshSubset& FbxDataManager::GetMesh(IZ_UINT idx) const
{
    IZ_ASSERT(idx < m_meshes.size());
    return m_meshes[idx];
}

IZ_UINT FbxDataManager::GetVtxNum() const
{
    IZ_UINT ret = 0;

    for each (auto& mesh in m_meshes)
    {
        ret += mesh.vertices.size();
    }

    return ret;
}

const VertexData& FbxDataManager::GetVertex(IZ_UINT idx) const
{
#if 0
    IZ_UINT pos = 0;

    for each (auto& mesh in m_meshes)
    {
        if (pos <= idx && idx < pos + mesh.vertices.size())
        {
            IZ_UINT i = idx - pos;
            return mesh.vertices[i];
        }
    }

    IZ_ASSERT(IZ_FALSE);
    return VertexData();
#else
    return m_meshes[0].vertices[idx];
#endif
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
    auto& meshSubset = m_meshes[0];

    for (IZ_UINT i = 0; i < meshSubset.vertices.size(); i++)
    {
        if (meshSubset.vertices[i].idxInMesh == vtxIdxInMesh)
        {
            return i;
        }
    }

    IZ_ASSERT(IZ_FALSE);
    return 0;
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

#if 0
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
                MeshSubset* mesh = nullptr;
                {
                    auto it = std::find(m_meshes.begin(), m_meshes.end(), MeshSubset(fbxMesh, material));

                    // 未登録だった.
                    if (it == m_meshes.end())
                    {
                        m_meshes.push_back(MeshSubset(fbxMesh, material));
                        mesh = &(m_meshes.back());
                    }
                    else
                    {
                        mesh = &(*it);
                    }
                }

                // 三角形なので３点.
                mesh->indices.push_back(fbxMesh->GetPolygonVertex(i, 0));
                mesh->indices.push_back(fbxMesh->GetPolygonVertex(i, 1));
                mesh->indices.push_back(fbxMesh->GetPolygonVertex(i, 2));
            }
        }
        else {
            IZ_ASSERT(IZ_FALSE);
        }
#else
        if (m_meshes.size() == 0)
        {
            m_meshes.push_back(MeshSubset(fbxMesh, nullptr));
        }

        MeshSubset& mesh = m_meshes[0];

        for (int i = 0; i < polygonCount; i++)
        {
            mesh.indices.push_back(fbxMesh->GetPolygonVertex(i, 0));
            mesh.indices.push_back(fbxMesh->GetPolygonVertex(i, 1));
            mesh.indices.push_back(fbxMesh->GetPolygonVertex(i, 2));
        }
#endif
    }
}

void FbxDataManager::GatherVertices()
{
    std::map<FbxMesh*, std::vector<PosData>> posList;
    GatherPos(posList);

    std::map<FbxMesh*, std::vector<UVData>> uvList;
    GatherUV(uvList);

    IZ_ASSERT(posList.size() == uvList.size());

    MeshSubset& mesh = m_meshes[0];
    IZ_ASSERT(posList[mesh.fbxMesh].size() == uvList[mesh.fbxMesh].size());
    IZ_ASSERT(mesh.indices.size() == uvList[mesh.fbxMesh].size());

    std::vector<IZ_UINT> indices;

    for (IZ_UINT i = 0; i < mesh.indices.size(); i++)
    {
        auto& pos = posList[mesh.fbxMesh];
        auto& uv = uvList[mesh.fbxMesh];

        auto index = mesh.indices[i];
        const auto& posData = pos[i];
        const auto& uvData = uv[i];

        IZ_ASSERT(posData.idxInMesh == index);
        IZ_ASSERT(uvData.idxInMesh == index);

        VertexData vtx;
        vtx.idxInMesh = index;
        vtx.pos = posData.pos;
        vtx.uv = uvData.uv;

        auto it = std::find(mesh.vertices.begin(), mesh.vertices.end(), vtx);
        if (it == mesh.vertices.end())
        {
            indices.push_back(mesh.vertices.size());
            mesh.vertices.push_back(vtx);
        }
        else
        {
            auto idx = std::distance(mesh.vertices.begin(), it);
            indices.push_back(idx);
        }
    }

    for (IZ_UINT i = 0; i < indices.size(); i += 3)
    {
        Face face;
        face.vtx[0] = indices[i + 0];
        face.vtx[1] = indices[i + 1];
        face.vtx[2] = indices[i + 2];

        mesh.faces.push_back(face);
    }
}

void FbxDataManager::GatherPos(std::map<FbxMesh*, std::vector<PosData>>& posList)
{
    for (IZ_UINT m = 0; m < m_fbxMeshes.size(); m++)
    {
        FbxMesh* fbxMesh = m_fbxMeshes[m];

        posList.insert(std::make_pair(fbxMesh, std::vector<PosData>()));
        auto it = posList.find(fbxMesh);

        auto& list = it->second;
        
        auto polygonCnt = fbxMesh->GetPolygonCount();

        for (IZ_UINT p = 0; p < polygonCnt; p++)
        {
            for (IZ_UINT i = 0; i < 3; i++)
            {
                IZ_UINT idx = fbxMesh->GetPolygonVertex(p, i);

                auto position = fbxMesh->GetControlPointAt(idx);

                PosData pos;
                {
                    pos.idxInMesh = idx;
                    pos.pos = position;
                }

                list.push_back(pos);
            }
        }
    }
}

void FbxDataManager::GatherUV(std::map<FbxMesh*, std::vector<UVData>>& uvList)
{
    for (IZ_UINT m = 0; m < m_fbxMeshes.size(); m++)
    {
        FbxMesh* fbxMesh = m_fbxMeshes[m];

        uvList.insert(std::make_pair(fbxMesh, std::vector<UVData>()));
        auto it = uvList.find(fbxMesh);

        auto& list = it->second;

        IZ_UINT polygonCnt = fbxMesh->GetPolygonCount();
        IZ_UINT vtxNum = fbxMesh->GetControlPointsCount();

        FbxLayerElementUV* layerUV = fbxMesh->GetLayer(0)->GetUVs();
        IZ_UINT UVIndex = 0;

        for (IZ_UINT p = 0; p < polygonCnt; p++)
        {
            for (IZ_UINT n = 0; n < 3; n++)
            {
                int lUVIndex = layerUV->GetIndexArray().GetAt(UVIndex);

                // 取得したインデックスから UV を取得する
                FbxVector2 lVec2 = layerUV->GetDirectArray().GetAt(lUVIndex);

                IZ_UINT idxInMesh = fbxMesh->GetPolygonVertex(p, n);

                UVData uv;
                {
                    uv.idxInMesh = idxInMesh;
                    uv.uv = lVec2;
                }

                list.push_back(uv);

                UVIndex++;
            }
        }
    }
}
