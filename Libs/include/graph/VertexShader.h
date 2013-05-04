#if !defined(__IZANAGI_GRAPH_VERTEX_SHADER_H__)
#define __IZANAGI_GRAPH_VERTEX_SHADER_H__

#include "izStd.h"

namespace izanagi
{
namespace graph
{
    /** 頂点シェーダ
     */
    class CVertexShader : public CObject {
    protected:
        CVertexShader() { m_Allocator = IZ_NULL; }
        virtual ~CVertexShader() {}

        NO_COPIABLE(CVertexShader);

        IZ_DEFINE_INTERNAL_RELEASE();

    protected:
        IMemoryAllocator* m_Allocator;
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_VERTEX_SHADER_H__)
