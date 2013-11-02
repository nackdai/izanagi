#include "debugutil/mesh/DebugMeshCylinder.h"

using namespace izanagi;

// インスタンス作成
CDebugMeshCylinder* CDebugMeshCylinder::CreateDebugMeshCylinder(
    IMemoryAllocator* pAllocator,
    graph::CGraphicsDevice* pDevice,
    IZ_UINT flag,
    IZ_COLOR nColor,
    IZ_FLOAT fRadius,
    IZ_FLOAT fHeight,
    IZ_UINT nSlices,
    IZ_UINT nStacks)
{
    flag |= E_DEBUG_MESH_VTX_FORM_POS;

    nSlices = IZ_MAX(3, nSlices);
    nStacks = IZ_MAX(2, nStacks + 1);

    void* pBuf = ALLOC_ZERO(pAllocator, sizeof(CDebugMeshCylinder));
    VRETURN_VAL(pBuf != IZ_NULL, IZ_NULL);

    CDebugMeshCylinder* pInstance = IZ_NULL;
    IZ_BOOL result = IZ_FALSE;

    // インスタンス作成
    pInstance = CreateMesh<CDebugMeshCylinder>(pBuf, pAllocator, pDevice, flag);
    VGOTO(result != (pInstance != IZ_NULL), __EXIT__);

    // 初期化
    result = pInstance->Init(
                pDevice,
                flag,
                nSlices,
                nStacks);
    VGOTO(result, __EXIT__);

    // 頂点データセット
    result = pInstance->SetVtx(
                flag,
                nColor,
                fRadius,
                fHeight,
                nSlices,
                nStacks);
    VGOTO(result, __EXIT__);

    // インデックスデータセット
    result = pInstance->SetIdx(
                nSlices,
                nStacks);
    VGOTO(result, __EXIT__);

    // データをVB、IBにコピーする
    result = pInstance->CopyDataToBuffer(flag);
    VGOTO(result, __EXIT__);

__EXIT__:
    if (!result) {
        SAFE_RELEASE(pInstance);
    }
    return pInstance;
}

namespace {
    // 頂点数計算
    inline IZ_UINT _ComputeVtxNum(IZ_UINT nSlices, IZ_UINT nStacks)
    {
        // +nSlices * 2 -> 上端、下端の２点
        IZ_UINT ret = (nSlices * 2) + (nSlices + 1) * nStacks;
        return ret;
    }

    // インデックス数計算
    inline IZ_UINT _ComputeIdxNum(IZ_UINT nSlices, IZ_UINT nStacks)
    {
        // 上下
        IZ_UINT ret = (nSlices * 3) * 2;

        // 真ん中
        ret += (nSlices * 3 * 2) * (nStacks - 1);

        return ret;
    }
}   // namespace

// 初期化
IZ_BOOL CDebugMeshCylinder::Init(
    graph::CGraphicsDevice* device,
    IZ_UINT flag,
    IZ_UINT nSlices,
    IZ_UINT nStacks)
{
    IZ_UINT nVtxNum = _ComputeVtxNum(nSlices, nStacks);
    IZ_UINT nIdxNum = _ComputeIdxNum(nSlices, nStacks);

#if 0
    graph::E_GRAPH_INDEX_BUFFER_FMT fmt = (nIdxNum > IZ_UINT16_MAX 
                                    ? graph::E_GRAPH_INDEX_BUFFER_FMT_INDEX32 
                                    : graph::E_GRAPH_INDEX_BUFFER_FMT_INDEX16);
#else
    graph::E_GRAPH_INDEX_BUFFER_FMT fmt = graph::E_GRAPH_INDEX_BUFFER_FMT_INDEX32;
#endif

    VRETURN(
        CreateVB(
            device,
            flag, 
            nVtxNum));

    VRETURN(
        CreateIB(
            device,
            nIdxNum, 
            fmt));

    VRETURN(CreateVD(device, flag));

    m_PrimType = graph::E_GRAPH_PRIM_TYPE_TRIANGLELIST;
    m_nPrimCnt = nIdxNum / 3;

    VRETURN(
        CreateDataBuffer(
            nVtxNum,
            nIdxNum));

    VRETURN(
        CreateDebugAxis(
            device,
            nVtxNum,
            flag));

    return IZ_TRUE;
}

