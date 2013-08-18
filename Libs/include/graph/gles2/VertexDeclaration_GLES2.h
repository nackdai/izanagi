#if !defined(__IZANAGI_GRAPH_VERTEX_DECLARATION_GLES2_H__)
#define __IZANAGI_GRAPH_VERTEX_DECLARATION_GLES2_H__

#include "graph/VertexDeclaration.h"

namespace izanagi
{
namespace graph
{
    class CGraphicsDeviceGLES2;
    class CShaderProgramGLES2;

    // 頂点宣言
    class CVertexDeclarationGLES2 : public CVertexDeclaration
    {
        friend class CGraphicsDeviceGLES2;

    private:
        // インスタンス作成
        static CVertexDeclaration* CreateVertexDeclaration(
            IMemoryAllocator* allocator,
            const SVertexElement* pElem,
            IZ_UINT nElemNum);

    private:
        inline CVertexDeclarationGLES2();
        virtual inline ~CVertexDeclarationGLES2();

    private:
        IZ_UINT GetElemNum() const;

        const SVertexElement* GetElements() const;

        IZ_BOOL Apply(
            CShaderProgramGLES2* program,
            IZ_UINT vtxOffset,
            IZ_UINT vtxStride);

        const char* GetAttribName(IZ_UINT elementIdx);

    protected:
        IZ_UINT m_ElemNum;
        SVertexElement* m_Elements;
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_VERTEX_DECLARATION_GLES2_H__)
