#include "fbxsdk.h"
#include "FbxImporter.h"

#include <map>

class CFbxImporter::FbxSdk {
public:
    FbxSdk(){}
    ~FbxSdk(){}

    void getNodes(FbxNode* node);

    void getMeshes();

    IZ_INT getNodeIndex(FbxNode* node);

public:
    FbxManager* manager{ nullptr };
    FbxScene* scene{ nullptr };

    std::vector<FbxNode*> nodes;
    std::map<std::string, FbxSurfaceMaterial*> mtrls;

    struct MeshSubset {
        IZ_UINT idxIB{ 0 };
        IZ_UINT triNum{ 0 };

        MeshSubset() {}
        ~MeshSubset() {}
    };

    std::map<std::string, MeshSubset> meshes;
};

void CFbxImporter::FbxSdk::getNodes(FbxNode* node)
{
    nodes.push_back(node);

    // Gather materials.
    const int mtrlCount = node->GetMaterialCount();
    for (int i = 0; i < mtrlCount; i++)
    {
        FbxSurfaceMaterial* material = node->GetMaterial(i);
        if (material) {
            mtrls.insert(std::make_pair(material->GetName(), material));
        }
    }

    for (IZ_UINT i = 0; i < node->GetChildCount(); i++) {
        FbxNode* child = node->GetChild(i);

        getNodes(child);
    }
}

void CFbxImporter::FbxSdk::getMeshes()
{
    for each (auto node in nodes)
    {
        FbxNodeAttribute* attr = node->GetNodeAttribute();
        if (attr) {
            // Bake mesh as VBO(vertex buffer object) into GPU.
            if (attr->GetAttributeType() == FbxNodeAttribute::eMesh) {
                FbxMesh* mesh = node->GetMesh();

                const int polygonCount = mesh->GetPolygonCount();

                if (mesh->GetElementMaterial()) {
                    auto& materialIndices = mesh->GetElementMaterial()->GetIndexArray();

                    if (materialIndices.GetCount() == polygonCount) {
                        for (int i = 0; i < polygonCount; i++) {
                            const int materialIdx = materialIndices.GetAt(i);

                            auto material = node->GetMaterial(materialIdx);
                            std::string name(material->GetName());

                            if (meshes.find(name) != meshes.end()) {
                                meshes.insert(std::make_pair(name, MeshSubset()));
                            }

                            meshes[name].triNum += 1;
                        }
                    }
                    else {
                        IZ_ASSERT(IZ_FALSE);
                    }

                    IZ_UINT offset = 0;
                    for (auto it = meshes.begin(); it != meshes.end(); it++) {
                        MeshSubset& subset = it->second;

                        subset.idxIB = offset;
                        offset += subset.triNum * 3;

                        // Reset to zero to re-use.
                        subset.triNum = 0;
                    }
                }
            }
        }
    }
}

IZ_INT CFbxImporter::FbxSdk::getNodeIndex(FbxNode* node)
{
    for (IZ_INT i = 0; i < nodes.size(); i++) {
        if (nodes[i] == node) {
            return i;
        }
    }

    return -1;
}

CFbxImporter::CFbxImporter()
{
    m_fbx = new FbxSdk();
}

