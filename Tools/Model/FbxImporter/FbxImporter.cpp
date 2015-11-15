#include "fbxsdk.h"
#include "FbxImporter.h"

#include <map>

// NOTE
// http://www.gamedev.net/page/resources/_/technical/graphics-programming-and-theory/how-to-work-with-fbx-sdk-r3582

class CFbxImporter::FbxSdk {
public:
    struct Face {
        FbxMesh* mesh;
        int polygonIdx;

        Face(FbxMesh* _mesh, int _idx)
            : mesh(_mesh), polygonIdx(_idx)
        {}
    };

    struct Vertex {
        FbxMesh* mesh;

        // Fbxにおけるインデックス.
        IZ_UINT fbxIdx;

        // メッシュ内での頂点カウント.
        IZ_UINT countInFbxMesh;

        Vertex(FbxMesh* _mesh, int _idx, int _pos)
            : mesh(_mesh), fbxIdx(_idx), countInFbxMesh(_pos)
        {}
    };

    struct MeshSet {
        // 頂点オフセット.
        IZ_UINT offset;

        std::vector<Face> faces;

        std::vector<Vertex> vertices;

        // 頂点数取得.
        IZ_UINT getVertexNum() const
        {
            IZ_UINT ret = vertices.size();
            return ret;
        }

        // 頂点列のインデックスを取得.
        IZ_INT find(FbxMesh* mesh, IZ_UINT fbxIdx) const
        {
            for (IZ_UINT i = 0; i < vertices.size(); i++)
            {
                const Vertex& v = vertices[i];

                if (v.mesh == mesh && v.fbxIdx == fbxIdx) {
                    return i;
                }
            }
            return -1;
        }

        void add(FbxMesh* mesh, IZ_UINT idx, IZ_UINT pos)
        {
            IZ_INT posVtx = find(mesh, idx);
            if (posVtx < 0) {
                vertices.push_back(Vertex(mesh, idx, pos));
            }
        }

        // 全頂点を通じた頂点インデックスを取得.
        IZ_BOOL getVtxIdx(FbxMesh* mesh, IZ_UINT fbxIdx, IZ_UINT& result) const
        {
            IZ_INT pos = find(mesh, fbxIdx);
            if (pos < 0) {
                return IZ_FALSE;
            }

            result = pos + offset;
            return IZ_TRUE;
        }
    };

    struct Mesh{
        FbxMesh* mesh;

        // メッシュが保持する頂点カウント.
        IZ_UINT vtxCounter;

        Mesh(FbxMesh* _mesh)
            : mesh(_mesh), vtxCounter{ 0 }
        {}

        bool operator==(const Mesh& rhs) const
        {
            return (mesh == rhs.mesh);
        }
    };

public:
    FbxSdk(){}
    ~FbxSdk(){}

    // ノードを集める.
    void getNodes(FbxNode* node);

    // メッシュをマテリアルごとにまとめる.
    void getMeshes();

    void getClusters();

    FbxCluster* getClusterByNode(const FbxNode* node);

    // 指定されたノードのインデックスを取得.
    IZ_INT getNodeIndex(const FbxNode* node);

    const MeshSet& getMeshSet(IZ_UINT idx);

    // 頂点データを取得.
    Vertex* getVertexData(IZ_UINT vtxIdx);

    // 全頂点を通じた頂点インデックスを取得.
    IZ_UINT getVtxIdx(FbxMesh* mesh, IZ_UINT fbxVtxIdx);

public:
    FbxManager* manager{ nullptr };
    FbxScene* scene{ nullptr };

    std::vector<FbxNode*> nodes;
    std::map<std::string, FbxSurfaceMaterial*> mtrls;

    std::vector<Mesh> meshes;

    std::map<std::string, MeshSet> meshSets;

    IZ_UINT vtxNum{ 0 };

    std::vector<FbxCluster*> clusters;
};

