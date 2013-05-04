#if !defined(__IZANAGI_GRAPH_SHADER_PROXY_GLES2_H__)
#define __IZANAGI_GRAPH_SHADER_PROXY_GLES2_H__

#include "izGLES2Defs.h"

namespace izanagi
{
namespace graph
{
    // シェーダ定数テーブル
    template <class _SHADER_BASE>
    class CShaderProxy : public _SHADER_BASE {
    protected:
        // コンストラクタ
        CShaderProxy()
        {
            m_Shader = 0;
        }
        virtual ~CShaderProxy()
        {
            if (m_Shader != 0) {
                ::glDeleteShader(m_Shader);
                m_Shader = 0;
            }
        }

    protected:
        IZ_BOOL CreateShader(const void* program, GLenum type)
        {
            m_Shader = ::glCreateShader(type);
            VRETURN(m_Shader != 0);
        
            ::glShaderSource(
                m_Shader,
                1,
                (const char**)&program,
                IZ_NULL);
            
            ::glCompileShader(m_Shader);

            // TODO
            // Error Check

            return IZ_TRUE;
        }

    protected:
        GLuint m_Shader;
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_SHADER_PROXY_GLES2_H__)
