#include "graph/gles2/VertexDeclaration_GLES2.h"
#include "graph/gles2/ShaderProgram_GLES2.h"
#include "graph/gles2/GraphicsDevice_GLES2.h"
#include "graph/ParamValueConverter.h"
#include "graph/VertexBuffer.h"

namespace izanagi
{
namespace graph
{
    CVertexDeclarationGLES2::State CVertexDeclarationGLES2::s_EnabledAttribIndex[16] = { CVertexDeclarationGLES2::None };

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
        CShaderProgramGLES2* program,
        IZ_UINT vtxOffset,
        IZ_UINT vtxStride)
    {
        for (IZ_UINT i = 0; i < COUNTOF(s_EnabledAttribIndex); i++) {
            s_EnabledAttribIndex[i] = (s_EnabledAttribIndex[i] == Enabled ? Disabled : s_EnabledAttribIndex[i]);
        }

        for (IZ_UINT i = 0; i < m_ElemNum; i++) {
            const char* attribName = GetAttribName(i);
            IZ_INT attribIndex = program->GetAttribIndex(attribName);

            if (attribIndex >= 0) {
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

                E_GRAPH_VTX_DECL_TYPE elemType = element.Type;
                if (element.Usage == E_GRAPH_VTX_DECL_USAGE_POSITION) {
                    elemType = E_GRAPH_VTX_DECL_TYPE_FLOAT3;
                }

                switch (elemType)
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

                // NOTE
                // glVertexAttribPointer に渡すオフセットはバイト数
                // しかし、ここにわたってくるオフセットは頂点位置のオフセット
                // そのため、バイトオフセットに変換する
                IZ_UINT offset = vtxOffset * vtxStride;

                CALL_GL_API(::glVertexAttribPointer(
                    attribIndex,
                    num,
                    type,
                    needNormalized ? GL_TRUE : GL_FALSE,
                    vtxStride,
                    (void*)(offset + element.Offset)));

                CALL_GL_API(::glEnableVertexAttribArray(attribIndex));
                s_EnabledAttribIndex[attribIndex] = Enabled;
            }
        }

        for (IZ_UINT i = 0; i < COUNTOF(s_EnabledAttribIndex); i++) {
            if (s_EnabledAttribIndex[i] == Disabled) {
                CALL_GL_API(::glDisableVertexAttribArray(i));
                s_EnabledAttribIndex[i] = None;
            }
        }

        return IZ_TRUE;
    }

    const char* CVertexDeclarationGLES2::GetAttribName(IZ_UINT elementIdx)
    {
        IZ_ASSERT(elementIdx < m_ElemNum);

        // NOTE
        // glBindAttribLocation は必ず glLinkProgram よりも前に呼ばれなければいけない

        const SVertexElement& element = m_Elements[elementIdx];

        // NOTE
        // VBOを使う場合は常に１ストリームのみ
        //IZ_ASSERT(element.Stream == 0);

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
            "blendweight",
            "blendindices",
        };

        static const IZ_UINT positionNameIdx = 0;
        static const IZ_UINT texcoordNameIdx = 1;
        static const IZ_UINT colorNameIdx = 9;
        static const IZ_UINT normalNameIdx = 13;
        static const IZ_UINT tangentNameIdx = 14;
        static const IZ_UINT blendWeightNameIdx = 15;
        static const IZ_UINT blendIndicesNameIdx = 16;

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
        case E_GRAPH_VTX_DECL_USAGE_BLENDWEIGHT:
            name = names[blendWeightNameIdx];
            break;
        case E_GRAPH_VTX_DECL_USAGE_BLENDINDICES:
            name = names[blendIndicesNameIdx];
            break;
        default:
            IZ_ASSERT(IZ_FALSE);
            break;
        }

        return name;
    }
}   // namespace graph
}   // namespace izanagi
