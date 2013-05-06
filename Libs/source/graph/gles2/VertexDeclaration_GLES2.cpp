#include "graph/gles2/VertexDeclaration_GLES2.h"
#include "graph/gles2/ShaderProgram_GLES2.h"
#include "graph/gles2/GraphicsDevice_GLES2.h"
#include "graph/ParamValueConverter.h"
#include "graph/VertexBuffer.h"

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

    IZ_BOOL CVertexDeclarationGLES2::Apply(
        CGraphicsDeviceGLES2* device,
        IZ_UINT vtxOffset,
        IZ_UINT vtxStride)
    {
        CShaderProgramGLES2* program = (CShaderProgramGLES2*)device->GetShaderProgram();

        for (IZ_UINT i = 0; i < m_ElemNum; i++) {
            const SVertexElement& element = m_Elements[i];

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
                size,
                type,
                needNormalized,
                vtxStride,
                (void*)(vtxOffset + element.Offset));

            static const char* names[] = 
            {
                "position",
                "texcoord_0",
                "texcoord_1",
                "texcoord_2",
                "texcoord_3",
                "texcoord_4",
                "texcoord_5",
                "texcoord_6",
                "texcoord_7",
                "color_0",
                "color_1",
                "color_2",
                "color_3",
                "normal",
                "tangent",
            };

            static const IZ_UINT positionNameIdx = 0;
            static const IZ_UINT texcoordNameIdx = 1;
            static const IZ_UINT colorNameIdx = 9;
            static const IZ_UINT normalNameIdx = 13;
            static const IZ_UINT tangentNameIdx = 14;

            const char* name;

            switch (element.Usage)
            {
            case E_GRAPH_VTX_DECL_USAGE_POSITION:
                name = names[positionNameIdx];
                break;
            case E_GRAPH_VTX_DECL_USAGE_NORMAL:
                name = names[normalNameIdx];
                break;
            case E_GRAPH_VTX_DECL_USAGE_TEXCOORD:
                name = names[texcoordNameIdx + element.UsageIndex];
                break;
            case E_GRAPH_VTX_DECL_USAGE_TANGENT:
                name = names[tangentNameIdx];
                break;
            case E_GRAPH_VTX_DECL_USAGE_COLOR:
                name = names[colorNameIdx + element.UsageIndex];
                break;
            default:
                IZ_ASSERT(IZ_FALSE);
                break;
            }

            // NOTE
            // ShaderCompilerによってSemanticに応じたアトリビュート名が設定されている
            ::glBindAttribLocation(
                program->GetRawInterface(),
                i,
                name);
        }

        return IZ_TRUE;
    }
}   // namespace graph
}   // namespace izanagi