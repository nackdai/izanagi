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
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> mtrls;
        std::string err;

        auto result = tinyobj::LoadObj(
            &attrib,
            &shapes, &mtrls,
            &err,
            path);
        VRETURN(result);

        IZ_UINT vtxNum = attrib.vertices.size();
        IZ_ASSERT(vtxNum % 3 == 0);
        vtxNum /= 3;

        m_vb = device->CreateVertexBuffer(
            sizeof(IZ_FLOAT) * 3,
            vtxNum,
            izanagi::graph::E_GRAPH_RSC_USAGE_STATIC);
        {
            void* data = nullptr;
            result = m_vb->Lock(device, 0, 0, &data, IZ_FALSE);
            VRETURN(result);

            memcpy(data, &attrib.vertices[0], 3 * sizeof(IZ_FLOAT) * vtxNum);

            m_vb->Unlock(device);
        }

        IZ_UINT idxNum = 0;

        for (const auto& s : shapes) {
            idxNum += s.mesh.indices.size();
        }

        m_ib = device->CreateIndexBuffer(
            idxNum,
            izanagi::graph::E_GRAPH_INDEX_BUFFER_FMT_INDEX32,
            izanagi::graph::E_GRAPH_RSC_USAGE_STATIC);
        {
            IZ_INT* data = nullptr;
            result = m_ib->Lock(device, 0, 0, (void**)&data, IZ_FALSE);
            VRETURN(result);

            for (const auto& s : shapes) {
                for (const auto& i : s.mesh.indices) {
                    *data++ = i.vertex_index;
                }
            }

            m_ib->Unlock(device);
        }

        izanagi::graph::SVertexElement elems[] = {
            { 0, 0, izanagi::graph::E_GRAPH_VTX_DECL_TYPE_FLOAT3, izanagi::graph::E_GRAPH_VTX_DECL_USAGE_POSITION, 0, },
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