IZ_BOOL CFbxImporter::Open(IZ_PCSTR pszName)
{
    m_fbx->manager = FbxManager::Create();

    FbxIOSettings* ios = FbxIOSettings::Create(m_fbx->manager, IOSROOT);
    m_fbx->manager->SetIOSettings(ios);

    m_fbx->scene = FbxScene::Create(m_fbx->manager, "");

    auto importer = FbxImporter::Create(m_fbx->manager, "");

    int fileFormat = -1;

    if (m_fbx->manager->GetIOPluginRegistry()->DetectReaderFileFormat(pszName, fileFormat)) {
        if (importer->Initialize(pszName, fileFormat)) {
            if (importer->Import(m_fbx->scene)) {
                importer->Destroy(true);

                // Convert Axis System to what is used in this example, if needed
                FbxAxisSystem SceneAxisSystem = m_fbx->scene->GetGlobalSettings().GetAxisSystem();
                FbxAxisSystem axisSystem(
                    FbxAxisSystem::eYAxis,
                    FbxAxisSystem::eParityOdd,
                    FbxAxisSystem::eLeftHanded);

                if (SceneAxisSystem != axisSystem)
                {
                    axisSystem.ConvertScene(m_fbx->scene);
                }

                // Convert Unit System to what is used in this example, if needed
                FbxSystemUnit SceneSystemUnit = m_fbx->scene->GetGlobalSettings().GetSystemUnit();
                if (SceneSystemUnit.GetScaleFactor() != 1.0)
                {
                    //The unit in this tool is centimeter.
                    FbxSystemUnit::cm.ConvertScene(m_fbx->scene);
                }

                // Convert mesh, NURBS and patch into triangle mesh
                FbxGeometryConverter geomConverter(m_fbx->manager);
                geomConverter.Triangulate(m_fbx->scene, true);

                m_fbx->getNodes(m_fbx->scene->GetRootNode());
                m_fbx->getMeshes();

                return IZ_TRUE;
            }
        }
    }

    IZ_ASSERT(IZ_FALSE);
    return IZ_FALSE;
}

IZ_BOOL CFbxImporter::Close()
{
    if (m_fbx->scene) {
        m_fbx->scene->Destroy(true);
    }

    if (m_fbx->manager) {
        // Delete the FBX SDK manager. All the objects that have been allocated 
        // using the FBX SDK manager and that haven't been explicitly destroyed 
        // are automatically destroyed at the same time.
        m_fbx->manager->Destroy();
    }

    return IZ_TRUE;
}

//////////////////////////////////
// For geometry chunk.

void CFbxImporter::ExportGeometryCompleted()
{
}

void CFbxImporter::BeginMesh(IZ_UINT nIdx)
{
    m_curMeshIdx = nIdx;
}

void CFbxImporter::EndMesh()
{
}

IZ_UINT CFbxImporter::GetMeshNum()
{
    IZ_UINT ret = m_fbx->meshes.size();
    return ret;
}

// スキニング情報を取得.
void CFbxImporter::GetSkinList(std::vector<SSkin>& tvSkinList)
{
    for (IZ_UINT i = 0; i < m_fbx->nodes.size(); i++) {
        FbxNode* node = m_fbx->nodes[i];

        FbxNodeAttribute* attr = node->GetNodeAttribute();
        if (attr) {
            if (attr->GetAttributeType() != FbxNodeAttribute::eMesh) {
                continue;
            }
        }

        FbxMesh* mesh = node->GetMesh();

        int skinCount = mesh->GetDeformerCount(FbxDeformer::EDeformerType::eSkin);

        for (int n = 0; n < skinCount; n++) {
            FbxDeformer* deformer = mesh->GetDeformer(n, FbxDeformer::EDeformerType::eSkin);

            FbxSkin* skin = (FbxSkin*)deformer;

            int boneCount = skin->GetClusterCount();

            for (int b = 0; b < boneCount; b++) {
                FbxCluster* cluster = skin->GetCluster(b);

                // TODO
                IZ_ASSERT(cluster->GetLinkMode() == FbxCluster::ELinkMode::eTotalOne);

                FbxNode* targetNode = cluster->GetLink();

                IZ_INT nodeIdx = m_fbx->getNodeIndex(targetNode);
            }
        }
    }
}

// 指定されているメッシュに含まれる三角形を取得.
IZ_UINT CFbxImporter::GetTriangles(std::vector<STri>& tvTriList)
{
    return 0;
}

IZ_UINT CFbxImporter::GetSkinIdxAffectToVtx(IZ_UINT nVtxIdx)
{
    return 0;
}

IZ_UINT CFbxImporter::GetVtxSize()
{
    return 0;
}

IZ_UINT CFbxImporter::GetVtxFmt()
{
    return 0;
}

IZ_BOOL CFbxImporter::GetVertex(
    IZ_UINT nIdx,
    izanagi::math::SVector4& vec,
    izanagi::E_MSH_VTX_FMT_TYPE type)
{
    return IZ_FALSE;
}

