#include "ItemBoard.h"

ItemBoard* ItemBoard::Create(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    IZ_UINT flag,
    IZ_COLOR color,
    IZ_FLOAT width,
    IZ_FLOAT height)
{
    static const IZ_UINT divideX = 1;
    static const IZ_UINT divideY = 1;

    void* buf = ALLOC_ZERO(allocator, sizeof(ItemBoard));
    VRETURN_VAL(buf != IZ_NULL, IZ_NULL);

    ItemBoard* instance = IZ_NULL;
    IZ_BOOL result = IZ_FALSE;

    // インスタンス作成
    instance = CreateMesh<ItemBoard>(buf, allocator, device, flag);
    VGOTO(result != (instance != IZ_NULL), __EXIT__);

    // 初期化
    result = instance->Init(
                device,
                flag,
                divideX,
                divideY,
                width,
                height);
    VGOTO(result, __EXIT__);

    // 頂点データセット
    result = instance->SetVtx(
                flag,
                color,
                divideX,
                divideY,
                width,
                height);
    VGOTO(result, __EXIT__);

    // インデックスデータセット
    result = instance->SetIdx(
                divideX,
                divideY);
    VGOTO(result, __EXIT__);

    // データをVB、IBにコピーする
    result = instance->CopyDataToBuffer(device, flag);
    VGOTO(result, __EXIT__);

__EXIT__:
    if (!result) {
        SAFE_RELEASE(instance);
    }
    return instance;
}

IZ_BOOL ItemBoard::SetVtx(
    IZ_UINT flag,
    IZ_COLOR nColor,
    IZ_UINT nDivideX,
    IZ_UINT nDivideY,
    IZ_FLOAT fWidth,
    IZ_FLOAT fHeight)
{
    izanagi::SMeshVtx* pVtx = GetVtx();

    IZ_FLOAT fDivX = 1.0f / nDivideX;
    IZ_FLOAT fDivY = 1.0f / nDivideY;

    IZ_FLOAT fWidthPerVtx = fWidth * fDivX;

    IZ_FLOAT fTexUPerVtx = 1.0f * fDivX;
    IZ_FLOAT fTexVPerVtx = 1.0f * fDivY;

#ifdef IZ_COORD_LEFT_HAND
    IZ_FLOAT fZ = 0.5f * fHeight;
    IZ_FLOAT fHeightPerVtx = -fHeight * fDivY;
#else
    IZ_FLOAT fZ = -0.5f * fHeight;
    IZ_FLOAT fHeightPerVtx = fHeight * fDivY;
#endif

    for (IZ_UINT y = 0; y <= nDivideY; ++y) {
        IZ_FLOAT fX = -0.5f * fWidth;
        
        for (IZ_UINT x = 0; x <= nDivideX; ++x) {
            // 位置
            if (izanagi::CDebugMeshUtil::IsPos(flag)) {
                pVtx->pos.v[0] = fX;
                pVtx->pos.v[1] = fZ;
                pVtx->pos.v[2] = 0.0f;
                pVtx->pos.v[3] = 1.0f;
            }

            // 法線
            if (izanagi::CDebugMeshUtil::IsNormal(flag)) {
                pVtx->nml.v[0] = 0.0f;
                pVtx->nml.v[1] = 0.0f;
                pVtx->nml.v[2] = -1.0f;
                pVtx->nml.v[3] = 0.0f;
            }

            // 頂点カラー
            if (izanagi::CDebugMeshUtil::IsColor(flag)) {
                pVtx->clr = nColor;
            }

            // UV座標
            if (izanagi::CDebugMeshUtil::IsUV(flag)) {
                pVtx->uv[0] = fTexUPerVtx * x;
                pVtx->uv[1] = fTexVPerVtx * y;
            }

            ++pVtx;

            fX += fWidthPerVtx;
        }

        fZ += fHeightPerVtx;
    }

    IZ_ASSERT(izanagi::CStdUtil::GetPtrAdvanced(GetVtx(), pVtx) == GetVtxNum());

    return IZ_TRUE;
}
