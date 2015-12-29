#include "FbxDataManager.h"

IZ_BOOL FbxDataManager::IsValid() const
{
    return (m_manager != nullptr && m_scene != nullptr);
}

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
                
                return IZ_TRUE;
            }
        }
    }

    IZ_ASSERT(IZ_FALSE);
    return IZ_FALSE;
}

IZ_BOOL FbxDataManager::OpenForAnm(const char* path)
{
    m_manager = FbxManager::Create();

    FbxIOSettings* ios = FbxIOSettings::Create(m_manager, IOSROOT);
    m_manager->SetIOSettings(ios);

    m_scene = FbxScene::Create(m_manager, "animationScene");

    FbxImporter* importer = FbxImporter::Create(m_manager, "");

    int fileFormat = -1;

    if (m_manager->GetIOPluginRegistry()->DetectReaderFileFormat(path, fileFormat)) {
        if (importer->Initialize(path, fileFormat)) {
            if (importer->Import(m_scene)) {
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

                LoadAnimation(importer);

                importer->Destroy();

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

void FbxDataManager::LoadMesh()
{
    if (IsValid() && m_vertices.size() == 0) {
        GatherFaces();

        GatherVertices();
    }
}

void FbxDataManager::LoadMaterial()
{
    if (IsValid() && m_materials.size() == 0) {
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

            auto mtrlCnt = materialIndices.GetCount();

            if (mtrlCnt == polygonCount)
            {
                for (int i = 0; i < polygonCount; i++)
                {
                    // メッシュに含まれるポリゴン（三角形）が所属しているマテリアルへのインデックス.
                    const int materialIdx = materialIndices.GetAt(i);

                    // マテリアル本体を取得.
                    auto material = m_scene->GetMaterial(materialIdx);

                    auto itMtrl = std::find(m_materials.begin(), m_materials.end(), material);
                    if (itMtrl == m_materials.end())
                    {
                        m_materials.push_back(material);
                    }
                }
            }
            else {
                IZ_ASSERT(mtrlCnt == 1);

                auto material = fbxMesh->GetNode()->GetMaterial(0);

                auto itMtrl = std::find(m_materials.begin(), m_materials.end(), material);
                if (itMtrl == m_materials.end())
                {
                    m_materials.push_back(material);
                }
            }
        }
    }
}

void FbxDataManager::LoadAnimation(FbxImporter* importer)
{
    // 含まれるアニメーション数.
    // １つしか許さない.
    auto animStackCount = importer->GetAnimStackCount();
    IZ_ASSERT(animStackCount == 1);

    auto takeInfo = importer->GetTakeInfo(0);

    auto importOffset = takeInfo->mImportOffset;
    auto startTime = takeInfo->mLocalTimeSpan.GetStart();
    auto stopTime = takeInfo->mLocalTimeSpan.GetStop();

    // TODO
    auto oneFrame = fbxsdk::FbxTime::GetOneFrameValue(fbxsdk::FbxTime::eFrames60);

    // フレーム数計算.
    m_AnmStartFrame = (importOffset.Get() + startTime.Get()) / oneFrame;
    m_AnmStopFrame = (importOffset.Get() + stopTime.Get()) / oneFrame;
    IZ_ASSERT(m_AnmStartFrame < m_AnmStopFrame);

    // ノードを集める.
    GatherNodes(nullptr);
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

void FbxDataManager::GetSkinData(
    IZ_UINT idx,
    std::vector<IZ_FLOAT>& weight,
    std::vector<IZ_UINT>& joint) const
{
    const auto& vtx = m_vertices[idx];

    std::copy(vtx.weight.begin(), vtx.weight.end(), std::back_inserter(weight));
    std::copy(vtx.joint.begin(), vtx.joint.end(), std::back_inserter(joint));
}

IZ_UINT FbxDataManager::GetMaterialNum() const
{
    return m_materials.size();
}

FbxSurfaceMaterial* FbxDataManager::GetMaterial(IZ_UINT idx)
{
    IZ_ASSERT(idx < m_materials.size());
    return m_materials[idx];
}

// ノードを集める.
void FbxDataManager::GatherNodes(FbxNode* node)
{
#if 0
    // NOTE
    // RootNodeは除外する.
    if (node != m_scene->GetRootNode()) {
        m_nodes.push_back(node);
    }

    for (IZ_UINT i = 0; i < node->GetChildCount(); i++) {
        FbxNode* child = node->GetChild(i);
        GatherNodes(child);
    }
#else
    // ノードを集める.
    auto nodeCount = m_scene->GetNodeCount();
    for (IZ_UINT i = 0; i < nodeCount; ++i)
    {
        auto fbxNode = m_scene->GetNode(i);
        m_nodes.push_back(fbxNode);
    }
#endif
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

fbxsdk::FbxSurfaceMaterial* FbxDataManager::GetMaterial(FbxMesh* fbxMesh, IZ_UINT index)
{
    fbxsdk::FbxSurfaceMaterial* material = nullptr;

    // メッシュに含まれるポリゴン（三角形）の数.
    const int polygonCount = fbxMesh->GetPolygonCount();

    auto& materialIndices = fbxMesh->GetElementMaterial()->GetIndexArray();

    const auto cntMtrl = materialIndices.GetCount();

    // メッシュに含まれるポリゴン（三角形）が所属しているマテリアルへのインデックス.
    if (cntMtrl == polygonCount) {
        auto materialIdx = materialIndices.GetAt(index);

        // マテリアル本体を取得.
        material = m_scene->GetMaterial(materialIdx);
    }
    else {
        IZ_ASSERT(cntMtrl == 1);

        material = fbxMesh->GetNode()->GetMaterial(0);
    }

    return material;
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

        m_indices.insert(std::make_pair(fbxMesh, std::vector<IndexData>()));
        auto& indices = m_indices[fbxMesh];

        // メッシュに含まれるポリゴン（三角形）の数.
        const int polygonCount = fbxMesh->GetPolygonCount();

        auto& materialIndices = fbxMesh->GetElementMaterial()->GetIndexArray();

        const auto cntMtrl = materialIndices.GetCount();

        // メッシュに含まれるポリゴン（三角形）ごとにどのマテリアルに所属しているのかを調べる.
        for (int i = 0; i < polygonCount; i++)
        {
#if 0
            int materialIdx = -1;

            // メッシュに含まれるポリゴン（三角形）が所属しているマテリアルへのインデックス.
            if (cntMtrl == polygonCount) {
                materialIdx = materialIndices.GetAt(i);
            }
            else {
                IZ_ASSERT(cntMtrl == 1);
                materialIdx = materialIndices.GetAt(0);
            }

            // マテリアル本体を取得.
            auto material = m_scene->GetMaterial(materialIdx);
#else
            auto material = GetMaterial(fbxMesh, i);
#endif

            auto itMtrl = std::find(m_materials.begin(), m_materials.end(), material);
            if (itMtrl == m_materials.end())
            {
                m_materials.push_back(material);
            }

            // 登録済みメッシュを探す.
            auto it = std::find(m_meshes.begin(), m_meshes.end(), MeshSubset(fbxMesh, material));

            // 未登録だった.
            if (it == m_meshes.end())
            {
                m_meshes.push_back(MeshSubset(fbxMesh, material));
            }

            indices.push_back(IndexData(fbxMesh->GetPolygonVertex(i, 0), i, fbxMesh, material));
            indices.push_back(IndexData(fbxMesh->GetPolygonVertex(i, 1), i, fbxMesh, material));
            indices.push_back(IndexData(fbxMesh->GetPolygonVertex(i, 2), i, fbxMesh, material));
        }
    }
}

void FbxDataManager::GatherVertices()
{
    // 位置.
    std::map<FbxMesh*, std::vector<PosData>> posList;
    GatherPos(posList);

    // UV.
    std::map<FbxMesh*, std::vector<UVData>> uvList;
    GatherUV(uvList);

    // 法線.
    std::map<FbxMesh*, std::vector<NormalData>> nmlList;
    GatherNormal(nmlList);

    // 頂点カラー.
    std::map<FbxMesh*, std::vector<ColorData>> clrList;
    GatherColor(clrList);

    // スキン.
    std::vector<SkinData> skinList;
    GatherSkin(skinList);

    IZ_ASSERT(posList.size() == uvList.size());
    IZ_ASSERT(uvList.size() == nmlList.size());

    std::vector<IndexData> indices;

    // 頂点データを統合して整理する.

    for (auto& m : m_meshes)
    {
        auto& idxList = m_indices[m.fbxMesh];

        for (IZ_UINT i = 0; i < idxList.size(); i++)
        {
            auto index = idxList[i];

            auto itMesh = std::find(m_meshes.begin(), m_meshes.end(), MeshSubset(index.fbxMesh, index.mtrl));
            IZ_ASSERT(itMesh != m_meshes.end());

            MeshSubset& mesh = *itMesh;
            IZ_ASSERT(mesh.fbxMesh == m.fbxMesh);

            auto& pos = posList[mesh.fbxMesh];
            auto& uv = uvList[mesh.fbxMesh];
            auto& nml = nmlList[mesh.fbxMesh];

            const auto& posData = pos[i];
            const auto& uvData = uv[i];
            const auto& nmlData = nml[i];

            IZ_ASSERT(posData.idxInMesh == index.idxInMesh);
            IZ_ASSERT(uvData.idxInMesh == index.idxInMesh);
            IZ_ASSERT(nmlData.idxInMesh == index.idxInMesh);

            IZ_ASSERT(posData.fbxMesh == index.fbxMesh);
            IZ_ASSERT(uvData.fbxMesh == index.fbxMesh);
            IZ_ASSERT(nmlData.fbxMesh == index.fbxMesh);

            IZ_ASSERT(posData.mtrl == uvData.mtrl);
            IZ_ASSERT(uvData.mtrl == nmlData.mtrl);

            VertexData vtx;
            vtx.idxInMesh = index.idxInMesh;
            vtx.pos = posData.pos;
            vtx.uv = uvData.uv;
            vtx.nml = nmlData.nml;
            vtx.fbxMesh = index.fbxMesh;
            vtx.mtrl = posData.mtrl;

            if (clrList.size() > 0) {
                auto& clr = clrList[mesh.fbxMesh];
                const auto& clrData = clr[i];

                IZ_ASSERT(clrData.idxInMesh == index.idxInMesh);
                IZ_ASSERT(clrData.fbxMesh == index.fbxMesh);
                IZ_ASSERT(clrData.mtrl == posData.mtrl);

                vtx.clr = clrData.clr;
            }
            else {
                vtx.clr.Set(1.0, 1.0, 1.0);
            }

            // スキン.
            auto itSkin = std::find(skinList.begin(), skinList.end(), SkinData(index.idxInMesh, mesh.fbxMesh));
            if (itSkin != skinList.end())
            {
                auto& skin = *itSkin;
                std::copy(skin.weight.begin(), skin.weight.end(), std::back_inserter(vtx.weight));
                std::copy(skin.joint.begin(), skin.joint.end(), std::back_inserter(vtx.joint));
            }
            else {
                // TODO
                vtx.joint.push_back(0);
                vtx.weight.push_back(1.0f);
            }

            // 同じデータの頂点の有無を確認.
            auto it = std::find(m_vertices.begin(), m_vertices.end(), vtx);

            if (it == m_vertices.end())
            {
                // 未登録.

                IndexData newIdx(
                    m_vertices.size(),
                    0,  // もう使わない.
                    mesh.fbxMesh,
                    mesh.mtrl);

                indices.push_back(newIdx);
                m_vertices.push_back(vtx);

                mesh.vtxNum++;
            }
            else
            {
                // すでにあったので、どの頂点インデックスか取得.
                IndexData newIdx(
                    std::distance(m_vertices.begin(), it),
                    0,  // もう使わない.
                    mesh.fbxMesh,
                    mesh.mtrl);

                indices.push_back(newIdx);
            }
        }
    }

    // 新しく作られたインデックスリストから面リストを生成.
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

    // もういらない
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

        for (IZ_UINT p = 0; p < polygonCnt; p++)
        {
            for (IZ_UINT i = 0; i < 3; i++)
            {
#if 0
                int materialIdx = -1;

                // メッシュに含まれるポリゴン（三角形）が所属しているマテリアルへのインデックス.
                if (cntMtrl == polygonCnt) {
                    materialIdx = materialIndices.GetAt(p);
                }
                else {
                    IZ_ASSERT(cntMtrl == 1);
                    materialIdx = materialIndices.GetAt(0);
                }

                // マテリアル本体を取得.
                auto material = m_scene->GetMaterial(materialIdx);
#else
                auto material = GetMaterial(fbxMesh, p);
#endif

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

        auto elemUV = fbxMesh->GetElementUV();
        auto mappingMode = elemUV->GetMappingMode();
        auto referenceMode = elemUV->GetReferenceMode();

        IZ_ASSERT(mappingMode == FbxGeometryElement::eByPolygonVertex);
        IZ_ASSERT(referenceMode == FbxGeometryElement::eIndexToDirect);

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
#if 0
                int materialIdx = -1;

                // メッシュに含まれるポリゴン（三角形）が所属しているマテリアルへのインデックス.
                if (cntMtrl == polygonCnt) {
                    materialIdx = materialIndices.GetAt(p);
                }
                else {
                    IZ_ASSERT(cntMtrl == 1);
                    materialIdx = materialIndices.GetAt(0);
                }

                // マテリアル本体を取得.
                auto material = m_scene->GetMaterial(materialIdx);
#else
                auto material = GetMaterial(fbxMesh, p);
#endif

                int lUVIndex = layerUV->GetIndexArray().GetAt(UVIndex);

                // 取得したインデックスから UV を取得する
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

void FbxDataManager::GatherNormal(std::map<FbxMesh*, std::vector<NormalData>>& nmlList)
{
    for (IZ_UINT m = 0; m < m_fbxMeshes.size(); m++)
    {
        FbxMesh* fbxMesh = m_fbxMeshes[m];

        auto elemNml = fbxMesh->GetElementNormal();
        auto mappingMode = elemNml->GetMappingMode();
        auto referenceMode = elemNml->GetReferenceMode();

        nmlList.insert(std::make_pair(fbxMesh, std::vector<NormalData>()));
        auto it = nmlList.find(fbxMesh);

        auto& list = it->second;

        IZ_UINT polygonCnt = fbxMesh->GetPolygonCount();
        IZ_UINT vtxNum = fbxMesh->GetControlPointsCount();

        FbxLayerElementNormal* layerNml = fbxMesh->GetLayer(0)->GetNormals();
        IZ_UINT idxNml = 0;

        if (mappingMode == FbxGeometryElement::eByPolygonVertex)
        {
            for (IZ_UINT p = 0; p < polygonCnt; p++)
            {
                for (IZ_UINT n = 0; n < 3; n++)
                {
#if 0
                    int materialIdx = -1;

                    // メッシュに含まれるポリゴン（三角形）が所属しているマテリアルへのインデックス.
                    if (cntMtrl == polygonCnt) {
                        materialIdx = materialIndices.GetAt(p);
                    }
                    else {
                        IZ_ASSERT(cntMtrl == 1);
                        materialIdx = materialIndices.GetAt(0);
                    }

                    // マテリアル本体を取得.
                    auto material = m_scene->GetMaterial(materialIdx);
#else
                    auto material = GetMaterial(fbxMesh, p);
#endif

                    int lNmlIndex = (referenceMode == FbxGeometryElement::eIndexToDirect
                        ? layerNml->GetIndexArray().GetAt(idxNml)
                        : idxNml);

                    // 取得したインデックスから法線を取得する
                    FbxVector4 lVec4 = layerNml->GetDirectArray().GetAt(lNmlIndex);

                    IZ_UINT idxInMesh = fbxMesh->GetPolygonVertex(p, n);

                    NormalData nml;
                    {
                        nml.idxInMesh = idxInMesh;
                        nml.nml = lVec4;

                        nml.fbxMesh = fbxMesh;
                        nml.mtrl = material;
                    }

                    list.push_back(nml);

                    idxNml++;
                }
            }
        }
        else {
            auto& indices = m_indices[fbxMesh];

            for (const auto& idx : indices)
            {
                auto material = GetMaterial(fbxMesh, idx.polygonIdxInMesh);

                int lNmlIndex = (referenceMode == FbxGeometryElement::eIndexToDirect
                    ? layerNml->GetIndexArray().GetAt(idx.idxInMesh)
                    : idx.idxInMesh);

                // 取得したインデックスから法線を取得する
                FbxVector4 lVec4 = layerNml->GetDirectArray().GetAt(lNmlIndex);

                NormalData nml;
                {
                    nml.idxInMesh = idx.idxInMesh;
                    nml.nml = lVec4;

                    nml.fbxMesh = fbxMesh;
                    nml.mtrl = material;
                }

                list.push_back(nml);
            }
        }
    }
}

void FbxDataManager::GatherColor(std::map<FbxMesh*, std::vector<ColorData>>& clrList)
{
    for (IZ_UINT m = 0; m < m_fbxMeshes.size(); m++)
    {
        FbxMesh* fbxMesh = m_fbxMeshes[m];

        if (fbxMesh->GetElementVertexColorCount() == 0) {
            continue;
        }

        auto elemClr = fbxMesh->GetElementVertexColor();
        auto mappingMode = elemClr->GetMappingMode();
        auto referenceMode = elemClr->GetReferenceMode();

        IZ_ASSERT(mappingMode == FbxGeometryElement::eByPolygonVertex);
        IZ_ASSERT(referenceMode == FbxGeometryElement::eIndexToDirect);

        auto& materialIndices = fbxMesh->GetElementMaterial()->GetIndexArray();

        const auto cntMtrl = materialIndices.GetCount();

        clrList.insert(std::make_pair(fbxMesh, std::vector<ColorData>()));
        auto it = clrList.find(fbxMesh);

        auto& list = it->second;

        IZ_UINT polygonCnt = fbxMesh->GetPolygonCount();
        IZ_UINT vtxNum = fbxMesh->GetControlPointsCount();

        FbxLayerElementVertexColor* layerClr = fbxMesh->GetLayer(0)->GetVertexColors();
        IZ_UINT idxClr = 0;

        for (IZ_UINT p = 0; p < polygonCnt; p++)
        {
            for (IZ_UINT n = 0; n < 3; n++)
            {
                int materialIdx = -1;

                // メッシュに含まれるポリゴン（三角形）が所属しているマテリアルへのインデックス.
                if (cntMtrl == polygonCnt) {
                    materialIdx = materialIndices.GetAt(p);
                }
                else {
                    IZ_ASSERT(cntMtrl == 1);
                    materialIdx = materialIndices.GetAt(0);
                }

                // マテリアル本体を取得.
                auto material = m_scene->GetMaterial(materialIdx);

                int lNmlIndex = layerClr->GetIndexArray().GetAt(idxClr);

                // 取得したインデックスから法線を取得する
                FbxColor color = layerClr->GetDirectArray().GetAt(lNmlIndex);

                IZ_UINT idxInMesh = fbxMesh->GetPolygonVertex(p, n);

                ColorData clr;
                {
                    clr.idxInMesh = idxInMesh;
                    clr.clr = color;

                    clr.fbxMesh = fbxMesh;
                    clr.mtrl = material;
                }

                list.push_back(clr);

                idxClr++;
            }
        }
    }
}

void FbxDataManager::GatherSkin(std::vector<SkinData>& skinList)
{
    for (IZ_UINT m = 0; m < m_fbxMeshes.size(); m++)
    {
        FbxMesh* fbxMesh = m_fbxMeshes[m];

        // メッシュに含まれるスキニング情報数.
        int skinCount = fbxMesh->GetDeformerCount(FbxDeformer::EDeformerType::eSkin);

        for (int n = 0; n < skinCount; n++) {
            // スキニング情報を取得.
            FbxDeformer* deformer = fbxMesh->GetDeformer(n, FbxDeformer::EDeformerType::eSkin);

            FbxSkin* skin = (FbxSkin*)deformer;

            // スキニングに影響を与えるボーンの数.
            int boneCount = skin->GetClusterCount();

            for (int b = 0; b < boneCount; b++) {
                FbxCluster* cluster = skin->GetCluster(b);

                // ボーンが影響を与える頂点数.
                int influencedVtxNum = cluster->GetControlPointIndicesCount();

                // ボーンと関連づいているノード.
                FbxNode* targetNode = cluster->GetLink();

                // ノードのインデックス.
                IZ_INT nodeIdx = GetNodeIndex(targetNode);
                IZ_ASSERT(nodeIdx >= 0);

                for (int v = 0; v < influencedVtxNum; v++) {
                    int vtxIdxInMesh = cluster->GetControlPointIndices()[v];
                    float weight = (float)cluster->GetControlPointWeights()[v];

                    auto it = std::find(skinList.begin(), skinList.end(), SkinData(vtxIdxInMesh, fbxMesh));

                    if (it == skinList.end()) {
                        SkinData skin(vtxIdxInMesh, fbxMesh);
                        skin.weight.push_back(weight);
                        skin.joint.push_back(nodeIdx);

                        skinList.push_back(skin);
                    }
                    else {
                        SkinData& skin = *it;

                        skin.weight.push_back(weight);
                        skin.joint.push_back(nodeIdx);
                    }
                }
            }
        }
    }
}