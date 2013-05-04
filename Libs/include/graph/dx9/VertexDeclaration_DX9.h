#ifdef __IZ_DX9__
#if !defined(__IZANAGI_GRAPH_VERTEX_DECLARATION_DX9_H__)
#define __IZANAGI_GRAPH_VERTEX_DECLARATION_DX9_H__

#include "izDX9Defs.h"
#include "graph/VertexDeclaration.h"

namespace izanagi
{
namespace graph
{
    class CGraphicsDeviceDX9;

    // 頂点宣言
    class CVertexDeclarationDX9 : public CVertexDeclaration
    {
        friend class CGraphicsDeviceDX9;

    private:
        // インスタンス作成
        static CVertexDeclaration* CreateVertexDeclaration(
            CGraphicsDeviceDX9* device,
            IMemoryAllocator* allocator,
            const D3D_VTX_ELEMENT* pElem);

        // インスタンス作成
        static CVertexDeclaration* CreateVertexDeclaration(
            CGraphicsDeviceDX9* device,
            IMemoryAllocator* allocator,
            const SVertexElement* pElem,
            IZ_UINT nElemNum);

    private:
        inline CVertexDeclarationDX9();
        virtual inline ~CVertexDeclarationDX9();

    public:
        D3D_VD* GetRawInterface()
        {
            return m_VD;
        }

    protected:
        // 本体
        D3D_VD* m_VD;
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_VERTEX_DECLARATION_DX9_H__)
#endif  // #ifdef __IZ_DX9__
