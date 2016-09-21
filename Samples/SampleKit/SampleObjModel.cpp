#include "SampleObjModel.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace izanagi {
namespace sample {
    ObjModel* ObjModel::create(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        const char* path)
    {
        void* buf = ALLOC(allocator, sizeof(ObjModel));
        VRETURN_NULL(buf);

        IZ_BOOL result = IZ_FALSE;

        ObjModel* instance = new(buf)ObjModel;
        {
            instance->m_Allocator = allocator;
            instance->AddRef();

            result = instance->load(
                allocator,
                device,
                path);
        }

        if (!result) {
            SAFE_RELEASE(instance);
        }

        return instance;
    }

    ObjModel::~ObjModel()
    {
        SAFE_RELEASE(m_vb);
        SAFE_RELEASE(m_ib);
        SAFE_RELEASE(m_vd);
    }

    IZ_BOOL ObjModel::load(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        const char* path)
    {
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> mtrls;
        std::string err;

        // TODO
        // mtl_basepath

        IZ_UINT flags = tinyobj::triangulation | tinyobj::calculate_normals;

        auto result = tinyobj::LoadObj(
            shapes, mtrls,
            err,
            path, nullptr,
            flags);
        VRETURN(result);

        IZ_UINT vtxNum = 0;

        // NOTE
        // positon = float3
        for (const auto& shape : shapes) {
            auto num = shape.mesh.positions.size();
            IZ_ASSERT(num % 3 == 0);
            IZ_ASSERT(num == shape.mesh.normals.size());

            vtxNum += num;
        }

        vtxNum /= 3;

        struct Vertex {
            IZ_FLOAT pos[3];
            IZ_FLOAT nml[3];
        };

        m_vb = device->CreateVertexBuffer(
            sizeof(Vertex),
            vtxNum,
            izanagi::graph::E_GRAPH_RSC_USAGE_STATIC);
        {
            Vertex* data = nullptr;

            result = m_vb->Lock(device, 0, 0, (void**)&data, IZ_FALSE);
            VRETURN(result);

            auto top = data;

            for (const auto& shape : shapes) {
                auto num = shape.mesh.positions.size();

                for (IZ_UINT i = 0; i < num; i += 3) {
                    Vertex& v = *data++;

                    v.pos[0] = shape.mesh.positions[i + 0];
                    v.pos[1] = shape.mesh.positions[i + 1];
                    v.pos[2] = shape.mesh.positions[i + 2];

                    v.nml[0] = shape.mesh.normals[i + 0];
                    v.nml[1] = shape.mesh.normals[i + 1];
                    v.nml[2] = shape.mesh.normals[i + 2];
                }
            }

            IZ_ASSERT(izanagi::CStdUtil::GetPtrDistance(top, data) == sizeof(Vertex) * vtxNum);

            m_vb->Unlock(device);
        }

        IZ_UINT idxNum = 0;

        for (const auto& s : shapes) {
            auto num = s.mesh.indices.size();
            IZ_ASSERT(num % 3 == 0);

            idxNum += num;
        }

        m_ib = device->CreateIndexBuffer(
            idxNum,
            izanagi::graph::E_GRAPH_INDEX_BUFFER_FMT_INDEX32,
            izanagi::graph::E_GRAPH_RSC_USAGE_STATIC);
        {
            IZ_INT* data = nullptr;
            result = m_ib->Lock(device, 0, 0, (void**)&data, IZ_FALSE);
            VRETURN(result);

            auto top = data;

            IZ_INT idxOffset = 0;

            for (const auto& s : shapes) {
                auto num = s.mesh.indices.size();

                for (IZ_UINT i = 0; i < num; i++) {
                    *data++ = s.mesh.indices[i] + idxOffset;
                }

                auto vtxnum = s.mesh.positions.size() / 3;

                idxOffset += vtxnum;
            }

            IZ_ASSERT(izanagi::CStdUtil::GetPtrDistance(top, data) == sizeof(IZ_INT) * idxNum);

            m_ib->Unlock(device);
        }

        izanagi::graph::SVertexElement elems[] = {
            { 0, 0,  izanagi::graph::E_GRAPH_VTX_DECL_TYPE_FLOAT3, izanagi::graph::E_GRAPH_VTX_DECL_USAGE_POSITION, 0, },
            { 0, 12, izanagi::graph::E_GRAPH_VTX_DECL_TYPE_FLOAT3, izanagi::graph::E_GRAPH_VTX_DECL_USAGE_NORMAL, 0, },
        };

        m_vd = device->CreateVertexDeclaration(elems, COUNTOF(elems));

        return result;
    }

    void ObjModel::render(izanagi::graph::CGraphicsDevice* device)
    {
        device->SetVertexDeclaration(m_vd);
        device->SetIndexBuffer(m_ib);

        auto stride = m_vb->GetStride();
        device->SetVertexBuffer(0, 0, stride, m_vb);

        auto vtxNum = m_vb->GetVtxNum();
        auto idxNum = m_ib->GetIdxNum();
        auto primNum = idxNum / 3;

        device->DrawIndexedPrimitive(
            izanagi::graph::E_GRAPH_PRIM_TYPE_TRIANGLELIST,
            0,
            vtxNum,
            0,
            primNum);
    }
}
}