// 頂点データセット
IZ_BOOL CDebugMeshCylinder::SetVtx(
    IZ_UINT flag,
    IZ_COLOR nColor,
    IZ_FLOAT fRadius,
    IZ_FLOAT fHeight,
    IZ_UINT nSlices,
    IZ_UINT nStacks)
{
    SMeshVtx* pVtx = GetVtx();

    // NOTE
    // 半分下に下げるのは、円柱の中心が原点にくるようにするため

    // 中心点（上）
    for (IZ_UINT i = 0; i < nSlices; ++i) {
        ComputeVtx(
            pVtx,
            flag,
            nColor,
            0.0f,
            fHeight,
            0.5f * fHeight,
            IZ_MATH_PI2 * i / (nSlices -1));

        if (IsNormal(flag)) {
            pVtx->nml.Set(0.0f, 1.0f, 0.0f, 0.0f);
        }

        ++pVtx;
    }

    IZ_FLOAT fLongitudeStep = IZ_MATH_PI2 / nSlices;    // 経度ステップ
    IZ_FLOAT fHeightStep = fHeight / (nStacks - 1);

    // 真ん中
    for (IZ_UINT stack = 0; stack < nStacks; ++stack) {
        IZ_FLOAT fY = fHeight * 0.5f - fHeightStep * stack;

        for (IZ_UINT slice = 0; slice < nSlices; ++slice) {
            IZ_FLOAT fLongitude = fLongitudeStep * slice;

            ComputeVtx(
                pVtx,
                flag,
                nColor,
                fRadius,
                fHeight,
                fY,
                fLongitude);
            ++pVtx;
        }

#if 1
        // 最後にひとつ重ねる
        {
            IZ_FLOAT fLongitude = 0.0f;

            ComputeVtx(
                pVtx,
                flag & (~E_DEBUG_MESH_VTX_FORM_UV), // UVをセットさせない
                nColor,
                fRadius,
                fHeight,
                fY,
                fLongitude);

            // ここでUV座標をセットする
            if (IsUV(flag)) {
                pVtx->uv[0] = 1.0f;
                pVtx->uv[1] = (fHeight - (fY + fHeight * 0.5f)) / fHeight;
            }

            ++pVtx;
        }
#endif
    }

    // 中心点（下）
    for (IZ_UINT i = 0; i < nSlices; ++i) {
        ComputeVtx(
            pVtx,
            flag,
            nColor,
            0.0f,
            fHeight,
            -0.5f * fHeight,
            IZ_MATH_PI2 * i / (nSlices -1));

        if (IsNormal(flag)) {
            pVtx->nml.Set(0.0f, -1.0f, 0.0f, 0.0f);
        }

        ++pVtx;
    }

    IZ_ASSERT(CStdUtil::GetPtrAdvanced(GetVtx(), pVtx) == GetVtxNum());

    return IZ_TRUE;
}

// 頂点データ計算
void CDebugMeshCylinder::ComputeVtx(
    SMeshVtx* pVtx,
    IZ_UINT flag,
    IZ_COLOR nColor,
    IZ_FLOAT fRadius,
    IZ_FLOAT fHeight,
    IZ_FLOAT fY,
    IZ_FLOAT fLongitude)    // 経度
{
    IZ_FLOAT fSinLong = math::CMath::SinF(fLongitude);
    IZ_FLOAT fCosLong = math::CMath::CosF(fLongitude);

    // 位置
    if (IsPos(flag)) {
#ifdef IZ_COORD_LEFT_HAND
        pVtx->pos.v[0] = fRadius * fCosLong;
        pVtx->pos.v[1] = fY;
        pVtx->pos.v[2] = fRadius * fSinLong;
        pVtx->pos.v[3] = 1.0f;
#else
        pVtx->pos.v[0] = fRadius * fSinLong;
        pVtx->pos.v[1] = fY;
        pVtx->pos.v[2] = fRadius * fCosLong;
        pVtx->pos.v[3] = 1.0f;
#endif
    }

    // 法線
    if (IsNormal(flag)) {
#ifdef IZ_COORD_LEFT_HAND
        pVtx->nml.v[0] = fCosLong;
        pVtx->nml.v[1] = 0.0f;
        pVtx->nml.v[2] = fSinLong;
        pVtx->nml.v[3] = 0.0f;
#else
        pVtx->nml.v[0] = fSinLong;
        pVtx->nml.v[1] = 0.0f;
        pVtx->nml.v[2] = fCosLong;
        pVtx->nml.v[3] = 0.0f;
#endif
    }

    // 頂点カラー
    if (IsColor(flag)) {
        pVtx->clr = nColor;
    }

    // UV座標
    if (IsUV(flag)) {
        pVtx->uv[0] = fLongitude / IZ_MATH_PI2;

        // NOTE
        // 半分下に下がっているため * 0.5f で計算する
        pVtx->uv[1] = (fHeight - (fY + fHeight * 0.5f)) / fHeight;
    }
}

