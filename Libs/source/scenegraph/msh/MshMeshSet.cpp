#include "scenegraph/msh/MshMeshSet.h"
#include "scenegraph/msh/MshPrimitiveSet.h"
#include "izGraph.h"
#include "izIo.h"

#include "scenegraph/msh/MshRenderHandler.h"

using namespace izanagi;

CMeshSet::CMeshSet()
{
    FILL_ZERO(&m_Info, sizeof(m_Info));

    m_VD = IZ_NULL;

    m_pPrimSet = IZ_NULL;
}

CMeshSet::~CMeshSet()
{
    for (IZ_UINT i = 0; i < m_Info.numSubset; ++i) {
        SAFE_RELEASE(m_pPrimSet[i]);
    }

    SAFE_RELEASE(m_VD);
}

IZ_BOOL CMeshSet::Render(
    graph::CGraphicsDevice* pDevice,
    CSkeletonInstance* pSkeleton,
    IMshRenderHandler* pRenderHandler)
{
    VRETURN(pDevice->SetVertexDeclaration(m_VD));

    for (IZ_UINT i = 0; i < m_Info.numSubset; ++i) {
        if (pRenderHandler != IZ_NULL) {
            pRenderHandler->BeginRenderMesh();
        }

#if 1
        VRETURN(
            m_pPrimSet[i]->Render(
                pDevice,
                pSkeleton,
                pRenderHandler));
#else
        VRETURN(
            m_pPrimSet[i]->DebugRender(
                pDevice,
                pSkeleton,
                pRenderHandler));
#endif

        if (pRenderHandler != IZ_NULL) {
            pRenderHandler->EndRenderMesh();
        }
    }

    return IZ_TRUE;
}

namespace {
    // 位置
    IZ_WORD _SetVtxElementPos(graph::SVertexElement* pElem, IZ_WORD nOffset)
    {
        pElem->Offset = nOffset;
        pElem->Type = graph::E_GRAPH_VTX_DECL_TYPE_FLOAT4;
        pElem->Usage = graph::E_GRAPH_VTX_DECL_USAGE_POSITION;
        nOffset += E_MSH_VTX_SIZE_POS;
        return nOffset;
    }

    // 法線
    IZ_WORD _SetVtxElementNormal(graph::SVertexElement* pElem, IZ_WORD nOffset)
    {
        pElem->Offset = nOffset;
        pElem->Type = graph::E_GRAPH_VTX_DECL_TYPE_FLOAT3;
        pElem->Usage = graph::E_GRAPH_VTX_DECL_USAGE_NORMAL;
        nOffset += E_MSH_VTX_SIZE_NORMAL;
        return nOffset;
    }

    // 頂点カラー
    IZ_WORD _SetVtxElementColor(graph::SVertexElement* pElem, IZ_WORD nOffset)
    {
        pElem->Offset = nOffset;
        pElem->Type = graph::E_GRAPH_VTX_DECL_TYPE_COLOR;
        pElem->Usage = graph::E_GRAPH_VTX_DECL_USAGE_COLOR;
        nOffset += E_MSH_VTX_SIZE_COLOR;
        return nOffset;
    }

    // UV座標
    IZ_WORD _SetVtxElementUV(graph::SVertexElement* pElem, IZ_WORD nOffset)
    {
        pElem->Offset = nOffset;
        pElem->Type = graph::E_GRAPH_VTX_DECL_TYPE_FLOAT2;
        pElem->Usage = graph::E_GRAPH_VTX_DECL_USAGE_TEXCOORD;
        nOffset += E_MSH_VTX_SIZE_UV;
        return nOffset;
    }

    // 接ベクトル
    IZ_WORD _SetVtxElementTangent(graph::SVertexElement* pElem, IZ_WORD nOffset)
    {
        pElem->Offset = nOffset;
        pElem->Type = graph::E_GRAPH_VTX_DECL_TYPE_FLOAT3;
        pElem->Usage = graph::E_GRAPH_VTX_DECL_USAGE_TANGENT;
        nOffset += E_MSH_VTX_SIZE_TANGENT;
        return nOffset;
    }

