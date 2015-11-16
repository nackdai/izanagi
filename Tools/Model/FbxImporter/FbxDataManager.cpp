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
                GatherFaces();

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


// ノードを集める.
void FbxDataManager::GatherNodes(FbxNode* node)
{
    // NOTE
    // RootNodeは除外する.
    if (node != m_scene->GetRootNode()) {
        m_nodes.push_back(node);
    }

    // Gather materials.
    const int mtrlCount = node->GetMaterialCount();
    for (int i = 0; i < mtrlCount; i++)
    {
        auto* material = node->GetMaterial(i);
        if (material) {
            m_mtrls.insert(std::make_pair(material->GetName(), material));
        }
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
    m_meshes.resize(meshCount);

    for (int i = 0; i < meshCount; ++i)
    {
        FbxMesh* mesh = m_scene->GetMember<FbxMesh>(i);
        m_meshes[i].mesh = mesh;
    }
}

void FbxDataManager::GatherFaces()
{
    IZ_UINT offset = 0;

    for (int i = 0; i < m_meshes.size(); i++)
    {
        FbxMesh* mesh = m_meshes[i].mesh;

        if (mesh->GetElementMaterial() == NULL) {
            continue;
        }

        auto& subsets = m_meshes[i].subsets;

        // メッシュに含まれるポリゴン（三角形）の数.
        const int polygonCount = mesh->GetPolygonCount();

        auto& materialIndices = mesh->GetElementMaterial()->GetIndexArray();

        if (materialIndices.GetCount() == polygonCount)
        {
            // メッシュに含まれるポリゴン（三角形）ごとにどのマテリアルに所属しているのかを調べる.
            for (int i = 0; i < polygonCount; i++)
            {
                // メッシュに含まれるポリゴン（三角形）が所属しているマテリアルへのインデックス.
                const int materialIdx = materialIndices.GetAt(i);

                // マテリアル本体を取得.
                auto material = m_scene->GetMaterial(materialIdx);
                std::string name(material->GetName());

                if (subsets.find(name) != subsets.end()) {
                    subsets.insert(std::make_pair(name, std::vector<Face>()));
                }

                Face face;

                // 三角形なので３点.
                face.idx[0] = mesh->GetPolygonVertex(i, 0) + offset;
                face.idx[1] = mesh->GetPolygonVertex(i, 1) + offset;
                face.idx[2] = mesh->GetPolygonVertex(i, 2) + offset;

                subsets[name].push_back(face);
            }
        }
        else {
            IZ_ASSERT(IZ_FALSE);
        }

        offset = mesh->GetControlPointsCount();
    }
}