#include "graph/gles2/ShaderProgram_GLES2.h"
#include "graph/gles2/VertexShader_GLES2.h"
#include "graph/gles2/PixelShader_GLES2.h"

namespace izanagi
{
namespace graph
{
    IZ_BOOL CShaderProgram::IsValidHandle(const SHADER_PARAM_HANDLE& handle)
    {
        return (handle >= 0);
    }

    CShaderProgram* CShaderProgramGLES2::CreateShaderProgram(IMemoryAllocator* allocator)
    {
        void* buf = ALLOC(allocator, sizeof(CShaderProgramGLES2));
        VRETURN(buf != IZ_NULL);

        CShaderProgramGLES2* instance = new(buf) CShaderProgramGLES2;
        {
            instance->AddRef();
            instance->m_Allocator = allocator;

            CALL_GL_API(instance->m_Program = ::glCreateProgram());
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

        m_IsLinked = IZ_TRUE;

        m_AttribNum = -1;
        m_Attribs = IZ_NULL;
    }

    CShaderProgramGLES2::~CShaderProgramGLES2()
    {
        ClearAttributeInfo();
    }

    IZ_BOOL CShaderProgramGLES2::AttachVertexShader(CVertexShader* vs)
    {
        IZ_ASSERT(m_Program != 0);
        IZ_ASSERT(vs != IZ_NULL);

        if (m_VS == IZ_NULL) {
            CALL_GL_API(
                ::glAttachShader(
                    m_Program,
                    ((CVertexShaderGLES2*)vs)->GetRawInterface()));

            SAFE_REPLACE(m_VS, vs);

            m_IsLinked = IZ_FALSE;

            if (IsValid()) {
                Link();
            }
        }
        
        return IZ_TRUE;
    }

    IZ_BOOL CShaderProgramGLES2::AttachPixelShader(CPixelShader* ps)
    {
        IZ_ASSERT(m_Program != 0);
        IZ_ASSERT(ps != IZ_NULL);
        
        if (m_PS == IZ_NULL) {
            CALL_GL_API(
                ::glAttachShader(
                    m_Program,
                    ((CPixelShaderGLES2*)ps)->GetRawInterface()));

            SAFE_REPLACE(m_PS, ps);

            m_IsLinked = IZ_FALSE;

            if (IsValid()) {
                Link();
            }
        }
        
        return IZ_TRUE;
    }

    IZ_BOOL CShaderProgramGLES2::Link()
    {
        if (m_IsLinked) {
            return IZ_TRUE;
        }

        IZ_ASSERT(m_Program != 0);
        IZ_ASSERT(IsValid());

        if (IsValid()) {
            CALL_GL_API(::glLinkProgram(m_Program));

            // TODO
            // リンク結果をチェック
            GLint isLinked = 0;

            CALL_GL_API(::glGetProgramiv(m_Program, GL_LINK_STATUS, &isLinked));

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

    void CShaderProgramGLES2::ClearAttributeInfo()
    {
        FREE(m_Allocator, m_Attribs);

        m_Attribs = IZ_NULL;
        m_AttribNum = 0;
    }

    IZ_INT CShaderProgramGLES2::GetAttribIndex(const char* name)
    {
        if (m_AttribNum < 0) {
            m_AttribNum = 0;
            CALL_GL_API(::glGetProgramiv(m_Program, GL_ACTIVE_ATTRIBUTES, &m_AttribNum));
        }

        if (m_AttribNum > 0
            && m_Attribs == IZ_NULL)
        {
            void* buf = ALLOC(m_Allocator, sizeof(SAttribute) * m_AttribNum);
            VRETURN(buf != IZ_NULL);

            m_Attribs = reinterpret_cast<SAttribute*>(buf);

            for (GLint i = 0; i < m_AttribNum; i++) {
                m_Attribs[i].index = -1;
                m_Attribs[i].nameLength = 0;
            }
        }

        for (GLint i = 0; i < m_AttribNum; i++) {
            if (m_Attribs[i].index >= 0) {
                if (::strcmp(m_Attribs[i].name, name) == 0) {
                    return m_Attribs[i].index;
                }
            }
            else {
                IZ_ASSERT(::strlen(name) < COUNTOF(m_Attribs[i].name) - 1);

                m_Attribs[i].nameLength = ::strlen(name);
                ::strncpy(m_Attribs[i].name, name, m_Attribs[i].nameLength);
                m_Attribs[i].name[m_Attribs[i].nameLength] = 0;

                CALL_GL_API(m_Attribs[i].index = ::glGetAttribLocation(m_Program, name));

                return m_Attribs[i].index;
            }
        }

        return -1;
    }

    IZ_BOOL CShaderProgramGLES2::IsValid()
    {
        return (m_VS != IZ_NULL && m_PS != IZ_NULL);
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

        CALL_GL_API(
            SHADER_PARAM_HANDLE ret = ::glGetUniformLocation(
                m_Program,
                name));
        //IZ_ASSERT(ret >= 0);

        return ret;
    }

    IZ_BOOL CShaderProgramGLES2::SetBool(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, IZ_BOOL b)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(IsLinked());

        CALL_GL_API(::glUniform1i(handle, b));
        return IZ_TRUE;
    }

    IZ_BOOL CShaderProgramGLES2::SetBoolArray(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const IZ_BOOL* b, IZ_UINT num)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(IsLinked());

        CALL_GL_API(::glUniform1iv(handle, num, b));
        return IZ_TRUE;
    }
    
    IZ_BOOL CShaderProgramGLES2::SetFloat(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, IZ_FLOAT f)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(IsLinked());

        CALL_GL_API(::glUniform1f(handle, f));
        return IZ_TRUE;
    }