// ノードを集める.
void CFbxImporter::FbxSdk::getNodes(FbxNode* node)
{
    // NOTE
    // RootNodeは除外する.
    if (node != this->scene->GetRootNode()) {
        nodes.push_back(node);
    }

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

// メッシュをマテリアルごとにまとめる.
void CFbxImporter::FbxSdk::getMeshes()
{
    for each (auto node in nodes)
    {
        FbxNodeAttribute* attr = node->GetNodeAttribute();
        if (attr == NULL) {
            continue;
        }
            
        if (attr->GetAttributeType() != FbxNodeAttribute::eMesh) {
            continue;
        }

        FbxMesh* mesh = node->GetMesh();

        if (mesh->GetElementMaterial() == NULL) {
            continue;
        }

        // 頂点カウント.
        IZ_UINT vtxCounter = 0;

        Mesh m(mesh);
        auto it = std::find(this->meshes.begin(), this->meshes.end(), m);
        if (it == this->meshes.end()) {
            this->meshes.push_back(m);
            it = this->meshes.begin();
        }
        else {
            vtxCounter = it->vtxCounter;
        }

        // メッシュに含まれるポリゴン（三角形）の数.
        const int polygonCount = mesh->GetPolygonCount();

        auto& materialIndices = mesh->GetElementMaterial()->GetIndexArray();

        if (materialIndices.GetCount() == polygonCount) {
            // メッシュに含まれるポリゴン（三角形）ごとにどのマテリアルに所属しているのかを調べる.
            for (int i = 0; i < polygonCount; i++) {
                // メッシュに含まれるポリゴン（三角形）が所属しているマテリアルへのインデックス.
                const int materialIdx = materialIndices.GetAt(i);

                // マテリアル本体を取得.
                auto material = node->GetMaterial(materialIdx);
                std::string name(material->GetName());

                if (meshSets.find(name) != meshSets.end()) {
                    meshSets.insert(std::make_pair(name, MeshSet()));
                }

                meshSets[name].faces.push_back(Face(mesh, i));

                // 三角形なので３点.
                meshSets[name].add(mesh, mesh->GetPolygonVertex(i, 0), vtxCounter++);
                meshSets[name].add(mesh, mesh->GetPolygonVertex(i, 1), vtxCounter++);
                meshSets[name].add(mesh, mesh->GetPolygonVertex(i, 2), vtxCounter++);
            }
        }
        else {
            IZ_ASSERT(IZ_FALSE);
        }

        // Update.
        it->vtxCounter = vtxCounter;
    }

    IZ_UINT offset = 0;
    for (auto it = meshSets.begin(); it != meshSets.end(); it++) {
        auto& set = it->second;

        set.offset = offset;

        offset += set.getVertexNum();
    }

    // 頂点数を保持.
    vtxNum = offset;
}

void CFbxImporter::FbxSdk::getClusters()
{
    for each (CFbxImporter::FbxSdk::Mesh m in this->meshes)
    {
        FbxMesh* mesh = m.mesh;

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

                auto it = std::find(this->clusters.begin(), this->clusters.end(), cluster);
                if (it == this->clusters.end()) {
                    this->clusters.push_back(cluster);
                }
            }
        }
    }
}

FbxCluster* CFbxImporter::FbxSdk::getClusterByNode(const FbxNode* node)
{
    for each (auto cluster in this->clusters)
    {
        FbxNode* targetNode = cluster->GetLink();

        if (node == targetNode) {
            return cluster;
        }
    }

    return nullptr;
}

IZ_INT CFbxImporter::FbxSdk::getNodeIndex(const FbxNode* node)
{
    for (IZ_INT i = 0; i < nodes.size(); i++) {
        if (nodes[i] == node) {
            return i;
        }
    }

    return -1;
}

const CFbxImporter::FbxSdk::MeshSet& CFbxImporter::FbxSdk::getMeshSet(IZ_UINT idx)
{
    IZ_ASSERT(idx < this->meshSets.size());

    auto it = this->meshSets.begin();
    std::advance(it, idx);

    auto& ret = it->second;
    return ret;
}

CFbxImporter::FbxSdk::Vertex* CFbxImporter::FbxSdk::getVertexData(IZ_UINT vtxIdx)
{
    IZ_UINT begin = 0;

    for (auto it = this->meshSets.begin(); it != this->meshSets.end(); it++) {
        auto& meshSet = it->second;

        IZ_UINT vtxNum = meshSet.getVertexNum();

        IZ_UINT end = begin + vtxNum;

        if (begin <= vtxIdx && vtxIdx < end) {
            IZ_UINT idx = vtxIdx - begin;

            auto& vtx = meshSet.vertices[idx];

            return &vtx;
        }

        begin += vtxNum;
    }
    
    IZ_ASSERT(IZ_FALSE);
    return nullptr;
}

