#include "graph/gles2/ShaderProgram_GLES2.h"
#include "graph/gles2/VertexShader_GLES2.h"
#include "graph/gles2/PixelShader_GLES2.h"

namespace izanagi
{
namespace graph
{
    CShaderProgram* CShaderProgramGLES2::CreateShaderProgram(IMemoryAllocator* allocator)
    {
        void* buf = ALLOC(allocator, sizeof(CShaderProgramGLES2));
        VRETURN(buf != IZ_NULL);

        CShaderProgramGLES2* instance = new(buf) CShaderProgramGLES2;
        {
            instance->AddRef();
            instance->m_Allocator = allocator;

            instance->m_Program = ::glCreateProgram();
            IZ_ASSERT(instance->m_Program != 0);
        }

        if (instance->m_Program == 0) {
            SAFE_RELEASE(instance);
        }

        return instance;
    }

    CShaderProgramGLES2::CShaderProgramGLES2()
    {
        m_Program = 0;
        m_IsDirty = IZ_FALSE;
        m_IsLinked = IZ_TRUE;
    }

    IZ_BOOL CShaderProgramGLES2::AttachVertexShader(CVertexShader* vs)
    {
        IZ_ASSERT(m_Program != 0);

        if (m_VS != vs) {
            if (vs == IZ_NULL) {
                if (m_VS != IZ_NULL) {
                    ::glDetachShader(
                        m_Program,
                        VertexShader()->GetRawInterface());
                }
            }
            else {
                ::glAttachShader(
                    m_Program,
                    ((CVertexShaderGLES2*)vs)->GetRawInterface());
            }

            SAFE_REPLACE(m_VS, vs);

            m_IsDirty = IZ_TRUE;
            m_IsLinked = IZ_FALSE;
        }
        
        return IZ_TRUE;
    }

    IZ_BOOL CShaderProgramGLES2::AttachPixelShader(CPixelShader* ps)
    {
        IZ_ASSERT(m_Program != 0);

        if (m_PS != ps) {
            if (ps == IZ_NULL) {
                if (m_PS != IZ_NULL) {
                    ::glDetachShader(
                        m_Program,
                        PixelShader()->GetRawInterface());
                }
            }
            else {
                ::glAttachShader(
                    m_Program,
                    ((CPixelShaderGLES2*)ps)->GetRawInterface());
            }

            SAFE_REPLACE(m_PS, ps);

            m_IsDirty = IZ_TRUE;
            m_IsLinked = IZ_FALSE;
        }
        
        return IZ_TRUE;
    }

    IZ_BOOL CShaderProgramGLES2::Link()
    {
#if 0
        if (m_IsLinked) {
            return IZ_TRUE;
        }
#endif

        IZ_ASSERT(m_Program != 0);
        IZ_ASSERT(IsValid());

        if (IsValid() && IsDirty()) {
            ::glLinkProgram(m_Program);

            // TODO
            // リンク結果をチェック
            GLint isLinked = 0;

            ::glGetProgramiv(m_Program, GL_LINK_STATUS, &isLinked);

            if (isLinked) {
                m_IsLinked = IZ_TRUE;
            }
#ifdef __IZ_DEBUG__
            else {
                GLint infoLen = 0;

                ::glGetProgramiv(m_Program, GL_INFO_LOG_LENGTH, &infoLen);

                if (infoLen > 1) {
                    char* log = (char*)ALLOC_ZERO(m_Allocator, infoLen);

                    ::glGetProgramInfoLog(m_Program, infoLen, NULL, log);
                    IZ_PRINTF("ShaderProgram Link Log : [%s]\n", log);

                    FREE(m_Allocator, log);
                }
            }
#endif  // #ifdef __IZ_DEBUG__
        }

        return IZ_TRUE;
    }

    IZ_BOOL CShaderProgramGLES2::IsValid()
    {
        return (m_VS != IZ_NULL && m_PS != IZ_NULL);
    }