    IZ_BOOL CShaderProgramGLES2::SetFloatArray(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const IZ_FLOAT* f, IZ_UINT num)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(IsLinked());

        CALL_GL_API(::glUniform1fv(handle, num, f));
        return IZ_TRUE;
    }
    
    IZ_BOOL CShaderProgramGLES2::SetInt(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, IZ_INT n)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(IsLinked());

        CALL_GL_API(::glUniform1i(handle, n));
        return IZ_TRUE;
    }

    IZ_BOOL CShaderProgramGLES2::SetIntArray(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const IZ_INT* n, IZ_UINT num)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(IsLinked());

        CALL_GL_API(::glUniform1iv(handle, num, n));
        return IZ_TRUE;
    }
    
    IZ_BOOL CShaderProgramGLES2::SetMatrix(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const math::SMatrix44& m)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(IsLinked());

        CALL_GL_API(
            ::glUniformMatrix4fv(
                handle,
                1,
                GL_FALSE,
                m.a));

        return IZ_TRUE;
    }

    IZ_BOOL CShaderProgramGLES2::SetMatrixArray(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const math::SMatrix44* m, IZ_UINT num)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(IsLinked());

        CALL_GL_API(
            ::glUniformMatrix4fv(
                handle,
                num,
                GL_FALSE,
                (const GLfloat*)m));

        return IZ_TRUE;
    }
    
    IZ_BOOL CShaderProgramGLES2::SetVector(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const math::SVector4& v)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(IsLinked());

        CALL_GL_API(
            ::glUniform4f(
                handle,
                v.x, v.y, v.z, v.w));

        return IZ_TRUE;
    }

    IZ_BOOL CShaderProgramGLES2::SetVectorArray(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const math::SVector4* v, IZ_UINT num)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(IsLinked());

        CALL_GL_API(::glUniform4fv(handle, num, (const GLfloat*)v));
        return IZ_TRUE;
    }
    
    IZ_BOOL CShaderProgramGLES2::SetValue(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const void* p, IZ_UINT size)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(IsLinked());

        // Not implemented
        IZ_ASSERT(IZ_FALSE);
        return IZ_TRUE;
    }

    IZ_BOOL CShaderProgramGLES2::SetMatrixArrayAsVectorArray(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const math::SMatrix44* m, IZ_UINT num)
    {
        // TODO
        // transpose先のメモリをどうするか

        math::SMatrix44* tmp = const_cast<math::SMatrix44*>(m);

        // NOTE
        // 元が Matrix(float4x4) のところを Vector(float4) として扱う
        // そのため、num は Vector として見た場合の個数がくる
        // しかし、以下の処理は Matrix(float4x4) として扱うので
        // 1/4 した数を個数としてみないといけない

        for (IZ_UINT i = 0; i < (num >> 2); i++) {
            const math::SMatrix44& mtx = m[i];
            math::SMatrix44::Transpose(tmp[i], mtx);
        }

        math::SVector4* buf = reinterpret_cast<math::SVector4*>(tmp);

        return SetVectorArray(device, handle, buf, num);
    }
}   // namespace graph
}   // namespace izanagi