// 全頂点を通じた頂点インデックスを取得.
IZ_UINT CFbxImporter::FbxSdk::getVtxIdx(FbxMesh* mesh, IZ_UINT fbxVtxIdx)
{
    IZ_UINT ret = 0;

    for (auto it = this->meshSets.begin(); it != this->meshSets.end(); it++)
    {
        auto& meshSet = it->second;

        if (meshSet.getVtxIdx(mesh, fbxVtxIdx, ret)) {
            return ret;
        }
    }

    IZ_ASSERT(IZ_FALSE);
    return ret;
}

////////////////////////////////////////////////////////////////////

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

#if 0
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
#endif

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
                m_fbx->getClusters();

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
    IZ_UINT ret = m_fbx->meshSets.size();
    return ret;
}

// スキニング情報を取得.
void CFbxImporter::GetSkinList(std::vector<SSkin>& tvSkinList)
{
    if (tvSkinList.size() > 0) {
        return;
    }

    tvSkinList.resize(m_fbx->vtxNum);

    for each (CFbxImporter::FbxSdk::Mesh m in m_fbx->meshes)
    {
        FbxMesh* mesh = m.mesh;

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

                // TODO
                //IZ_ASSERT(cluster->GetLinkMode() == FbxCluster::ELinkMode::eTotalOne);

                // ボーンと関連づいているノード.
                FbxNode* targetNode = cluster->GetLink();

                // ノードのインデックス.
                IZ_INT nodeIdx = m_fbx->getNodeIndex(targetNode);

                // ボーンが影響を与える頂点数.
                int influencedVtxNum = cluster->GetControlPointIndicesCount();

                for (int v = 0; v < influencedVtxNum; v++) {
                    int vtxIdx = cluster->GetControlPointIndices()[v];

                    // 全体での頂点インデックスに変換.
                    vtxIdx = m_fbx->getVtxIdx(mesh, vtxIdx);

                    float weight = (float)cluster->GetControlPointWeights()[v];

                    tvSkinList[vtxIdx].Add(nodeIdx, weight);
                }
            }
        }
    }
}

// 指定されているメッシュに含まれる三角形を取得.
IZ_UINT CFbxImporter::GetTriangles(std::vector<STri>& tvTriList)
{
    const auto& meshSet = m_fbx->getMeshSet(m_curMeshIdx);

    // NOTE
    // 三角形を構成する頂点に影響を与える（スキニングする）関節インデックスについては外部で処理するので、ここでは何もしない.

    for each (auto& face in meshSet.faces)
    {
        // Check if this is a triangle.
        //IZ_ASSERT(face.mNumIndices == 3);

        // NOTE
        // 全頂点を通じた一意のインデックスを指定する.

        STri tri;
        
        // NOTE
        // 三角形なので３頂点.
        int idx0 = face.mesh->GetPolygonVertex(face.polygonIdx, 0);
        int idx1 = face.mesh->GetPolygonVertex(face.polygonIdx, 1);
        int idx2 = face.mesh->GetPolygonVertex(face.polygonIdx, 2);

        IZ_BOOL result = meshSet.getVtxIdx(face.mesh, idx0, tri.vtx[0]);
        IZ_ASSERT(result);

        result = meshSet.getVtxIdx(face.mesh, idx1, tri.vtx[1]);
        IZ_ASSERT(result);

        result = meshSet.getVtxIdx(face.mesh, idx2, tri.vtx[2]);
        IZ_ASSERT(result);

        tvTriList.push_back(tri);
    }

    IZ_UINT ret = meshSet.getVertexNum();

    return ret;
}

IZ_UINT CFbxImporter::GetSkinIdxAffectToVtx(IZ_UINT nVtxIdx)
{
    // NOTE
    // 頂点インデックスとスキン情報のインデックスは一致する.
    return nVtxIdx;
}

