#include "graph/gles2/ShaderProgram_GLES2.h"
#include "graph/gles2/VertexShader_GLES2.h"
#include "graph/gles2/PixelShader_GLES2.h"

#define ENABLE_SET_UNIFORM_IMMEDIATERY  (0)

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

            CALL_GLES2_API(instance->m_Program = ::glCreateProgram());
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

        m_IsCommitChanged = IZ_TRUE;

        m_UniformNum = 0;
        m_Uniforms = IZ_NULL;
    }

    CShaderProgramGLES2::~CShaderProgramGLES2()
    {
        ClearUniformInfo();
    }

    IZ_BOOL CShaderProgramGLES2::AttachVertexShader(CVertexShader* vs)
    {
        IZ_ASSERT(m_Program != 0);

        if (m_VS != vs) {
            if (m_VS != IZ_NULL) {
                CALL_GLES2_API(
                    ::glDetachShader(
                        m_Program,
                        VertexShader()->GetRawInterface()));
            }

            if (vs != IZ_NULL) {
                CALL_GLES2_API(
                    ::glAttachShader(
                        m_Program,
                        ((CVertexShaderGLES2*)vs)->GetRawInterface()));
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
            if (m_PS != IZ_NULL) {
                CALL_GLES2_API(
                    ::glDetachShader(
                        m_Program,
                        PixelShader()->GetRawInterface()));
            }

            if (ps != IZ_NULL) {
                CALL_GLES2_API(
                    ::glAttachShader(
                        m_Program,
                        ((CPixelShaderGLES2*)ps)->GetRawInterface()));
            }

            SAFE_REPLACE(m_PS, ps);

            m_IsDirty = IZ_TRUE;
            m_IsLinked = IZ_FALSE;
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

        if (IsValid() && IsDirty()) {
            CALL_GLES2_API(::glLinkProgram(m_Program));

            // TODO
            // リンク結果をチェック
            GLint isLinked = 0;

            CALL_GLES2_API(::glGetProgramiv(m_Program, GL_LINK_STATUS, &isLinked));

            if (isLinked) {
                m_IsLinked = IZ_TRUE;

                VRETURN(GetUniformInfo());
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

    IZ_BOOL CShaderProgramGLES2::LinkForcibly()
    {
        IZ_ASSERT(m_Program != 0);
        IZ_ASSERT(IsValid());

        if (IsValid()) {
            CALL_GLES2_API(::glLinkProgram(m_Program));

            // TODO
            // リンク結果をチェック
            GLint isLinked = 0;

            CALL_GLES2_API(::glGetProgramiv(m_Program, GL_LINK_STATUS, &isLinked));

            if (isLinked) {
                m_IsLinked = IZ_TRUE;

                if (m_Uniforms == IZ_NULL) {
                    VRETURN(GetUniformInfo());
                }
                else {
                    // NOTE
                    // ShaderProgram を Link すると uniform 変数の location の値は保証されないので
                    // 再度取得しなおす必要があるため、値をクリアしておく
                    for (GLint i = 0; i < m_UniformNum; i++) {
                        m_Uniforms[i].handle = -1;
                    }
                }
            }

            return IZ_TRUE;
        }

        return IZ_FALSE;
    }

    IZ_BOOL CShaderProgramGLES2::GetUniformInfo()
    {
#if !ENABLE_SET_UNIFORM_IMMEDIATERY
        // クリアしておく
        ClearUniformInfo();

        // Get number of attributes
        CALL_GLES2_API(::glGetProgramiv(m_Program, GL_ACTIVE_UNIFORMS, &m_UniformNum));

        void* buf = ALLOC_ZERO(m_Allocator, m_UniformNum * sizeof(SUniform));
        VRETURN(buf != IZ_NULL);

        m_Uniforms = reinterpret_cast<SUniform*>(buf);

        // Get attribute informations.
        for (GLint i = 0; i < m_UniformNum; i++) {
            GLint nameLength;
            GLenum type;
            GLint size;

            CALL_GLES2_API(
                ::glGetActiveUniform(
                    m_Program,
                    i,
                    sizeof(m_Uniforms[i].name),
                    &nameLength,
                    &size,
                    &type,
                    m_Uniforms[i].name));

            for (GLint pos = 0; pos < nameLength; pos++) {
                if (m_Uniforms[i].name[pos] == ' '
                    || m_Uniforms[i].name[pos] == '[')
                {
                    m_Uniforms[i].name[pos] = 0;
                }
            }

            m_Uniforms[i].nameLength = nameLength;
            m_Uniforms[i].type = type;

            // NOTE
            // 問い合わせるuniform変数が配列の場合、この変数にはprogramで使用される配列の最大数（+1）が書き込まれる
            // 問い合わせるuniform変数が配列でない場合、この値は１になります

            // Compute allocation size.
            switch (type)
            {
            case GL_FLOAT:
                m_Uniforms[i].bufferSize = size * sizeof(GLfloat);
                break;
            case GL_FLOAT_VEC2:
                m_Uniforms[i].bufferSize = size * sizeof(GLfloat) * 2;
                break;
            case GL_FLOAT_VEC3:
                m_Uniforms[i].bufferSize = size * sizeof(GLfloat) * 3;
                break;
            case GL_FLOAT_VEC4:
                m_Uniforms[i].bufferSize = size * sizeof(GLfloat) * 4;
                break;
            case GL_INT:
                m_Uniforms[i].bufferSize = size * sizeof(GLint);
                break;
            case GL_INT_VEC2:
                m_Uniforms[i].bufferSize = size * sizeof(GLint) * 2;
                break;
            case GL_INT_VEC3:
                m_Uniforms[i].bufferSize = size * sizeof(GLint) * 3;
                break;
            case GL_INT_VEC4:
                m_Uniforms[i].bufferSize = size * sizeof(GLint) * 4;
                break;
            case GL_BOOL:
                m_Uniforms[i].bufferSize = size * sizeof(GLboolean);
                break;
            case GL_BOOL_VEC2:
                m_Uniforms[i].bufferSize = size * sizeof(GLboolean) * 2;
                break;
            case GL_BOOL_VEC3:
                m_Uniforms[i].bufferSize = size * sizeof(GLboolean) * 3;
                break;
            case GL_BOOL_VEC4:
                m_Uniforms[i].bufferSize = size * sizeof(GLboolean) * 4;
                break;
            case GL_FLOAT_MAT2:
                // TODO
                m_Uniforms[i].bufferSize = size * sizeof(GLfloat) * 2 * 2;
                break;
            case GL_FLOAT_MAT3:
                // TODO
                m_Uniforms[i].bufferSize = size * sizeof(GLfloat) * 3 * 3;
                break;
            case GL_FLOAT_MAT4:
                m_Uniforms[i].bufferSize = size * sizeof(GLfloat) * 4 * 4;
                break;
            case GL_SAMPLER_2D:
            case GL_SAMPLER_CUBE:
                m_Uniforms[i].bufferSize = size * sizeof(GLint);
                break;
            default:
                VRETURN(IZ_FALSE);
                break;
            }

            void* param = ALLOC_ZERO(m_Allocator, m_Uniforms[i].bufferSize);
            VRETURN(param != IZ_NULL);

            m_Uniforms[i].buffer = param;
            m_Uniforms[i].arraySize = size;

            m_Uniforms[i].handle = -1;
            m_Uniforms[i].isDirty = IZ_FALSE;
        }
#endif

        return IZ_TRUE;
    }

    void CShaderProgramGLES2::ClearUniformInfo()
    {
        for (GLint i = 0; i < m_UniformNum; i++) {
            FREE(m_Allocator, m_Uniforms[i].buffer);
        }

        FREE(m_Allocator, m_Uniforms);

        m_Uniforms = IZ_NULL;
        m_UniformNum = 0;
    }

    IZ_BOOL CShaderProgramGLES2::CommitChanges()
    {
        if (m_IsCommitChanged) {
            return IZ_TRUE;
        }

        m_IsCommitChanged = IZ_TRUE;

        for (GLint i = 0; i < m_UniformNum; i++) {
            CommitChanges(i);
        }

        return IZ_TRUE;
    }

    IZ_BOOL CShaderProgramGLES2::CommitChanges(SHADER_PARAM_HANDLE location)
    {
#if !ENABLE_SET_UNIFORM_IMMEDIATERY
        SUniform& uniform = m_Uniforms[location];

        if (uniform.isDirty) {
            SHADER_PARAM_HANDLE handle = uniform.handle;

            //m_Uniforms[i].isDirty = IZ_FALSE;

            // If not assigned handle
            if (handle < 0) {
                CALL_GLES2_API(handle = ::glGetUniformLocation(m_Program, uniform.name));
                //IZ_ASSERT(handle >= 0);

                uniform.handle = handle;
            }

            if (handle >= 0) {
                GLenum type = uniform.type;
                GLsizei num = uniform.arraySize;

                switch (type)
                {
                case GL_FLOAT:
                    {
                        if (num == 1) {
                            GLfloat f = *(GLfloat*)uniform.buffer;
                            CALL_GLES2_API(::glUniform1f(handle, f));
                        }
                        else {
                            GLfloat* f = (GLfloat*)uniform.buffer;
                            CALL_GLES2_API(::glUniform1fv(handle, num, f));
                        }
                    }
                    break;
                case GL_FLOAT_VEC4:
                    {
                        GLfloat* fv = (GLfloat*)uniform.buffer;

                        if (num == 1) {
                            CALL_GLES2_API(::glUniform4f(handle, fv[0], fv[1], fv[2], fv[3]));
                        }
                        else {
                            CALL_GLES2_API(::glUniform4fv(handle, num, fv));
                        }
                    }
                    break;
                case GL_INT:
                    {
                        if (num == 1) {
                            GLint n = *(GLint*)uniform.buffer;
                            CALL_GLES2_API(::glUniform1i(handle, n));
                        }
                        else {
                            GLint* n = (GLint*)uniform.buffer;
                            CALL_GLES2_API(::glUniform1iv(handle, num, n));
                        }
                    }
                    break;
                case GL_BOOL:
                    {
                        GLboolean b = *(GLboolean*)uniform.buffer;
                        CALL_GLES2_API(::glUniform1i(handle, b));
                    }
                    break;
                case GL_FLOAT_MAT4:
                    {
                        GLfloat* mtx = (GLfloat*)uniform.buffer;
                        CALL_GLES2_API(::glUniformMatrix4fv(handle, num, IZ_FALSE, mtx));
                    }
                    break;
                case GL_SAMPLER_2D:
                    {
                        GLint n = *(GLint*)uniform.buffer;
                        CALL_GLES2_API(::glUniform1i(handle, n));
                    }
                    break;
                default:
                    IZ_ASSERT(IZ_FALSE);
                    break;
                }
            }
        }
#endif

        return IZ_TRUE;
    }

    void CShaderProgramGLES2::ClearCommitChanges()
    {
        m_IsCommitChanged = IZ_FALSE;
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
        //IZ_ASSERT(IsLinked());

        Link();

#if ENABLE_SET_UNIFORM_IMMEDIATERY
        CALL_GLES2_API(
            SHADER_PARAM_HANDLE ret = ::glGetUniformLocation(
                m_Program,
                name));
        //IZ_ASSERT(ret >= 0);

        return ret;
#else
        size_t len = ::strlen(name);

        for (GLint i = 0; i < m_UniformNum; i++) {
            size_t maxCount = IZ_MAX(len, m_Uniforms[i].nameLength);

            if (::strncmp(m_Uniforms[i].name, name, maxCount) == 0) {
                return i;
            }
        }

        //IZ_ASSERT(IZ_FALSE);
        return -1;
#endif
    }

    IZ_BOOL CShaderProgramGLES2::SetBool(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, IZ_BOOL b)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(IsLinked());

#if ENABLE_SET_UNIFORM_IMMEDIATERY
        CALL_GLES2_API(::glUniform1i(handle, b));
#else
        IZ_ASSERT(handle >= 0);
        IZ_ASSERT(m_Uniforms[handle].bufferSize == sizeof(GLboolean));

        *(GLboolean*)(m_Uniforms[handle].buffer) = b;
        m_Uniforms[handle].isDirty = IZ_TRUE;
#endif
        return IZ_TRUE;
    }

    IZ_BOOL CShaderProgramGLES2::SetBoolArray(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const IZ_BOOL* b, IZ_UINT num)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(IsLinked());

#if ENABLE_SET_UNIFORM_IMMEDIATERY
        CALL_GLES2_API(::glUniform1iv(handle, num, b));
#else
        // Not implemented
        IZ_ASSERT(IZ_FALSE);
#endif
        return IZ_TRUE;
    }
    
    IZ_BOOL CShaderProgramGLES2::SetFloat(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, IZ_FLOAT f)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(IsLinked());

#if ENABLE_SET_UNIFORM_IMMEDIATERY
        CALL_GLES2_API(::glUniform1f(handle, f));
#else
        IZ_ASSERT(handle >= 0);
        IZ_ASSERT(m_Uniforms[handle].bufferSize == sizeof(GLfloat));

        *(GLfloat*)(m_Uniforms[handle].buffer) = f;
        m_Uniforms[handle].isDirty = IZ_TRUE;
#endif
        return IZ_TRUE;
    }

    IZ_BOOL CShaderProgramGLES2::SetFloatArray(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const IZ_FLOAT* f, IZ_UINT num)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(IsLinked());

#if ENABLE_SET_UNIFORM_IMMEDIATERY
        CALL_GLES2_API(::glUniform1fv(handle, num, f));
#else
        IZ_ASSERT(handle >= 0);
        IZ_ASSERT(m_Uniforms[handle].bufferSize >= sizeof(GLfloat) * num);

        memcpy(m_Uniforms[handle].buffer, f, sizeof(GLfloat) * num);
        m_Uniforms[handle].isDirty = IZ_TRUE;
#endif
        return IZ_TRUE;
    }
    
    IZ_BOOL CShaderProgramGLES2::SetInt(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, IZ_INT n)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(IsLinked());

#if ENABLE_SET_UNIFORM_IMMEDIATERY
        CALL_GLES2_API(::glUniform1i(handle, n));
#else
        IZ_ASSERT(handle >= 0);
        IZ_ASSERT(m_Uniforms[handle].bufferSize == sizeof(GLint));

        *(GLint*)(m_Uniforms[handle].buffer) = n;
        m_Uniforms[handle].isDirty = IZ_TRUE;
#endif
        return IZ_TRUE;
    }

    IZ_BOOL CShaderProgramGLES2::SetIntArray(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const IZ_INT* n, IZ_UINT num)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(IsLinked());

