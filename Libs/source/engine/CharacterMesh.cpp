#include "engine/CharacterMesh.h"

namespace izanagi {
namespace engine {
    Mesh* Mesh::create(IMemoryAllocator* allocator)
    {
        auto buf = ALLOC(allocator, sizeof(Mesh));
        VRETURN_NULL(buf);

        Mesh* msh = new(buf)Mesh;
        {
            msh->m_Allocator = allocator;
            msh->AddRef();
        }

        return msh;
    }

    Mesh::Mesh()
    {
    }

    Mesh::~Mesh()
    {
        SAFE_RELEASE(m_Mdl);
        SAFE_RELEASE(m_Msh);
        SAFE_RELEASE(m_Skl);
        SAFE_RELEASE(m_renderHandler);

        for (IZ_UINT i = 0; i < m_mtrlNum; i++) {
            SAFE_RELEASE(m_Mtrl[i]);
        }
        FREE(m_Allocator, m_Mtrl);
    }

    IZ_BOOL Mesh::loadModel(
        izanagi::graph::CGraphicsDevice* device,
        const char* mshPath,
        const char* sklPath,
        izanagi::IMshRenderHandler* renderHandler)
    {
        IZ_ASSERT(renderHandler);

        IZ_BOOL result = IZ_FALSE;

        // Mesh.
        {
            izanagi::CFileInputStream in;
            result = in.Open(mshPath);
            IZ_ASSERT(result);

            if (result) {
                m_Msh = izanagi::CMesh::CreateMesh(
                    m_Allocator,
                    device,
                    &in);
            }

            VGOTO(result = (m_Msh != IZ_NULL), __EXIT__);
        }

        // Skelton.
        {
            izanagi::CFileInputStream in;
            result = in.Open(sklPath);
            IZ_ASSERT(result);

            if (result) {
                m_Skl = izanagi::CSkeleton::CreateSkeleton(
                    m_Allocator,
                    &in);
            }

            VGOTO(result = (m_Skl != IZ_NULL), __EXIT__);
        }

        // Model
        {
            m_Mdl = izanagi::CModel::CreateModel(
                m_Allocator,
                m_Msh,
                m_Skl,
                renderHandler);

            VGOTO(result = (m_Mdl != IZ_NULL), __EXIT__);

            SAFE_REPLACE(m_renderHandler, renderHandler);
        }

    __EXIT__:
        if (!result) {
            SAFE_RELEASE(m_Mdl);
            SAFE_RELEASE(m_Msh);
            SAFE_RELEASE(m_Skl);
            SAFE_RELEASE(m_renderHandler);
        }

        return result;
    }

    IZ_BOOL Mesh::loadMaterials(
        izanagi::graph::CGraphicsDevice* device,
        IZ_UINT num,
        const char* paths[])
    {
        IZ_ASSERT(num > 0);

        VRETURN(m_mtrlNum == 0);

        m_Mtrl = (izanagi::CMaterial**)ALLOC_ZERO(
            m_Allocator,
            sizeof(izanagi::CMaterial*) * num);
        VRETURN(m_Mtrl != nullptr);

        IZ_BOOL result = IZ_FALSE;
        izanagi::CFileInputStream in;

        IZ_UINT openendMaterialNum = 0;

        for (IZ_UINT i = 0; i < num; i++) {
            result = in.Open(paths[i]);
            IZ_ASSERT(result);

            if (result) {
                m_Mtrl[i] = izanagi::CMaterial::CreateMaterial(m_Allocator, &in);
                result = (m_Mtrl[i] != IZ_NULL);

                in.Finalize();
            }

            if (!result) {
                break;
            }

            openendMaterialNum++;
        }

        if (result) {
            m_mtrlNum = num;
        }
        else {
            for (IZ_UINT i = 0; i < openendMaterialNum; i++) {
                SAFE_RELEASE(m_Mtrl[i]);
            }
        }

        return result;
    }

    IZ_BOOL Mesh::bindMaterial(
        IZ_UINT idx,
        const CKey& texKey,
        izanagi::graph::CBaseTexture* tex,
        izanagi::shader::CShaderBasic* shd)
    {
        VRETURN(m_mtrlNum > 0);

        IZ_ASSERT(idx < m_mtrlNum);
        IZ_ASSERT(m_Mtrl[idx]);

        // マテリアルに対応するテクスチャを設定.
        auto result = m_Mtrl[idx]->SetTexture(texKey, tex);
        VRETURN(result);

        if (result) {
            // マテリアルに対応するシェーダを設定.
            result = m_Mtrl[idx]->SetShader(shd);
            VRETURN(result);

            if (result) {
                // TODO
                // level...
                
                // メッシュにマテリアルを設定.
                result = m_Mdl->GetMesh()->SetMaterial(0, m_Mtrl[idx]);
                IZ_ASSERT(result);
            }
        }

        return result;
    }

    izanagi::CModel* Mesh::getMdl()
    {
        return m_Mdl;
    }

    IZ_UINT Mesh::getMaterialNum() const
    {
        return m_mtrlNum;
    }

    izanagi::CMaterial* Mesh::getMaterial(IZ_UINT idx)
    {
        IZ_ASSERT(idx < m_mtrlNum);
        return m_Mtrl[idx];
    }

    izanagi::IMshRenderHandler* Mesh::getMshRenderHandler()
    {
        return m_renderHandler;
    }
}   // namespace engine
}   // namespace izanagi
