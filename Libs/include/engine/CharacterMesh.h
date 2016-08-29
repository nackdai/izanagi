#if !defined(__IZANAGI_ENGINE_CHARACTER_MESH_H__)
#define __IZANAGI_ENGINE_CHARACTER_MESH_H__

#include "izStd.h"
#include "izSceneGraph.h"

namespace izanagi {
    namespace engine {
        class Mesh : public CObject {
        public:
            static Mesh* create(IMemoryAllocator* allocator);

        private:
            Mesh();
            virtual ~Mesh();

            IZ_DEFINE_INTERNAL_RELEASE();

        public:
            IZ_BOOL loadModel(
                izanagi::graph::CGraphicsDevice* device,
                const char* mshPath,
                const char* sklPath,
                izanagi::IMshRenderHandler* renderHandler);

            IZ_BOOL loadMaterials(
                izanagi::graph::CGraphicsDevice* device,
                IZ_UINT num,
                const char* paths[]);

            IZ_BOOL bindMaterial(
                IZ_UINT idx,
                const CKey& texKey,
                izanagi::graph::CBaseTexture* tex,
                izanagi::shader::CShaderBasic* shd);

            izanagi::CModel* getMdl();

            IZ_UINT getMaterialNum() const;

            izanagi::CMaterial* getMaterial(IZ_UINT idx);

            izanagi::IMshRenderHandler* getMshRenderHandler();

        private:
            izanagi::IMemoryAllocator* m_Allocator{ nullptr };

            izanagi::CModel* m_Mdl{ nullptr };

            izanagi::CMesh* m_Msh{ nullptr };
            izanagi::CSkeleton* m_Skl{ nullptr };

            IZ_UINT m_mtrlNum{ 0 };
            izanagi::CMaterial** m_Mtrl{ nullptr };

            izanagi::IMshRenderHandler* m_renderHandler{ nullptr };
        };

    }   // namespace engine
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_ENGINE_CHARACTER_MESH_H__)
