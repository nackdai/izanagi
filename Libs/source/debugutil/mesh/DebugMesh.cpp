#include "debugutil/mesh/DebugMesh.h"
#include "math/MathVector4.h"
#include "debugutil/mesh/DebugMeshAxis.h"

using namespace izanagi;

CDebugMeshUtil::SetElemFunc CDebugMeshUtil::SetElemFuncTbl[E_DEBUG_MESH_VTX_FORM_NUM] = {
    &CDebugMeshUtil::SetVtxElementPos,
    &CDebugMeshUtil::SetVtxElementNormal,
    &CDebugMeshUtil::SetVtxElementColor,
    &CDebugMeshUtil::SetVtxElementUV,
    &CDebugMeshUtil::SetVtxElementTangent,
};

CDebugMeshUtil::GetElemSizeFunc CDebugMeshUtil::GetElemSizeFuncTbl[E_DEBUG_MESH_VTX_FORM_NUM] = {
    &CDebugMeshUtil::GetPosSize,
    &CDebugMeshUtil::GetColorSize,
    &CDebugMeshUtil::GetNormalSize,
    &CDebugMeshUtil::GetUVSize,
    &CDebugMeshUtil::GetTangentSize,
};

// 位置
IZ_UINT CDebugMeshUtil::SetVtxElementPos(IZ_UINT flag, graph::SVertexElement* pElem, IZ_UINT nPos, IZ_WORD* pOffset)
{
    if (IsPos(flag)) {
        pElem[nPos].Offset = *pOffset;
        pElem[nPos].Type = graph::E_GRAPH_VTX_DECL_TYPE_FLOAT4;
        pElem[nPos].Usage = graph::E_GRAPH_VTX_DECL_USAGE_POSITION;
        *pOffset += GetPosSize(flag);
        return nPos + 1;
    }
    return nPos;
}

// 法線
IZ_UINT CDebugMeshUtil::SetVtxElementNormal(IZ_UINT flag, graph::SVertexElement* pElem, IZ_UINT nPos, IZ_WORD* pOffset)
{
    if (IsNormal(flag)) {
        pElem[nPos].Offset = *pOffset;
        pElem[nPos].Type = graph::E_GRAPH_VTX_DECL_TYPE_FLOAT3;
        pElem[nPos].Usage = graph::E_GRAPH_VTX_DECL_USAGE_NORMAL;
        *pOffset += GetNormalSize(flag);
        return nPos + 1;
    }
    return nPos;
}

// 頂点カラー
IZ_UINT CDebugMeshUtil::SetVtxElementColor(IZ_UINT flag, graph::SVertexElement* pElem, IZ_UINT nPos, IZ_WORD* pOffset)
{
    if (IsColor(flag)) {
        pElem[nPos].Offset = *pOffset;
        pElem[nPos].Type = graph::E_GRAPH_VTX_DECL_TYPE_COLOR;
        pElem[nPos].Usage = graph::E_GRAPH_VTX_DECL_USAGE_COLOR;
        *pOffset += GetColorSize(flag);
        return nPos + 1;
    }
    return nPos;
}

// UV座標
IZ_UINT CDebugMeshUtil::SetVtxElementUV(IZ_UINT flag, graph::SVertexElement* pElem, IZ_UINT nPos, IZ_WORD* pOffset)
{
    if (IsUV(flag)) {
        pElem[nPos].Offset = *pOffset;
        pElem[nPos].Type = graph::E_GRAPH_VTX_DECL_TYPE_FLOAT2;
        pElem[nPos].Usage = graph::E_GRAPH_VTX_DECL_USAGE_TEXCOORD;
        *pOffset += GetUVSize(flag);
        return nPos + 1;
    }
    return nPos;
}

// 接ベクトル
IZ_UINT CDebugMeshUtil::SetVtxElementTangent(IZ_UINT flag, graph::SVertexElement* pElem, IZ_UINT nPos, IZ_WORD* pOffset)
{
    if (IsTangent(flag)) {
        pElem[nPos].Offset = *pOffset;
        pElem[nPos].Type = graph::E_GRAPH_VTX_DECL_TYPE_FLOAT4;
        pElem[nPos].Usage = graph::E_GRAPH_VTX_DECL_USAGE_TANGENT;
        *pOffset += GetTangentSize(flag);
        return nPos + 1;
    }
    return nPos;
}

//////////////////////////////////////////////////////////////////

CDebugMesh::CDebugMesh()
{
    m_pDebugAxis = IZ_NULL;
}

CDebugMesh::~CDebugMesh()
{
    SAFE_RELEASE(m_pDebugAxis);
}

IZ_BOOL CDebugMesh::CreateDebugAxis(
    graph::CGraphicsDevice* device,
    IZ_UINT nVtxNum,
    IZ_UINT flag)
{
    IZ_ASSERT(nVtxNum > 0);

    IZ_UINT nAxisFlag = 0;
    nAxisFlag |= (CDebugMeshUtil::IsNormal(flag) ? E_DEBUG_MESH_AXIS_Z : 0);
    nAxisFlag |= (CDebugMeshUtil::IsTangent(flag) ? E_DEBUG_MESH_AXIS_X | E_DEBUG_MESH_AXIS_Y : 0);

    m_pDebugAxis = CDebugMeshAxis::CreateDebugMeshAxis(
                    m_Allocator,
                    device,
                    nAxisFlag,
                    nVtxNum);
    IZ_ASSERT(m_pDebugAxis != IZ_NULL);

    return (m_pDebugAxis != IZ_NULL);
}

void CDebugMesh::DrawDebugAxis(graph::CGraphicsDevice* device)
{
    if (m_pDebugAxis != IZ_NULL) {
        m_pDebugAxis->Draw(device);
    }
}

IZ_BOOL CDebugMesh::BeginDebugAxisRegister(graph::CGraphicsDevice* device)
{
    if (m_pDebugAxis != IZ_NULL) {
        VRETURN(m_pDebugAxis->BeginRegister(device));
    }
    return IZ_TRUE;
}

IZ_BOOL CDebugMesh::EndDebugAxisRegister(graph::CGraphicsDevice* device)
{
    if (m_pDebugAxis != IZ_NULL) {
        VRETURN(m_pDebugAxis->EndRegister(device));
    }
    return IZ_TRUE;
}

void CDebugMesh::SetDebugAxisVtxData(
    const void* vtx,
    IZ_UINT flag)
{
    const SMeshVtx& sVtx = *(const SMeshVtx*)(vtx);

    if (m_pDebugAxis != IZ_NULL) {
        if (CDebugMeshUtil::IsNormal(flag)) {
            m_pDebugAxis->SetVtx(
                E_DEBUG_MESH_AXIS_Z,
                sVtx.pos,
                sVtx.nml);
        }

        if (CDebugMeshUtil::IsTangent(flag)) {
            // Tangent
            m_pDebugAxis->SetVtx(
                E_DEBUG_MESH_AXIS_X,
                sVtx.pos,
                sVtx.tangent);

            // BiNormal
            m_pDebugAxis->SetVtx(
                E_DEBUG_MESH_AXIS_Y,
                sVtx.pos,
                sVtx.binml);
        }
    }
}
