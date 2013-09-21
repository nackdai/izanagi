#if !defined(__IZANAGI_GRAPH_VERTEX_DECLARATION_H__)
#define __IZANAGI_GRAPH_VERTEX_DECLARATION_H__

#include "izDefs.h"
#include "izStd.h"
#include "graph/GraphDefs.h"

namespace izanagi
{
namespace graph
{
    /**
     */
    struct SVertexElement
    {
        IZ_WORD Stream;                 ///< Stream index
        IZ_WORD Offset;                 ///< Offset in the stream in bytes
        E_GRAPH_VTX_DECL_TYPE Type;     ///< Data type
        E_GRAPH_VTX_DECL_USAGE Usage;   ///< Semantics
        IZ_BYTE UsageIndex;             ///< Semantic index
        IZ_BYTE padding[3];
    };

    /** 頂点宣言
     */
    class CVertexDeclaration : public CObject
    {
    protected:
        CVertexDeclaration()
        {
            m_Allocator = IZ_NULL;

            m_ElemNum = 0;
            m_Elements = IZ_NULL;
        }

        virtual ~CVertexDeclaration() {};

        NO_COPIABLE(CVertexDeclaration);

        IZ_DEFINE_INTERNAL_RELEASE();

    public:
        IZ_UINT GetElementNum() const { return m_ElemNum; }

        const SVertexElement* GetElement() const { return m_Elements; }

    protected:
        IMemoryAllocator* m_Allocator;

        IZ_UINT m_ElemNum;
        SVertexElement* m_Elements;
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_VERTEX_DECLARATION_H__)
