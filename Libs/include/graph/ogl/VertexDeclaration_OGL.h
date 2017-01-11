#if !defined(__IZANAGI_GRAPH_VERTEX_DECLARATION_OGL_H__)
#define __IZANAGI_GRAPH_VERTEX_DECLARATION_OGL_H__

#include "graph/gles2/VertexDeclaration_GLES2.h"

#define USE_VAO

namespace izanagi
{
namespace graph
{
    class CGraphicsDeviceGLES2;
    class CShaderProgramGLES2;
    class CVertexBuffer;
    class CVertexBufferOGL;
    class CGraphicsDevice;

    struct InstancingParam
    {
        IZ_UINT divisor{ 0 };
        IZ_UINT offset{ 0 };
        IZ_UINT stride{ 0 };
    };

    // 頂点宣言
    class CVertexDeclarationOGL : public CVertexDeclarationGLES2
    {
        friend class CGraphicsDeviceOGL;

    private:
        // インスタンス作成
        static CVertexDeclaration* CreateVertexDeclaration(
            IMemoryAllocator* allocator,
            const SVertexElement* pElem,
            IZ_UINT nElemNum);

    private:
        CVertexDeclarationOGL();
        virtual ~CVertexDeclarationOGL();

    private:
        IZ_BOOL begin(
            CGraphicsDevice* device,
            CShaderProgramGLES2* program,
            IZ_UINT vtxOffset,
            CVertexBufferOGL* vb,
            IZ_UINT vtxStride);

        IZ_BOOL beginInstancing(
            CShaderProgramGLES2* program,
            InstancingParam* params,
            CVertexBuffer** vbs);

        void end();

        void enableForceUpdate()
        {
            m_isForceUpdate = IZ_TRUE;
        }

    private:
#ifdef USE_VAO
        GLuint m_vao;

        IZ_BOOL m_isApplied{ IZ_FALSE };
#endif

        izanagi::graph::CVertexBuffer* m_vbs[MAX_STREAM_NUM];

        IZ_BOOL m_isForceUpdate{ IZ_FALSE };

        IZ_UINT m_prevVtxOffset{ IZ_UINT32_MAX };
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_VERTEX_DECLARATION_OGL_H__)
