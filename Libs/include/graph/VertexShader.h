#if !defined(__IZANAGI_GRAPH_VERTEX_SHADER_H__)
#define __IZANAGI_GRAPH_VERTEX_SHADER_H__

#include "ShaderBase.h"

namespace izanagi
{
namespace graph
{
	/** 頂点シェーダ
	 */
	class CVertexShader : public IShader {
	protected:
        CVertexShader() {}
        virtual ~CVertexShader() {}
	};
}   // namespace graph
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_GRAPH_VERTEX_SHADER_H__)
