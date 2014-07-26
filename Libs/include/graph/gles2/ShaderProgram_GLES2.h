#if !defined(__IZANAGI_GRAPH_SHADER_PROGRAM_GLES2_H__)
#define __IZANAGI_GRAPH_SHADER_PROGRAM_GLES2_H__

#include "graph/GraphDefs.h"
#include "graph/ShaderProgram.h"

namespace izanagi
{
namespace graph
{
    class CVertexShaderGLES2;
    class CPixelShaderGLES2;

    /**
     */
    class CShaderProgramGLES2 : public CShaderProgram
    {
        friend class CGraphicsDeviceGLES2;
        friend class CVertexDeclarationGLES2;

        static CShaderProgram* CreateShaderProgram(IMemoryAllocator* allocator);

    private:
        CShaderProgramGLES2();
        virtual ~CShaderProgramGLES2();

    public:
        virtual IZ_BOOL AttachVertexShader(CVertexShader* vs);

        virtual IZ_BOOL AttachPixelShader(CPixelShader* ps);

        IZ_BOOL Link();

        virtual IZ_BOOL IsValid();

        IZ_BOOL IsLinked();

    private:
        void ClearAttributeInfo();

        IZ_INT GetAttribIndex(const char* name);

    public:
        virtual SHADER_PARAM_HANDLE GetHandleByName(IZ_PCSTR name);

    public:
        virtual IZ_BOOL SetBool(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, IZ_BOOL b);
        virtual IZ_BOOL SetBoolArray(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const IZ_BOOL* b, IZ_UINT num);

        virtual IZ_BOOL SetFloat(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, IZ_FLOAT f);
        virtual IZ_BOOL SetFloatArray(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const IZ_FLOAT* f, IZ_UINT num);

        virtual IZ_BOOL SetInt(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, IZ_INT n);
        virtual IZ_BOOL SetIntArray(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const IZ_INT* n, IZ_UINT num);

        virtual IZ_BOOL SetMatrix(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const math::SMatrix& m);
        virtual IZ_BOOL SetMatrixArray(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const math::SMatrix* m, IZ_UINT num);

        virtual IZ_BOOL SetVector(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const math::SVector4& v);
        virtual IZ_BOOL SetVectorArray(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const math::SVector4* v, IZ_UINT num);

        virtual IZ_BOOL SetValue(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const void* p, IZ_UINT size);

        virtual IZ_BOOL SetMatrixArrayAsVectorArray(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const math::SMatrix* m, IZ_UINT num);

    private:
        CVertexShaderGLES2* VertexShader();
        CPixelShaderGLES2* PixelShader();

    private:
        GLuint GetRawInterface() { return m_Program; }

    private:
        struct SAttribute
        {
            char name[32];

            IZ_INT index;
            GLsizei nameLength;
        };

    private:
        GLuint m_Program;
        IZ_BOOL m_IsLinked;

        GLint m_AttribNum;
        SAttribute* m_Attribs;
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_SHADER_PROGRAM_GLES2_H__)