// インデックスデータセット
IZ_BOOL CDebugMeshCylinder::SetIdx(
    IZ_UINT nSlices,
    IZ_UINT nStacks)
{
    // 頂点数
    const IZ_UINT nVtxNum = GetVtxNum();

    SMeshFace* pFace = GetFace();
    VRETURN_NULL(pFace);

    // 上
    for (IZ_UINT slice = 0; slice < nSlices; ++slice) {
        pFace->idx[0] = slice;  // 一番上の点
        pFace->idx[1] = slice + nSlices;
        pFace->idx[2] = slice + nSlices + 1;

        if (slice + 1 >= nSlices) {
            SetOverlapVtx(
                (slice + 1) % nSlices + nSlices,
                slice + 1 + nSlices);
        }
        
        BindFaceToVtx(pFace);
        ++pFace;
    }

    // 真ん中
    // ex) nSlices = 4 の場合
    //  1___2___3___4___1
    //  |  /|  /|  /|  /|
    //  | / | / | / | / |
    //  |/__|/__|/__|/__|
    //  5   6   7   8   5
    //
    //  1-5-2 / 2-5-6
    //  2-6-3 / 3-6-7

    // NOTE
    // + nSlices は頂上(or下)の点の分

    for (IZ_UINT stack = 0; stack < nStacks - 1; ++stack) {
        for (IZ_UINT slice = 0; slice < nSlices; ++slice) {
            {
                pFace->idx[0] = slice + stack * (nSlices + 1) + nSlices;
                pFace->idx[1] = slice + (stack + 1) * (nSlices + 1) + nSlices;
                pFace->idx[2] = (slice + 1) + stack * (nSlices + 1) + nSlices;

                if (slice + 1 >= nSlices) {
                    SetOverlapVtx(
                        (slice + 1) % nSlices + stack * (nSlices + 1) + nSlices,
                        (slice + 1) + stack * (nSlices + 1) + nSlices);
                }

                BindFaceToVtx(pFace);
                ++pFace;
            }

            {
                pFace->idx[0] = (slice + 1) + stack * (nSlices + 1) + nSlices;
                pFace->idx[1] = slice + (stack + 1) * (nSlices + 1) + nSlices;
                pFace->idx[2] = (slice + 1) + (stack + 1) * (nSlices + 1) + nSlices;

                if (slice + 1 >= nSlices) {
                    SetOverlapVtx(
                        (slice + 1) % nSlices + (stack + 1) * (nSlices + 1) + nSlices,
                        (slice + 1) + (stack + 1) * (nSlices + 1) + nSlices);
                }

                BindFaceToVtx(pFace);
                ++pFace;
            }
        }
    }

    // 下
    for (IZ_UINT slice = 0; slice < nSlices; ++slice) {
        pFace->idx[0] = nVtxNum - nSlices + slice;  // 一番下の点
        pFace->idx[1] = nVtxNum - nSlices - ((nSlices + 1) - (slice + 1));
        pFace->idx[2] = nVtxNum - nSlices - ((nSlices + 1) - slice);

        if (slice + 1 >= nSlices) {
            SetOverlapVtx(
                nVtxNum - nSlices - ((nSlices + 1) - (slice + 1) % nSlices),
                nVtxNum - nSlices - ((nSlices + 1) - (slice + 1)));
        }

        BindFaceToVtx(pFace);
        ++pFace;
    }

    IZ_ASSERT(CStdUtil::GetPtrAdvanced(GetFace(), pFace) == m_nPrimCnt);

    return IZ_TRUE;
}