void CFbxImporter::GetMaterialForMesh(
    IZ_UINT nIdx,
    izanagi::S_MSH_MTRL& sMtrl)
{
}

//////////////////////////////////
// For joint chunk.

void CFbxImporter::ExportJointCompleted()
{
}

IZ_BOOL CFbxImporter::BeginJoint()
{
    return IZ_TRUE;
}

void CFbxImporter::EndJoint()
{
}

IZ_UINT CFbxImporter::GetJointNum()
{
    return 0;
}

IZ_PCSTR CFbxImporter::GetJointName(IZ_UINT nIdx)
{
    return NULL;
}

IZ_INT CFbxImporter::GetJointParent(
    IZ_UINT nIdx,
    const std::vector<izanagi::S_SKL_JOINT>& tvJoint)
{
    return 0;
}

void CFbxImporter::GetJointInvMtx(
    IZ_UINT nIdx,
    izanagi::math::SMatrix44& mtx)
{
}

void CFbxImporter::GetJointTransform(
    IZ_UINT nIdx,
    const std::vector<izanagi::S_SKL_JOINT>& tvJoint,
    std::vector<SJointTransform>& tvTransform)
{
}

//////////////////////////////////
// For animation.

IZ_BOOL CFbxImporter::ReadBaseModel(IZ_PCSTR pszName)
{
    return IZ_FALSE;
}

IZ_UINT CFbxImporter::GetAnmSetNum()
{
    return 0;
}

IZ_BOOL CFbxImporter::BeginAnm(IZ_UINT nSetIdx)
{
    return IZ_FALSE;
}

IZ_BOOL CFbxImporter::EndAnm()
{
    return IZ_FALSE;
}

IZ_UINT CFbxImporter::GetAnmNodeNum()
{
    return 0;
}

IZ_UINT CFbxImporter::GetAnmChannelNum(IZ_UINT nNodeIdx)
{
    return 0;
}

IZ_BOOL CFbxImporter::GetAnmNode(
    IZ_UINT nNodeIdx,
    izanagi::S_ANM_NODE& sNode)
{
    return IZ_FALSE;
}

IZ_BOOL CFbxImporter::GetAnmChannel(
    IZ_UINT nNodeIdx,
    IZ_UINT nChannelIdx,
    izanagi::S_ANM_CHANNEL& sChannel)
{
    return IZ_FALSE;
}

IZ_BOOL CFbxImporter::GetAnmKey(
    IZ_UINT nNodeIdx,
    IZ_UINT nChannelIdx,
    IZ_UINT nKeyIdx,
    izanagi::S_ANM_KEY& sKey,
    std::vector<IZ_FLOAT>& tvValue)
{
    return IZ_FALSE;
}

//////////////////////////////////
// For material.

IZ_BOOL CFbxImporter::BeginMaterial()
{
    return IZ_FALSE;
}

IZ_BOOL CFbxImporter::EndMaterial()
{
    return IZ_FALSE;
}

IZ_UINT CFbxImporter::GetMaterialNum()
{
    return IZ_FALSE;
}

IZ_BOOL CFbxImporter::GetMaterial(
    IZ_UINT nMtrlIdx,
    izanagi::S_MTRL_MATERIAL& sMtrl)
{
    return IZ_FALSE;
}

void CFbxImporter::GetMaterialTexture(
    IZ_UINT nMtrlIdx,
    IZ_UINT nTexIdx,
    izanagi::S_MTRL_TEXTURE& sTex)
{
}

void CFbxImporter::GetMaterialShader(
    IZ_UINT nMtrlIdx,
    IZ_UINT nShaderIdx,
    izanagi::S_MTRL_SHADER& sShader)
{
}

void CFbxImporter::GetMaterialParam(
    IZ_UINT nMtrlIdx,
    IZ_UINT nParamIdx,
    izanagi::S_MTRL_PARAM& sParam)
{
}

void CFbxImporter::GetMaterialParamValue(
    IZ_UINT nMtrlIdx,
    IZ_UINT nParamIdx,
    std::vector<IZ_FLOAT>& tvValue)
{
}
