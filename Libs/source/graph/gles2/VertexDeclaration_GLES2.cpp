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

    IZ_BOOL CVertexDeclarationGLES2::Apply(CGraphicsDeviceGLES2* device
    {
        for (IZ_UINT i = 0; i < m_ElemNum; i++) {
            SVertexElement& element = m_Elements[i];

            // NOTE
            // VBOを使う場合は常に１ストリームのみ
            IZ_ASSERT(element.Stream == 0);

            // NOTE
            // Semanticによる頂点データの位置は無視するm_Elementsの並び順通りに設定する

            IZ_BOOL needNormalized = IZ_FALSE;
            IZ_UINT num = 4;
            GLenum type = GL_BYTE;
            size_t size = 4;

            switch (element.Type)
            {
            case E_GRAPH_VTX_DECL_TYPE_FLOAT1:
                size = sizeof(GLfloat);
                type = GL_FLOAT;
                num = 1;
                break;
            case E_GRAPH_VTX_DECL_TYPE_FLOAT2:
                size = sizeof(GLfloat);
                type = GL_FLOAT;
                num = 2;
                break;
            case E_GRAPH_VTX_DECL_TYPE_FLOAT3:
                size = sizeof(GLfloat);
                type = GL_FLOAT;
                num = 3;
                break;
            case E_GRAPH_VTX_DECL_TYPE_FLOAT4:
                size = sizeof(GLfloat);
                type = GL_FLOAT;
                num = 4;
                break;
            case E_GRAPH_VTX_DECL_TYPE_COLOR:
                size = sizeof(GLbyte);
                type = GL_UNSIGNED_BYTE;
                num = 4;
                needNormalized = IZ_TRUE;
                break;
            case E_GRAPH_VTX_DECL_TYPE_UBYTE4:
                size = sizeof(GLbyte);
                type = GL_UNSIGNED_BYTE;
                num = 4;
                break;
            case E_GRAPH_VTX_DECL_TYPE_SHORT2:
                size = sizeof(GLshort);
                type = GL_SHORT;
                num = 2;
                break;
            case E_GRAPH_VTX_DECL_TYPE_SHORT4:
                size = sizeof(GLshort);
                type = GL_SHORT;
                num = 2;
                break;
            case E_GRAPH_VTX_DECL_TYPE_UBYTE4N:
                size = sizeof(GLbyte);
                type = GL_UNSIGNED_BYTE;
                num = 4;
                needNormalized = IZ_TRUE;
                break;
            case E_GRAPH_VTX_DECL_TYPE_SHORT2N:
                size = sizeof(GLshort);
                type = GL_SHORT;
                num = 2;
                needNormalized = IZ_TRUE;
                break;
            case E_GRAPH_VTX_DECL_TYPE_SHORT4N:
                size = sizeof(GLshort);
                type = GL_SHORT;
                num = 4;
                needNormalized = IZ_TRUE;
                break;
            case E_GRAPH_VTX_DECL_TYPE_USHORT2N:
                size = sizeof(GLshort);
                type = GL_UNSIGNED_SHORT;
                num = 2;
                needNormalized = IZ_TRUE;
                break;
            case E_GRAPH_VTX_DECL_TYPE_USHORT4N:
                size = sizeof(GLshort);
                type = GL_UNSIGNED_SHORT;
                num = 4;
                needNormalized = IZ_TRUE;
                break;
            default:
                IZ_ASSERT(IZ_FALSE);
                continue;
            }

            ::glEnableVertexAttribArray(i);

            ::glVertexAttribPointer(
                i,
                num,
                type,
                needNormalized,
                size * num,
                (void*)element.Offset);

            // TODO
            //::glBindAttribLocation
        }

        return IZ_TRUE;
    }
}   // namespace graph
}   // namespace izanagi