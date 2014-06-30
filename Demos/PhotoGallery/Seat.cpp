#include "Seat.h"

// Raduis.
const IZ_FLOAT Seat::InnerRadius = 10.0f;
const IZ_FLOAT Seat::OuterRadius = Seat::InnerRadius + 10.0f;
const IZ_FLOAT Seat::MostOuterRadius = Seat::OuterRadius + 10.0f;

// Height.
const IZ_FLOAT Seat::Height = 5.0f;

Seat::SeatPart* Seat::SeatPart::Create(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    IZ_FLOAT innerRadius, IZ_FLOAT outerRadius,
    IZ_FLOAT height,
    IZ_UINT slices)
{
    void* buf = ALLOC(allocator, sizeof(SeatPart));
    VRETURN_NULL(buf != IZ_NULL);

    IZ_BOOL result = IZ_TRUE;

    SeatPart* instance = new (buf) SeatPart;
    {
        instance->AddRef();
        instance->m_Allocator = allocator;
        result = instance->Init(
            device,
            MeshFlags,
            innerRadius, outerRadius,
            height,
            Slices);
    }

    if (!result) {
        SAFE_RELEASE(instance);
    }
    
    return instance;
}

IZ_BOOL Seat::SeatPart::Init(
    izanagi::graph::CGraphicsDevice* device,
    IZ_UINT flag,
    IZ_FLOAT innerRadius, IZ_FLOAT outerRadius,
    IZ_FLOAT height,
    IZ_UINT slices)
{
    IZ_ASSERT(device != IZ_NULL);

    // Vertex count.
    //  Inner faces = (slices + 1) * 2 [vertices]
    //  Top faces = slices + 1 [vertices] (1 is share with inner faces)
    IZ_UINT vtxNum = (slices + 1) * 3;

    // Index count.
    //  Inner faces = slices * 2 [triangles] = slices * 2 * 3 [indices] (1triangle = 3indices)
    //  Top facess = slices * 2 [triangles] = slices * 2 * 3 [indices] (1triangle = 3indices)
    IZ_UINT idxNum = slices * 2 * 3 * 2;

    // Index format.
    izanagi::graph::E_GRAPH_INDEX_BUFFER_FMT idxFmt = izanagi::graph::E_GRAPH_INDEX_BUFFER_FMT_INDEX32;

    VRETURN(
        CreateVB(
            device,
            flag, 
            vtxNum));

    VRETURN(
        CreateIB(
            device,
            idxNum, 
            idxFmt));

    VRETURN(CreateVD(device, flag));

    // TODO
    m_PrimType = izanagi::graph::E_GRAPH_PRIM_TYPE_TRIANGLELIST;
    m_nPrimCnt = idxNum / 3;

    VRETURN(
        CreateDataBuffer(
            vtxNum,
            idxNum));

    VRETURN(
        SetVtx(
            flag,
            Color,
            innerRadius, outerRadius,
            height,
            slices));

    VRETURN(SetIdx(slices));

    // Copy vertices and indices to buffer.
    VRETURN(CopyDataToBuffer(flag));

    return IZ_TRUE;
}

IZ_BOOL Seat::SeatPart::SetVtx(
    IZ_UINT flag,
    IZ_COLOR color,
    IZ_FLOAT innerRadius, IZ_FLOAT outerRadius,
    IZ_FLOAT height,
    IZ_UINT slices)
{
    SMeshVtx* vtx = GetVtx();

    //IZ_FLOAT longitudeStep = IZ_MATH_PI2 / slices;
    IZ_FLOAT longitudeStep = IZ_MATH_PI / slices;

    // Bottom of inner faces.
    for (IZ_UINT i = 0; i <= slices; i++) {
        IZ_FLOAT longitude = i * longitudeStep;

        ComputeVtx(
            vtx,
            flag,
            color,
            innerRadius,
            0.0f,
            longitude);

        vtx++;
    }

    // Top of inner faces.
    for (IZ_UINT i = 0; i <= slices; i++) {
        IZ_FLOAT longitude = i * longitudeStep;

        ComputeVtx(
            vtx,
            flag,
            color,
            innerRadius,
            height,
            longitude);

        vtx++;
    }

    // Outer of top faces.
    for (IZ_UINT i = 0; i <= slices; i++) {
        IZ_FLOAT longitude = i * longitudeStep;

        ComputeVtx(
            vtx,
            flag,
            color,
            outerRadius,
            height,
            longitude);

        vtx++;
    }

    IZ_ASSERT(izanagi::CStdUtil::GetPtrAdvanced(GetVtx(), vtx) == GetVtxNum());

    return IZ_TRUE;
}

