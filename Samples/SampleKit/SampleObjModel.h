#if !defined(__SAMPLE_OBJ_MODEL_H__)
#define __SAMPLE_OBJ_MODEL_H__

#include "izGraph.h"
#include "izSystem.h"

namespace izanagi {
namespace sample {
    class ObjModel : public izanagi::CObject {
    public:
        static ObjModel* create(
            izanagi::IMemoryAllocator* allocator,
            izanagi::graph::CGraphicsDevice* device,
            const char* path);
        
    private:
        ObjModel() {}
        virtual ~ObjModel();

        IZ_DEFINE_INTERNAL_RELEASE();

    public:
        void render(izanagi::graph::CGraphicsDevice* device);

    private:
        IZ_BOOL load(
            izanagi::IMemoryAllocator* allocator,
            izanagi::graph::CGraphicsDevice* device,
            const char* path);

    private:
        izanagi::IMemoryAllocator* m_Allocator{ nullptr };

        izanagi::graph::CVertexBuffer* m_vb{ nullptr };
        izanagi::graph::CIndexBuffer* m_ib{ nullptr };

        izanagi::graph::CVertexDeclaration* m_vd{ nullptr };
    };
}
}

#endif    // #if !defined(__SAMPLE_OBJ_MODEL_H__)