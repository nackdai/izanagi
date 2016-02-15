#include "graph/ogl/VertexDeclaration_OGL.h"
#include "graph/gles2/ShaderProgram_GLES2.h"

// NOTE
// http://blog.techlab-xe.net/archives/2870

namespace izanagi
{
namespace graph
{
    // インスタンス作成
    CVertexDeclaration* CVertexDeclarationOGL::CreateVertexDeclaration(
        IMemoryAllocator* allocator,
        const SVertexElement* elem,
        IZ_UINT elemNum)
    {
        size_t size = sizeof(CVertexDeclarationOGL);
        size += sizeof(SVertexElement) * elemNum;

        IZ_UINT8* buf = (IZ_UINT8*)ALLOC(allocator, size);
        VRETURN_NULL(buf != IZ_NULL);

        CVertexDeclarationOGL* instance = new (buf)CVertexDeclarationOGL;
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

    IZ_BOOL CVertexDeclarationOGL::ApplyForInstancing(
        CShaderProgramGLES2* program,
        IZ_UINT vtxOffset,
        IZ_UINT vtxStride,
        IZ_UINT* divisors)
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

                // For instancing.
                auto divisor = divisors[element.Stream];
                CALL_GL_API(::glVertexAttribDivisor(attribIndex, divisor));

                s_EnabledAttribIndex[attribIndex] = Enabled;
            }
        }

        for (IZ_UINT i = 0; i < COUNTOF(s_EnabledAttribIndex); i++) {
            if (s_EnabledAttribIndex[i] == Disabled) {
                CALL_GL_API(::glVertexAttribDivisor(i, 0));
                CALL_GL_API(::glDisableVertexAttribArray(i));

                s_EnabledAttribIndex[i] = None;
            }
        }

        return IZ_TRUE;
    }
}   // namespace graph
}   // namespace izanagi
