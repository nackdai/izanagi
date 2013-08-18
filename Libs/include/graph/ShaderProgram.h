#if !defined(__IZANAGI_GRAPH_SHADER_PROGRAM_H__)
#define __IZANAGI_GRAPH_SHADER_PROGRAM_H__

#include "izDefs.h"
#include "izStd.h"
#include "izMath.h"
#include "graph/GraphDefs.h"

namespace izanagi
{
namespace graph
{
    class CGraphicsDevice;
    class CVertexShader;
    class CPixelShader;

    /**
     */
    class CShaderProgram : public CObject
    {
        friend class CGraphicsDevice;

    protected:
        CShaderProgram();
        virtual ~CShaderProgram();

        NO_COPIABLE(CShaderProgram);

        IZ_DEFINE_INTERNAL_RELEASE();

    public:
        static IZ_BOOL IsValidHandle(const SHADER_PARAM_HANDLE& handle);

        PURE_VIRTUAL(IZ_BOOL AttachVertexShader(CVertexShader* vs));

        PURE_VIRTUAL(IZ_BOOL AttachPixelShader(CPixelShader* ps));

        PURE_VIRTUAL(IZ_BOOL IsValid());

        CVertexShader* GetVertexShader();

        CPixelShader* GetPixelShader();

    public:
        PURE_VIRTUAL(SHADER_PARAM_HANDLE GetHandleByName(IZ_PCSTR name));

    public:
        PURE_VIRTUAL(IZ_BOOL SetBool(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, IZ_BOOL b));
        PURE_VIRTUAL(IZ_BOOL SetBoolArray(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const IZ_BOOL* b, IZ_UINT num));

        PURE_VIRTUAL(IZ_BOOL SetFloat(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, IZ_FLOAT f));
        PURE_VIRTUAL(IZ_BOOL SetFloatArray(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const IZ_FLOAT* f, IZ_UINT num));

        PURE_VIRTUAL(IZ_BOOL SetInt(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, IZ_INT n));
        PURE_VIRTUAL(IZ_BOOL SetIntArray(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const IZ_INT* n, IZ_UINT num));

        PURE_VIRTUAL(IZ_BOOL SetMatrix(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const math::SMatrix& m));
        PURE_VIRTUAL(IZ_BOOL SetMatrixArray(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const math::SMatrix* m, IZ_UINT num));

        PURE_VIRTUAL(IZ_BOOL SetVector(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const math::SVector& v));
        PURE_VIRTUAL(IZ_BOOL SetVectorArray(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const math::SVector* v, IZ_UINT num));

        PURE_VIRTUAL(IZ_BOOL SetValue(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const void* p, IZ_UINT size));

        PURE_VIRTUAL(IZ_BOOL SetMatrixAsVectorArray(CGraphicsDevice* device, const SHADER_PARAM_HANDLE& handle, const math::SMatrix& m));

    protected:
        IMemoryAllocator* m_Allocator;

        CVertexShader* m_VS;
        CPixelShader* m_PS;
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_SHADER_PROGRAM_H__)
