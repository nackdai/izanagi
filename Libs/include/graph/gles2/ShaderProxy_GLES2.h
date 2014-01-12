#if !defined(__IZANAGI_GRAPH_SHADER_PROXY_GLES2_H__)
#define __IZANAGI_GRAPH_SHADER_PROXY_GLES2_H__

#include "graph/GraphDefs.h"

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

#ifdef __IZ_DEBUG__
            // Error Check
            GLint isCompiled = 0;
            ::glGetShaderiv(m_Shader, GL_COMPILE_STATUS, &isCompiled);

            if (!isCompiled) {
                GLint infoLen = 0;
                ::glGetShaderiv(m_Shader, GL_INFO_LOG_LENGTH, &infoLen);

                if (infoLen > 1) {
                    char* log = (char*)ALLOC_ZERO(m_Allocator, infoLen);
                    ::glGetShaderInfoLog(m_Shader, infoLen, NULL, log);
                    IZ_PRINTF("Shader Compile Log **** \n");
                    IZ_PRINTF("%s", log);
                    FREE(m_Allocator, log);
                }
            }
#endif  // #ifdef __IZ_DEBUG__

            return IZ_TRUE;
        }

    public:
        GLuint GetRawInterface() { return m_Shader; }

    protected:
        GLuint m_Shader;
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_SHADER_PROXY_GLES2_H__)
