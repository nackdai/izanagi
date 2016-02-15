#if !defined(__IZANAGI_GRAPH_VERTEX_DECLARATION_OGL_H__)
#define __IZANAGI_GRAPH_VERTEX_DECLARATION_OGL_H__

#include "graph/gles2/VertexDeclaration_GLES2.h"

namespace izanagi
{
namespace graph
{
    class CGraphicsDeviceGLES2;
    class CShaderProgramGLES2;

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
        CVertexDeclarationOGL() {}
        virtual ~CVertexDeclarationOGL() {}

    private:
        IZ_BOOL ApplyForInstancing(
            CShaderProgramGLES2* program,
            IZ_UINT vtxOffset,
            IZ_UINT vtxStride,
            IZ_UINT* divisors);
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_VERTEX_DECLARATION_OGL_H__)
