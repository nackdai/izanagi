#include "graph/dx9/ShaderProgram_DX9.h"
#include "graph/dx9/GraphicsDevice_DX9.h"
#include "graph/dx9/VertexShader_DX9.h"
#include "graph/dx9/PixelShader_DX9.h"

namespace izanagi
{
namespace graph
{
    IZ_BOOL CShaderProgram::IsValidHandle(const SHADER_PARAM_HANDLE& handle)
    {
        return (handle != 0);
    }

    CShaderProgram* CShaderProgramDX9::CreateShaderProgram(IMemoryAllocator* allocator)
    {
        void* buf = ALLOC(allocator, sizeof(CShaderProgramDX9));
        VRETURN(buf != IZ_NULL);

        CShaderProgramDX9* instance = new(buf) CShaderProgramDX9;
        {
            instance->AddRef();
            instance->m_Allocator = allocator;
        }

        return instance;
    }

    CShaderProgramDX9::CShaderProgramDX9()
    {
    }

    IZ_BOOL CShaderProgramDX9::AttachVertexShader(CVertexShader* vs)
    {
        IZ_ASSERT(vs != IZ_NULL);

        if (m_VS == IZ_NULL) {
            SAFE_REPLACE(m_VS, vs);
        }
        return IZ_TRUE;
    }

    IZ_BOOL CShaderProgramDX9::AttachPixelShader(CPixelShader* ps)
    {
        IZ_ASSERT(ps != IZ_NULL);
        
        if (m_PS == IZ_NULL) {
            SAFE_REPLACE(m_PS, ps);
        }
        return IZ_TRUE;
    }

    IZ_BOOL CShaderProgramDX9::IsValid()
    {
        return (m_VS != IZ_NULL && m_PS != IZ_NULL);
    }

    CVertexShaderDX9* CShaderProgramDX9::VertexShader()
    {
        return (CVertexShaderDX9*)m_VS;
    }

    CPixelShaderDX9* CShaderProgramDX9::PixelShader()
    {
        return (CPixelShaderDX9*)m_PS;
    }

    SHADER_PARAM_HANDLE CShaderProgramDX9::GetHandleByName(IZ_PCSTR name)
    {
        IZ_ASSERT(IsValid());

        SHADER_PARAM_HANDLE ret;

        ret.vsParam = VertexShader()->GetHandleByName(name);
        ret.psParam = PixelShader()->GetHandleByName(name);

        return ret;
    }

#define _EXEC_FUNC(func, device, param)\
    IZ_BOOL b1, b2; \
    b1 = b2 = IZ_TRUE; \
    if (handle.vsParam != 0) { \
        b1 = VertexShader()->func(device, handle.vsParam, param); \
        VRETURN(b1); \
    } \
    if (handle.psParam != 0) { \
        b2 = PixelShader()->func(device, handle.psParam, param); \
        VRETURN(b2); \
    } \
    return (b1 && b2)

#define _EXEC_ARRAY_FUNC(func, device, param, num)\
    IZ_BOOL b1, b2; \
    b1 = b2 = IZ_TRUE; \
    if (handle.vsParam != 0) { \
        b1 = VertexShader()->func(device, handle.vsParam, param, num); \
        VRETURN(b1); \
    } \
    if (handle.psParam != 0) { \
        b2 = PixelShader()->func(device, handle.psParam, param, num); \
        VRETURN(b2); \
    } \
    return (b1 && b2)

    IZ_BOOL CShaderProgramDX9::SetBool(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, IZ_BOOL b)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(device->GetShaderProgram() == this);

        _EXEC_FUNC(SetBool, device, b);
    }

    IZ_BOOL CShaderProgramDX9::SetBoolArray(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const IZ_BOOL* b, IZ_UINT num)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(device->GetShaderProgram() == this);

        _EXEC_ARRAY_FUNC(SetBoolArray, device, b, num);
    }
    
    IZ_BOOL CShaderProgramDX9::SetFloat(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, IZ_FLOAT f)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(device->GetShaderProgram() == this);

        _EXEC_FUNC(SetFloat, device, f);
    }

    IZ_BOOL CShaderProgramDX9::SetFloatArray(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const IZ_FLOAT* f, IZ_UINT num)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(device->GetShaderProgram() == this);

        _EXEC_ARRAY_FUNC(SetFloatArray, device, f, num);
    }
    
    IZ_BOOL CShaderProgramDX9::SetInt(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, IZ_INT n)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(device->GetShaderProgram() == this);

        _EXEC_FUNC(SetInt, device, n);
    }

    IZ_BOOL CShaderProgramDX9::SetIntArray(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const IZ_INT* n, IZ_UINT num)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(device->GetShaderProgram() == this);

        _EXEC_ARRAY_FUNC(SetIntArray, device, n, num);
    }
    
    IZ_BOOL CShaderProgramDX9::SetMatrix(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const math::SMatrix& m)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(device->GetShaderProgram() == this);

        _EXEC_FUNC(SetMatrix, device, m);
    }

    IZ_BOOL CShaderProgramDX9::SetMatrixArray(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const math::SMatrix* m, IZ_UINT num)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(device->GetShaderProgram() == this);

        _EXEC_ARRAY_FUNC(SetMatrixArray, device, m, num);
    }
    
    IZ_BOOL CShaderProgramDX9::SetVector(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const math::SVector& v)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(device->GetShaderProgram() == this);

        _EXEC_FUNC(SetVector, device, v);
    }

    IZ_BOOL CShaderProgramDX9::SetVectorArray(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const math::SVector* v, IZ_UINT num)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(device->GetShaderProgram() == this);

        _EXEC_ARRAY_FUNC(SetVectorArray, device, v, num);
    }
    
    IZ_BOOL CShaderProgramDX9::SetValue(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const void* p, IZ_UINT size)
    {
        IZ_ASSERT(IsValid());
        IZ_ASSERT(device->GetShaderProgram() == this);

        _EXEC_ARRAY_FUNC(SetValue, device, p, size);
    }

    IZ_BOOL CShaderProgramDX9::SetMatrixAsVectorArray(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const math::SMatrix& m)
    {
        return SetVectorArray(device, handle, m.v, 4);
    }
}   // namespace graph
}   // namespace izanagi