IZ_UINT CFbxImporter::GetVtxSize()
{
    // NOTE
    // skinのサイズは外でやるのでここではやらない

    auto& meshSet = m_fbx->getMeshSet(m_curMeshIdx);

    IZ_UINT ret = 0;

    // TODO
    // １つのメッシュを見るだけで大丈夫か.

    FbxMesh* mesh = meshSet.faces[0].mesh;

    // ポジションが存在しないことはないはず.
    ret += izanagi::E_MSH_VTX_SIZE::E_MSH_VTX_SIZE_POS;

    if (mesh->GetElementNormalCount() > 0) {
        ret += izanagi::E_MSH_VTX_SIZE::E_MSH_VTX_SIZE_NORMAL;
    }

    if (mesh->GetElementUVCount() > 0) {
        ret += izanagi::E_MSH_VTX_SIZE::E_MSH_VTX_SIZE_UV;
    }

    if (mesh->GetElementVertexColorCount() > 0) {
        ret += izanagi::E_MSH_VTX_SIZE::E_MSH_VTX_SIZE_COLOR;
    }

    return ret;
}

IZ_UINT CFbxImporter::GetVtxFmt()
{
    auto& meshSet = m_fbx->getMeshSet(m_curMeshIdx);

    IZ_UINT ret = 0;

    // TODO
    // １つのメッシュを見るだけで大丈夫か.

    FbxMesh* mesh = meshSet.faces[0].mesh;

    // ポジションが存在しないことはないはず.
    ret |= 1 << izanagi::E_MSH_VTX_FMT_TYPE::E_MSH_VTX_FMT_TYPE_POS;

    if (mesh->GetElementNormalCount() > 0) {
        ret |= 1 << izanagi::E_MSH_VTX_FMT_TYPE::E_MSH_VTX_FMT_TYPE_NORMAL;
    }

    if (mesh->GetElementUVCount() > 0) {
        ret |= 1 << izanagi::E_MSH_VTX_FMT_TYPE::E_MSH_VTX_FMT_TYPE_UV;
    }

    if (mesh->GetElementVertexColorCount() > 0) {
        ret |= 1 << izanagi::E_MSH_VTX_FMT_TYPE::E_MSH_VTX_FMT_TYPE_COLOR;
    }

    // NOTE
    // skinのフォーマットは外でやるのでここではやらない

    return ret;
}

namespace {
    IZ_BOOL GetVertexData(
        fbxsdk::FbxLayerElement::EMappingMode mappingMode,
        fbxsdk::FbxLayerElement::EReferenceMode referenceMode,
        IZ_UINT vtxIdx, IZ_UINT vtxCounter,
        std::function<void(IZ_UINT)> funcDirect,
        std::function<void(IZ_UINT)> funcIndex)
    {
        IZ_BOOL ret = IZ_FALSE;

        switch (mappingMode)
        {
        case FbxGeometryElement::eByControlPoint:
            switch (referenceMode)
            {
            case FbxGeometryElement::eDirect:
            {
                funcDirect(vtxIdx);
                ret = IZ_TRUE;
            }
                break;

            case FbxGeometryElement::eIndexToDirect:
            {
                funcIndex(vtxIdx);
                ret = IZ_TRUE;
            }
                break;

            default:
                throw std::exception("Invalid Reference");
            }
            break;

        case FbxGeometryElement::eByPolygonVertex:
            // NOTE
            // 頂点の順番でアクセスする場合.
            switch (referenceMode)
            {
            case FbxGeometryElement::eDirect:
            {
                funcDirect(vtxCounter);
                ret = IZ_TRUE;
            }
                break;

            case FbxGeometryElement::eIndexToDirect:
            {
                funcIndex(vtxCounter);
                ret = IZ_TRUE;
            }
                break;

            default:
                throw std::exception("Invalid Reference");
            }
            break;
        }

        return ret;
    }
}

