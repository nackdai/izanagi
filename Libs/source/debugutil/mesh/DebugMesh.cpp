#include "debugutil/mesh/DebugMesh.h"
#include "math/MathVector4.h"

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