#if ENABLE_SET_UNIFORM_IMMEDIATERY
        CALL_GLES2_API(::glUniform1iv(handle, num, n));
#else
        IZ_ASSERT(handle >= 0);
        IZ_ASSERT(m_Uniforms[handle].bufferSize >= sizeof(GLint) * num);

        memcpy(m_Uniforms[handle].buffer, n, sizeof(GLint) * num);
        m_Uniforms[handle].isDirty = IZ_TRUE;
#endif
        return IZ_TRUE;
    }
    
    IZ_BOOL CShaderProgramGLES2::SetMatrix(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const math::SMatrix& m)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(IsLinked());

#if ENABLE_SET_UNIFORM_IMMEDIATERY
        CALL_GLES2_API(
            ::glUniformMatrix4fv(
                handle,
                1,
                GL_FALSE,
                m.a));
#else
        IZ_ASSERT(handle >= 0);
        IZ_ASSERT(m_Uniforms[handle].bufferSize == sizeof(GLfloat) * 16);

        memcpy(m_Uniforms[handle].buffer, m.a, sizeof(GLfloat) * 16);
        m_Uniforms[handle].isDirty = IZ_TRUE;
#endif
        return IZ_TRUE;
    }

    IZ_BOOL CShaderProgramGLES2::SetMatrixArray(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const math::SMatrix* m, IZ_UINT num)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(IsLinked());