IZ_BOOL CFbxImporter::GetVertex(
    IZ_UINT nIdx,
    izanagi::math::SVector4& vec,
    izanagi::E_MSH_VTX_FMT_TYPE type)
{
    auto vtx = m_fbx->getVertexData(nIdx);
    VRETURN(vtx);

    IZ_UINT realVtxIdx = vtx->fbxIdx;
    IZ_UINT countInFbxMesh = vtx->countInFbxMesh;
    FbxMesh* mesh = vtx->mesh;

    if (type == izanagi::E_MSH_VTX_FMT_TYPE::E_MSH_VTX_FMT_TYPE_POS) {
        const FbxVector4& pos = mesh->GetControlPointAt(realVtxIdx);

        vec.x = static_cast<float>(pos.mData[0]);
        vec.y = static_cast<float>(pos.mData[1]);
        vec.z = static_cast<float>(pos.mData[2]);
        vec.w = 1.0f;

        return IZ_TRUE;
    }

    if (type == izanagi::E_MSH_VTX_FMT_TYPE::E_MSH_VTX_FMT_TYPE_NORMAL) {
        if (mesh->GetElementNormalCount() > 0) {
            const FbxGeometryElementNormal* vtxNormal = mesh->GetElementNormal();

            IZ_UINT numDirect = vtxNormal->GetDirectArray().GetCount();
            IZ_UINT numIndex = vtxNormal->GetIndexArray().GetCount();

            auto mappingMode = vtxNormal->GetMappingMode();
            auto referenceMode = vtxNormal->GetReferenceMode();

            GetVertexData(
                mappingMode, referenceMode,
                realVtxIdx, countInFbxMesh,
                [&](IZ_UINT idx) {
                const FbxVector4& nml = vtxNormal->GetDirectArray().GetAt(idx);
                vec.x = static_cast<float>(nml.mData[0]);
                vec.y = static_cast<float>(nml.mData[1]);
                vec.z = static_cast<float>(nml.mData[2]);
            },
                [&](IZ_UINT idx) {
                int index = vtxNormal->GetIndexArray().GetAt(idx);
                const FbxVector4& nml = vtxNormal->GetDirectArray().GetAt(index);
                vec.x = static_cast<float>(nml.mData[0]);
                vec.y = static_cast<float>(nml.mData[1]);
                vec.z = static_cast<float>(nml.mData[2]);
            });

            return IZ_TRUE;
        }
    }

    if (type == izanagi::E_MSH_VTX_FMT_TYPE::E_MSH_VTX_FMT_TYPE_UV) {
        if (mesh->GetElementUVCount() > 0) {
            const FbxGeometryElementUV* vtxUV = mesh->GetElementUV(0, FbxLayerElement::EType::eTextureDiffuse);
            IZ_ASSERT(vtxUV != NULL);

            IZ_UINT numDirect = vtxUV->GetDirectArray().GetCount();
            IZ_UINT numIndex = vtxUV->GetIndexArray().GetCount();

            auto mappingMode = vtxUV->GetMappingMode();
            auto referenceMode = vtxUV->GetReferenceMode();

            GetVertexData(
                mappingMode, referenceMode,
                realVtxIdx, countInFbxMesh,
                [&](IZ_UINT idx) {
                const FbxVector2& uv = vtxUV->GetDirectArray().GetAt(idx);
                vec.x = static_cast<float>(uv.mData[0]);
                vec.y = static_cast<float>(uv.mData[1]);
            },
                [&](IZ_UINT idx) {
                int index = vtxUV->GetIndexArray().GetAt(idx);
                const FbxVector2& uv = vtxUV->GetDirectArray().GetAt(index);
                vec.x = static_cast<float>(uv.mData[0]);
                vec.y = static_cast<float>(uv.mData[1]);
            });
                
            return IZ_TRUE;
        }
    }

    if (type == izanagi::E_MSH_VTX_FMT_TYPE::E_MSH_VTX_FMT_TYPE_COLOR) {
        if (mesh->GetElementVertexColorCount() > 0) {
            const FbxGeometryElementVertexColor* vtxClr = mesh->GetElementVertexColor();

            IZ_UINT numDirect = vtxClr->GetDirectArray().GetCount();
            IZ_UINT numIndex = vtxClr->GetIndexArray().GetCount();

            auto mappingMode = vtxClr->GetMappingMode();
            auto referenceMode = vtxClr->GetReferenceMode();

            GetVertexData(
                mappingMode, referenceMode,
                realVtxIdx, countInFbxMesh,
                [&](IZ_UINT idx) {
                const FbxColor& clr = vtxClr->GetDirectArray().GetAt(idx);
                vec.x = static_cast<float>(clr.mRed);
                vec.y = static_cast<float>(clr.mGreen);
                vec.z = static_cast<float>(clr.mBlue);
                vec.w = static_cast<float>(clr.mAlpha);
            },
                [&](IZ_UINT idx) {
                int index = vtxClr->GetIndexArray().GetAt(idx);
                const FbxColor& clr = vtxClr->GetDirectArray().GetAt(index);
                vec.x = static_cast<float>(clr.mRed);
                vec.y = static_cast<float>(clr.mGreen);
                vec.z = static_cast<float>(clr.mBlue);
                vec.w = static_cast<float>(clr.mAlpha);
            });

            return IZ_TRUE;
        }
    }


    return IZ_FALSE;
}