void Seat::SeatPart::ComputeVtx(
    izanagi::CDebugMesh::SMeshVtx* vtx,
    IZ_UINT flag,
    IZ_COLOR color,
    IZ_FLOAT radius,
    IZ_FLOAT y,
    IZ_FLOAT longitude)
{
    IZ_FLOAT sinLong = izanagi::math::CMath::SinF(longitude);
    IZ_FLOAT cosLong = izanagi::math::CMath::CosF(longitude);

    if (IsPos(flag)) {
        vtx->pos.v[0] = radius * cosLong;
        vtx->pos.v[1] = y;
        vtx->pos.v[2] = radius * sinLong;
        vtx->pos.v[3] = 1.0f;
    }

    if (IsNormal(flag)) {
        vtx->nml.v[0] = -cosLong;
        vtx->nml.v[1] = 0.0f;
        vtx->nml.v[2] = -sinLong;
        vtx->nml.v[3] = 0.0f;
    }

    // 頂点カラー
    if (IsColor(flag)) {
        vtx->clr = color;
    }

    // UV座標
    if (IsUV(flag)) {
        // TODO
    }
}

IZ_BOOL Seat::SeatPart::SetIdx(IZ_UINT slices)
{
    const IZ_UINT vtxNum = GetVtxNum();

    SMeshFace* face = GetFace();
    VRETURN(face != IZ_NULL);

    // Inner faces.
    for (IZ_UINT i = 0; i < slices; i++) {
        {
            face->idx[0] = i;
            face->idx[1] = i + slices + 1;
            face->idx[2] = i + slices + 2;

            BindFaceToVtx(face);
            ++face;
        }

        {
            face->idx[0] = i;
            face->idx[1] = i + slices + 2;
            face->idx[2] = i + 1;

            BindFaceToVtx(face);
            ++face;
        }
    }

    // Top faces.
    for (IZ_UINT i = 0; i < slices; i++) {
        {
            face->idx[0] = i + slices + 1;
            face->idx[1] = i + (slices + 1) * 2;
            face->idx[2] = i + (slices + 1) * 2 + 1;

            BindFaceToVtx(face);
            ++face;
        }

        {
            face->idx[0] = i + slices + 1;
            face->idx[1] = i + (slices + 1) * 2 + 1;
            face->idx[2] = i + slices + 2;

            BindFaceToVtx(face);
            ++face;
        }
    }

    IZ_ASSERT(izanagi::CStdUtil::GetPtrAdvanced(GetFace(), face) == m_nPrimCnt);

    return IZ_TRUE;
}

////////////////////////////////////////

Seat* Seat::Create(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device)
{
    void* buf = ALLOC(allocator, sizeof(Seat));
    VRETURN_NULL(buf != IZ_NULL);

    IZ_BOOL result = IZ_TRUE;

    Seat* instance = new (buf) Seat;
    {
        instance->AddRef();
        instance->m_Allocator = allocator;
        result = instance->Init(allocator, device);
    }

    if (!result) {
        SAFE_RELEASE(instance);
    }

    return instance;
}

Seat::Seat()
{
    m_Floor = IZ_NULL;
    m_FlontSeat = IZ_NULL;
    m_RearSeat = IZ_NULL;
}

Seat::~Seat()
{
    SAFE_RELEASE(m_Floor);
    SAFE_RELEASE(m_FlontSeat);
    SAFE_RELEASE(m_RearSeat);
}

IZ_BOOL Seat::Init(
    izanagi::IMemoryAllocator* allocator, 
    izanagi::graph::CGraphicsDevice* device)
{
    IZ_ASSERT(device != IZ_NULL);

    m_FlontSeat = SeatPart::Create(
        allocator,
        device,
        InnerRadius, OuterRadius,
        Height,
        Slices);
    VRETURN(m_FlontSeat != IZ_NULL);

    m_RearSeat = SeatPart::Create(
        allocator,
        device,
        OuterRadius, MostOuterRadius,
        Height * 2.0f,
        Slices);
    VRETURN(m_RearSeat != IZ_NULL);

    return IZ_TRUE;
}

void Seat::Render(izanagi::graph::CGraphicsDevice* device)
{
    IZ_ASSERT(device != IZ_NULL);

    IZ_ASSERT(m_FlontSeat != IZ_NULL);
    m_FlontSeat->Draw(device);

    IZ_ASSERT(m_RearSeat != IZ_NULL);
    m_RearSeat->Draw(device);
}
