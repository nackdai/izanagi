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

CDebugMeshUtil::SetVtxFunc CDebugMeshUtil::SetVtxFuncTbl[E_DEBUG_MESH_VTX_FORM_NUM] = {
    &CDebugMeshUtil::SetVtxPos,
    &CDebugMeshUtil::SetVtxNormal,
    &CDebugMeshUtil::SetVtxColor,
    &CDebugMeshUtil::SetVtxUV,
    &CDebugMeshUtil::SetVtxTangent,
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

// 位置
IZ_UINT8* CDebugMeshUtil::SetVtxPos(IZ_UINT flag, const SMeshVtx& sVtx, IZ_UINT8* pVtx)
{
    if (IsPos(flag)) {
        IZ_FLOAT* pos = (IZ_FLOAT*)pVtx;
        pos[0] = sVtx.pos.v[0];
        pos[1] = sVtx.pos.v[1];
        pos[2] = sVtx.pos.v[2];
        pos[3] = 1.0f;

        pVtx += GetPosSize(flag);
    }
    return pVtx;
}

// 法線
IZ_UINT8* CDebugMeshUtil::SetVtxNormal(IZ_UINT flag, const SMeshVtx& sVtx, IZ_UINT8* pVtx)
{
    if (IsNormal(flag)) {
        IZ_FLOAT* nml = (IZ_FLOAT*)pVtx;
        nml[0] = sVtx.nml.v[0];
        nml[1] = sVtx.nml.v[1];
        nml[2] = sVtx.nml.v[2];

        pVtx += GetNormalSize(flag);
    }
    return pVtx;
}

// 頂点カラー
IZ_UINT8* CDebugMeshUtil::SetVtxColor(IZ_UINT flag, const SMeshVtx& sVtx, IZ_UINT8* pVtx)
{
    if (IsColor(flag)) {
        *(IZ_COLOR*)pVtx = sVtx.clr;
        pVtx += GetColorSize(flag);
    }
    return pVtx;
}

// UV座標
IZ_UINT8* CDebugMeshUtil::SetVtxUV(IZ_UINT flag, const SMeshVtx& sVtx, IZ_UINT8* pVtx)
{
    if (IsUV(flag)) {
        IZ_FLOAT* uv = (IZ_FLOAT*)pVtx;
        uv[0] = sVtx.uv[0];
        uv[1] = sVtx.uv[1];

        pVtx += GetUVSize(flag);
    }
    return pVtx;
}

// 接ベクトル
IZ_UINT8* CDebugMeshUtil::SetVtxTangent(IZ_UINT flag, const SMeshVtx& sVtx, IZ_UINT8* pVtx)
{
    if (IsTangent(flag)) {
        IZ_FLOAT* tangent = (IZ_FLOAT*)pVtx;
        tangent[0] = sVtx.tangent.v[0];
        tangent[1] = sVtx.tangent.v[1];
        tangent[2] = sVtx.tangent.v[2];
        tangent[3] = sVtx.tangent.v[3];

        pVtx += GetTangentSize(flag);
    }
    return pVtx;
}