void CFbxImporter::GetMaterialForMesh(
    IZ_UINT nMeshIdx,
    izanagi::S_MSH_MTRL& sMtrl)
{
    IZ_ASSERT(nMeshIdx < m_fbx->meshSets.size());

    auto it = m_fbx->meshSets.begin();
    std::advance(it, nMeshIdx);

    const std::string& name = it->first;

    sMtrl.name.SetString(name.c_str());
    sMtrl.nameKey = sMtrl.name.GetKeyValue();
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
    IZ_UINT ret = m_fbx->nodes.size();
    return ret;
}

IZ_PCSTR CFbxImporter::GetJointName(IZ_UINT nIdx)
{
    FbxNode* node = m_fbx->nodes[nIdx];

    return node->GetName();
}

IZ_INT CFbxImporter::GetJointParent(
    IZ_UINT nIdx,
    const std::vector<izanagi::S_SKL_JOINT>& tvJoint)
{
    FbxNode* node = m_fbx->nodes[nIdx];

    const FbxNode* parent = node->GetParent();
    if (parent == NULL) {
        return -1;
    }

    IZ_INT ret = m_fbx->getNodeIndex(parent);

    return ret;
}

void CFbxImporter::GetJointInvMtx(
    IZ_UINT nIdx,
    izanagi::math::SMatrix44& mtx)
{
    FbxNode* node = m_fbx->nodes[nIdx];
    FbxCluster* cluster = m_fbx->getClusterByNode(node);

    if (cluster) {
        FbxAMatrix& mtxGlobal = node->EvaluateGlobalTransform();
        FbxAMatrix& mtxLocal = node->EvaluateLocalTransform();

        FbxVector4 trans = node->LclTranslation;
        FbxVector4 rot = node->LclRotation;
        FbxVector4 scale = node->LclScaling;

        FbxAMatrix tmp(trans, rot, scale);

        FbxAMatrix mtxTransformLink;
        cluster->GetTransformLinkMatrix(mtxTransformLink);

        FbxAMatrix mtxTransform;
        cluster->GetTransformMatrix(mtxTransform);

        FbxAMatrix globalBindposeInverseMatrix = mtxTransformLink.Inverse();

        for (IZ_UINT i = 0; i < 4; i++) {
            for (IZ_UINT n = 0; n < 4; n++) {
                mtx.m[i][n] = static_cast<IZ_FLOAT>(globalBindposeInverseMatrix.Get(i, n));
            }
        }
    }
    else {
        izanagi::math::SMatrix44::SetUnit(mtx);
    }
}

void CFbxImporter::GetJointTransform(
    IZ_UINT nIdx,
    const std::vector<izanagi::S_SKL_JOINT>& tvJoint,
    std::vector<SJointTransform>& tvTransform)
{
    FbxNode* node = m_fbx->nodes[nIdx];

    FbxAMatrix& mtxLocal = node->EvaluateLocalTransform();

    const FbxVector4 trans = mtxLocal.GetT();
    const FbxQuaternion quat = mtxLocal.GetQ();
    const FbxVector4 scale = mtxLocal.GetS();

    // For quat.
    if (quat.mData[0] != 0.0f
        || quat.mData[1] != 0.0f
        || quat.mData[2] != 0.0f
        || quat.mData[3] != 1.0f)
    {
        tvTransform.push_back(SJointTransform());
        SJointTransform& sTransform = tvTransform.back();

        sTransform.type = E_MDL_JOINT_TRANSFORM_QUATERNION;

        sTransform.param.push_back(quat.mData[0]);
        sTransform.param.push_back(quat.mData[1]);
        sTransform.param.push_back(quat.mData[2]);
        sTransform.param.push_back(quat.mData[3]);
    }

    // For trans.
    if (trans.mData[0] != 0.0f
        || trans.mData[1] != 0.0f
        || trans.mData[2] != 0.0f)
    {
        tvTransform.push_back(SJointTransform());
        SJointTransform& sTransform = tvTransform.back();

        sTransform.type = E_MDL_JOINT_TRANSFORM_TRANSLATE;

        sTransform.param.push_back(trans.mData[0]);
        sTransform.param.push_back(trans.mData[1]);
        sTransform.param.push_back(trans.mData[2]);
    }

    // TODO
    // scale
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