    // ブレンドウエイト
    IZ_WORD _SetVtxElementBlendWeight(graph::SVertexElement* pElem, IZ_WORD nOffset)
    {
        pElem->Offset = nOffset;
        pElem->Type = graph::E_GRAPH_VTX_DECL_TYPE_FLOAT4;
        pElem->Usage = graph::E_GRAPH_VTX_DECL_USAGE_BLENDWEIGHT;
        nOffset += E_MSH_VTX_SIZE_BLENDWEIGHT;
        return nOffset;
    }

    // ブレンドインデックス
    IZ_WORD _SetVtxElementBlendIndices(graph::SVertexElement* pElem, IZ_WORD nOffset)
    {
        pElem->Offset = nOffset;
        pElem->Type = graph::E_GRAPH_VTX_DECL_TYPE_FLOAT4;
        pElem->Usage = graph::E_GRAPH_VTX_DECL_USAGE_BLENDINDICES;
        nOffset += E_MSH_VTX_SIZE_BLENDINDICES;
        return nOffset;
    }

    // Create VertexDeclaration.
    graph::CVertexDeclaration* _CreateVertexDeclaration(
        graph::CGraphicsDevice* pDevice,
        const S_MSH_MESH_SET& sMesh)
    {
        typedef IZ_WORD (*FuncSetVtxElem)(graph::SVertexElement*, IZ_WORD);

        static FuncSetVtxElem funcSetVtxElem[] = {
            _SetVtxElementPos,
            _SetVtxElementNormal,
            _SetVtxElementColor,
            _SetVtxElementUV,
            _SetVtxElementTangent,
            _SetVtxElementBlendIndices,
            _SetVtxElementBlendWeight,
        };
        IZ_C_ASSERT(COUNTOF(funcSetVtxElem) == E_MSH_VTX_FMT_TYPE_NUM);

        graph::SVertexElement sVtxElem[E_MSH_VTX_FMT_TYPE_NUM];
        FILL_ZERO(&sVtxElem, sizeof(sVtxElem));

        IZ_WORD nOffset = 0;
        IZ_WORD nVtxElemNum = 0;

        for (IZ_UINT i = 0; i < E_MSH_VTX_FMT_TYPE_NUM; ++i) {
            if ((sMesh.fmt & (1 << i)) > 0) {
                nOffset = (*funcSetVtxElem[i])(&sVtxElem[nVtxElemNum], nOffset);
                ++nVtxElemNum;
            }
        }

        graph::CVertexDeclaration* pVD = pDevice->CreateVertexDeclaration(
                                    sVtxElem,
                                    nVtxElemNum);
        IZ_ASSERT_NULL(pVD);

        return pVD;
    }
}   // namespace

IZ_UINT8* CMeshSet::Read(
    IZ_UINT8* pBuf,
    graph::CGraphicsDevice* pDevice,
    IInputStream* pIn)
{
    IZ_INPUT_READ_ASSERT(pIn, &m_Info, 0, sizeof(m_Info));

    // MeshSetの頂点宣言を作成
    m_VD = _CreateVertexDeclaration(pDevice, m_Info);
    VRETURN(m_VD != IZ_NULL);

    m_pPrimSet = reinterpret_cast<CPrimitiveSet**>(pBuf);

    pBuf += sizeof(CPrimitiveSet*) * m_Info.numSubset;

    for (IZ_UINT i = 0; i < m_Info.numSubset; ++i) {
        // Create mesh.
        m_pPrimSet[i] = new(pBuf) CPrimitiveSet;
        pBuf += sizeof(CPrimitiveSet);

        m_pPrimSet[i]->AddRef();

        // PrimSetのデータを読み込み
        pBuf = m_pPrimSet[i]->Read(pDevice, pIn, pBuf);
        VRETURN_NULL(pBuf != IZ_NULL);

        // TODO
    }

    return pBuf;
}

CPrimitiveSet* CMeshSet::GetPrimSet(IZ_UINT idx)
{
    IZ_ASSERT(idx < m_Info.numSubset);
    IZ_ASSERT(m_pPrimSet != IZ_NULL);

    return m_pPrimSet[idx];
}