    IZ_BOOL CShaderProgramGLES2::IsDirty()
    {
        return m_IsDirty;
    }

    void CShaderProgramGLES2::ClearDirty()
    {
        m_IsDirty = IZ_FALSE;
    }

    IZ_BOOL CShaderProgramGLES2::IsLinked()
    {
        return m_IsLinked;
    }

    CVertexShaderGLES2* CShaderProgramGLES2::VertexShader()
    {
        return (CVertexShaderGLES2*)m_VS;
    }

    CPixelShaderGLES2* CShaderProgramGLES2::PixelShader()
    {
        return (CPixelShaderGLES2*)m_PS;
    }

    SHADER_PARAM_HANDLE CShaderProgramGLES2::GetHandleByName(IZ_PCSTR name)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(IsLinked());

        SHADER_PARAM_HANDLE ret = ::glGetUniformLocation(
            m_Program,
            name);
        IZ_ASSERT(ret >= 0);

        return ret;
    }

    IZ_UINT CShaderProgramGLES2::GetSamplerIndex(const SHADER_PARAM_HANDLE& handle)
    {
        // TODO
        IZ_ASSERT(IZ_FALSE);
        return 0;
    }

    IZ_BOOL CShaderProgramGLES2::SetBool(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, IZ_BOOL b)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(IsLinked());

        ::glUniform1i(handle, b);
        return IZ_TRUE;
    }

    IZ_BOOL CShaderProgramGLES2::SetBoolArray(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const IZ_BOOL* b, IZ_UINT num)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(IsLinked());

        ::glUniform1iv(handle, num, b);
        return IZ_TRUE;
    }
    
    IZ_BOOL CShaderProgramGLES2::SetFloat(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, IZ_FLOAT f)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(IsLinked());

        ::glUniform1f(handle, f);
        return IZ_TRUE;
    }

    IZ_BOOL CShaderProgramGLES2::SetFloatArray(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const IZ_FLOAT* f, IZ_UINT num)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(IsLinked());

        ::glUniform1fv(handle, num, f);
        return IZ_TRUE;
    }
    
    IZ_BOOL CShaderProgramGLES2::SetInt(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, IZ_INT n)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(IsLinked());

        ::glUniform1i(handle, n);
        return IZ_TRUE;
    }

    IZ_BOOL CShaderProgramGLES2::SetIntArray(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const IZ_INT* n, IZ_UINT num)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(IsLinked());

        ::glUniform1iv(handle, num, n);
        return IZ_TRUE;
    }
    
    IZ_BOOL CShaderProgramGLES2::SetMatrix(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const math::SMatrix& m)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(IsLinked());

        ::glUniformMatrix4fv(
            handle,
            1,
            GL_FALSE,
            m.a);
        return IZ_TRUE;
    }

    IZ_BOOL CShaderProgramGLES2::SetMatrixArray(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const math::SMatrix* m, IZ_UINT num)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(IsLinked());

        ::glUniformMatrix4fv(
            handle,
            num,
            GL_FALSE,
            (const GLfloat*)m);
        return IZ_TRUE;
    }
    
    IZ_BOOL CShaderProgramGLES2::SetVector(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const math::SVector& v)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(IsLinked());

        ::glUniform4f(
            handle,
            v.x, v.y, v.z, v.w);
        return IZ_TRUE;
    }

    IZ_BOOL CShaderProgramGLES2::SetVectorArray(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const math::SVector* v, IZ_UINT num)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(IsLinked());

        ::glUniform4fv(handle, num, (const GLfloat*)v);
        return IZ_TRUE;
    }
    
    IZ_BOOL CShaderProgramGLES2::SetValue(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const void* p, IZ_UINT size)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(IsLinked());

        // TODO
        IZ_ASSERT(IZ_FALSE);
        return IZ_TRUE;
    }
}   // namespace graph
}   // namespace izanagi