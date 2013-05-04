#include "graph/ShaderProgram.h"
#include "graph/VertexShader.h"
#include "graph/PixelShader.h"

namespace izanagi
{
namespace graph
{
    CShaderProgram::CShaderProgram()
    {
        m_Allocator = IZ_NULL;
        m_VS = IZ_NULL;
        m_PS = IZ_NULL;
    }

    CShaderProgram::~CShaderProgram()
    {
        SAFE_RELEASE(m_VS);
        SAFE_RELEASE(m_PS);
    }

    CVertexShader* CShaderProgram::GetVertexShader()
    {
        return m_VS;
    }

    CPixelShader* CShaderProgram::GetPixelShader()
    {
        return m_PS;
    }
}   // namespace graph
}   // namespace izanagi