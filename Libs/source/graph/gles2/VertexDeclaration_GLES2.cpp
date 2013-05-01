#include "graph/gles2/VertexDeclaration_GLES2.h"
#include "graph/internal/ParamValueConverter.h"

namespace izanagi
{
namespace graph
{
    // インスタンス作成
    CVertexDeclaration* CVertexDeclarationGLES2::CreateVertexDeclaration(
        IMemoryAllocator* allocator,
        const SVertexElement* elem,
        IZ_UINT elemNum)
    {
        size_t size = sizeof(CVertexDeclarationGLES2);
        size += sizeof(SVertexElement) * elemNum;

        IZ_UINT8* buf = (IZ_UINT8*)ALLOC(allocator, size);
        VRETURN_NULL(buf != IZ_NULL);

        CVertexDeclarationGLES2* instance = new (buf) CVertexDeclarationGLES2;
        {
            instance->AddRef();

            instance->m_Allocator = allocator;

            buf += sizeof(CVertexDeclarationGLES2);

            instance->m_Elements = reinterpret_cast<SVertexElement*>(buf);
            memcpy(
                instance->m_Elements,
                elem,
                sizeof(SVertexElement) * elemNum);

            instance->m_ElemNum = elemNum;
        }

        return instance;
    }

    // コンストラクタ
    CVertexDeclarationGLES2::CVertexDeclarationGLES2()
    {
        m_Allocator = IZ_NULL;
        m_ElemNum = 0;
        m_Elements = IZ_NULL;
    }

    // デストラクタ
    CVertexDeclarationGLES2::~CVertexDeclarationGLES2()
    {
        m_ElemNum = 0;
        FREE(m_Allocator, m_Elements);
    }

    IZ_UINT CVertexDeclarationGLES2::GetElemNum() const
    {
        return m_ElemNum;
    }

    const SVertexElement* CVertexDeclarationGLES2::GetElements() const
    {
        return m_Elements;
    }
}   // namespace graph
}   // namespace izanagi