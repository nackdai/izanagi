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
    IZ_UINT ret = m_vertices.size();
    return ret;
}

const VertexData& FbxDataManager::GetVertex(IZ_UINT idx) const
{
    return m_vertices[idx];
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
    for (IZ_UINT i = 0; i < m_vertices.size(); i++)
    {
        const auto& vtx = m_vertices[i];

        if (vtx.fbxMesh == mesh && vtx.idxInMesh == vtxIdxInMesh)
        {
            return i;
        }
    }

    IZ_ASSERT(IZ_FALSE);
    return 0;
}

// �m�[�h���W�߂�.
void FbxDataManager::GatherNodes(FbxNode* node)
{
    // NOTE
    // RootNode�͏��O����.
    if (node != m_scene->GetRootNode()) {
        m_nodes.push_back(node);
    }

    for (IZ_UINT i = 0; i < node->GetChildCount(); i++) {
        FbxNode* child = node->GetChild(i);
        GatherNodes(child);
    }
}

// ���b�V�����W�߂�
void FbxDataManager::GatherMeshes()
{
    // �V�[���Ɋ܂܂�郁�b�V���̉��
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
        // ���b�V���Ɋ܂܂��X�L�j���O���.
        int skinCount = mesh->GetDeformerCount(FbxDeformer::EDeformerType::eSkin);

        for (int n = 0; n < skinCount; n++) {
            // �X�L�j���O�����擾.
            FbxDeformer* deformer = mesh->GetDeformer(n, FbxDeformer::EDeformerType::eSkin);

            FbxSkin* skin = (FbxSkin*)deformer;

            // �X�L�j���O�ɉe����^����{�[���̐�.
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
    // �V�[���Ɋ܂܂�郁�b�V���̉��
    auto meshCount = m_scene->GetMemberCount<FbxMesh>();

    for (int i = 0; i < meshCount; ++i)
    {
        FbxMesh* fbxMesh = m_scene->GetMember<FbxMesh>(i);

        if (fbxMesh->GetElementMaterial() == NULL) {
            continue;
        }

        // ���b�V���Ɋ܂܂��|���S���i�O�p�`�j�̐�.
        const int polygonCount = fbxMesh->GetPolygonCount();

        auto& materialIndices = fbxMesh->GetElementMaterial()->GetIndexArray();

        if (materialIndices.GetCount() == polygonCount)
        {
            // ���b�V���Ɋ܂܂��|���S���i�O�p�`�j���Ƃɂǂ̃}�e���A���ɏ������Ă���̂��𒲂ׂ�.
            for (int i = 0; i < polygonCount; i++)
            {
                // ���b�V���Ɋ܂܂��|���S���i�O�p�`�j���������Ă���}�e���A���ւ̃C���f�b�N�X.
                const int materialIdx = materialIndices.GetAt(i);

                // �}�e���A���{�̂��擾.
                auto material = m_scene->GetMaterial(materialIdx);

                // �o�^�ς݃��b�V����T��.
                auto it = std::find(m_meshes.begin(), m_meshes.end(), MeshSubset(fbxMesh, material));

                // ���o�^������.
                if (it == m_meshes.end())
                {
                    m_meshes.push_back(MeshSubset(fbxMesh, material));
                }

                m_indices.push_back(IndexData(fbxMesh->GetPolygonVertex(i, 0), fbxMesh, material));
                m_indices.push_back(IndexData(fbxMesh->GetPolygonVertex(i, 1), fbxMesh, material));
                m_indices.push_back(IndexData(fbxMesh->GetPolygonVertex(i, 2), fbxMesh, material));
            }
        }
        else {
            IZ_ASSERT(IZ_FALSE);
        }
    }
}

void FbxDataManager::GatherVertices()
{
    std::map<FbxMesh*, std::vector<PosData>> posList;
    GatherPos(posList);

    std::map<FbxMesh*, std::vector<UVData>> uvList;
    GatherUV(uvList);

    IZ_ASSERT(posList.size() == uvList.size());

    std::vector<IndexData> indices;

    for (IZ_UINT i = 0; i < m_indices.size(); i++)
    {
        auto index = m_indices[i];

        auto itMesh = std::find(m_meshes.begin(), m_meshes.end(), MeshSubset(index.fbxMesh, index.mtrl));
        IZ_ASSERT(itMesh != m_meshes.end());

        MeshSubset& mesh = *itMesh;

        auto& pos = posList[mesh.fbxMesh];
        auto& uv = uvList[mesh.fbxMesh];

        const auto& posData = pos[i];
        const auto& uvData = uv[i];

        IZ_ASSERT(posData.idxInMesh == index.idxInMesh);
        IZ_ASSERT(uvData.idxInMesh == index.idxInMesh);

        IZ_ASSERT(posData.fbxMesh == index.fbxMesh);
        IZ_ASSERT(uvData.fbxMesh == index.fbxMesh);

        IZ_ASSERT(posData.mtrl == uvData.mtrl);

        VertexData vtx;
        vtx.idxInMesh = index.idxInMesh;
        vtx.pos = posData.pos;
        vtx.uv = uvData.uv;
        vtx.fbxMesh = index.fbxMesh;
        vtx.mtrl = posData.mtrl;

        auto it = std::find(m_vertices.begin(), m_vertices.end(), vtx);
        if (it == m_vertices.end())
        {
            IndexData newIdx(
                m_vertices.size(),
                mesh.fbxMesh,
                mesh.mtrl);

            indices.push_back(newIdx);
            m_vertices.push_back(vtx);

            mesh.vtxNum++;
        }
        else
        {
            IndexData newIdx(
                std::distance(m_vertices.begin(), it),
                mesh.fbxMesh,
                mesh.mtrl);

            indices.push_back(newIdx);
        }
    }

    for (IZ_UINT i = 0; i < indices.size(); i += 3)
    {
        auto index = indices[i];

        IZ_ASSERT(indices[i + 0].fbxMesh == indices[i + 1].fbxMesh);
        IZ_ASSERT(indices[i + 0].fbxMesh == indices[i + 2].fbxMesh);

        IZ_ASSERT(indices[i + 0].mtrl == indices[i + 1].mtrl);
        IZ_ASSERT(indices[i + 0].mtrl == indices[i + 2].mtrl);

        auto itMesh = std::find(m_meshes.begin(), m_meshes.end(), MeshSubset(index.fbxMesh, index.mtrl));
        IZ_ASSERT(itMesh != m_meshes.end());

        MeshSubset& mesh = *itMesh;

        Face face;
        face.vtx[0] = indices[i + 0].idxInMesh;
        face.vtx[1] = indices[i + 1].idxInMesh;
        face.vtx[2] = indices[i + 2].idxInMesh;

        mesh.faces.push_back(face);
    }

    // ��������Ȃ�
    m_indices.clear();
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

        auto& materialIndices = fbxMesh->GetElementMaterial()->GetIndexArray();

        for (IZ_UINT p = 0; p < polygonCnt; p++)
        {
            for (IZ_UINT i = 0; i < 3; i++)
            {
                // ���b�V���Ɋ܂܂��|���S���i�O�p�`�j���������Ă���}�e���A���ւ̃C���f�b�N�X.
                const int materialIdx = materialIndices.GetAt(i);

                // �}�e���A���{�̂��擾.
                auto material = m_scene->GetMaterial(materialIdx);

                IZ_UINT idx = fbxMesh->GetPolygonVertex(p, i);

                auto position = fbxMesh->GetControlPointAt(idx);

                PosData pos;
                {
                    pos.idxInMesh = idx;
                    pos.pos = position;

                    pos.fbxMesh = fbxMesh;
                    pos.mtrl = material;
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

        auto& materialIndices = fbxMesh->GetElementMaterial()->GetIndexArray();

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
                // ���b�V���Ɋ܂܂��|���S���i�O�p�`�j���������Ă���}�e���A���ւ̃C���f�b�N�X.
                const int materialIdx = materialIndices.GetAt(n);

                // �}�e���A���{�̂��擾.
                auto material = m_scene->GetMaterial(materialIdx);

                int lUVIndex = layerUV->GetIndexArray().GetAt(UVIndex);

                // �擾�����C���f�b�N�X���� UV ���擾����
                FbxVector2 lVec2 = layerUV->GetDirectArray().GetAt(lUVIndex);

                IZ_UINT idxInMesh = fbxMesh->GetPolygonVertex(p, n);

                UVData uv;
                {
                    uv.idxInMesh = idxInMesh;
                    uv.uv = lVec2;

                    uv.fbxMesh = fbxMesh;
                    uv.mtrl = material;
                }

                list.push_back(uv);

                UVIndex++;
            }
        }
    }
}