#if ENABLE_SET_UNIFORM_IMMEDIATERY
        CALL_GLES2_API(
            ::glUniformMatrix4fv(
                handle,
                num,
                GL_FALSE,
                (const GLfloat*)m));
#else
        IZ_ASSERT(handle >= 0);
        IZ_ASSERT(m_Uniforms[handle].bufferSize >= sizeof(GLfloat) * 16 * num);

        memcpy(m_Uniforms[handle].buffer, m, sizeof(GLfloat) * 16 * num);
        m_Uniforms[handle].isDirty = IZ_TRUE;
#endif
        return IZ_TRUE;
    }
    
    IZ_BOOL CShaderProgramGLES2::SetVector(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const math::SVector& v)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(IsLinked());

#if ENABLE_SET_UNIFORM_IMMEDIATERY
        CALL_GLES2_API(
            ::glUniform4f(
                handle,
                v.x, v.y, v.z, v.w));
#else
        IZ_ASSERT(handle >= 0);
        IZ_ASSERT(m_Uniforms[handle].bufferSize == sizeof(GLfloat) * 4);

        memcpy(m_Uniforms[handle].buffer, v.v, sizeof(GLfloat) * 4);
        m_Uniforms[handle].isDirty = IZ_TRUE;
#endif
        return IZ_TRUE;
    }

    IZ_BOOL CShaderProgramGLES2::SetVectorArray(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const math::SVector* v, IZ_UINT num)
    {
#if ENABLE_SET_UNIFORM_IMMEDIATERY
        IZ_ASSERT(IsValid());
        IZ_ASSERT(IsLinked());

        CALL_GLES2_API(::glUniform4fv(handle, num, (const GLfloat*)v));
#else
        IZ_ASSERT(handle >= 0);
        IZ_ASSERT(m_Uniforms[handle].bufferSize >= sizeof(GLfloat) * 4 * num);

        memcpy(m_Uniforms[handle].buffer, v, sizeof(GLfloat) * 4 * num);
        m_Uniforms[handle].isDirty = IZ_TRUE;
#endif
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

    IZ_BOOL CShaderProgramGLES2::SetMatrixAsVectorArray(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const math::SMatrix& m)
    {
        math::SMatrix transposed;
        math::SMatrix::Transpose(transposed, m);

        return SetVectorArray(device, handle, transposed.v, 4);
    }
}   // namespace graph
}   // namespace izanagi