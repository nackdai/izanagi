#if !defined(__IZANAGI_GRAPH_SHADER_BASE_H__)
#define __IZANAGI_GRAPH_SHADER_BASE_H__

#include "izDefs.h"
#include "izStd.h"
#include "izMath.h"
#include "graph/GraphDefs.h"

namespace izanagi
{
namespace graph
{
    class CGraphicsDevice;

	// シェーダ定数テーブル
	class IShader : public CObject {
	protected:
        IShader()
        {
            m_Allocator = IZ_NULL;
        }
        virtual ~IShader() {}

		NO_COPIABLE(IShader);

        IZ_DEFINE_INTERNAL_RELEASE();

	public:
		PURE_VIRTUAL(SHADER_PARAM_HANDLE GetHandleByIdx(IZ_UINT idx));
		PURE_VIRTUAL(SHADER_PARAM_HANDLE GetHandleByName(IZ_PCSTR name));

		PURE_VIRTUAL(IZ_UINT GetArrayLengthOf(CGraphicsDevice* device, SHADER_PARAM_HANDLE handle));

        PURE_VIRTUAL(IZ_UINT GetSamplerIndex(SHADER_PARAM_HANDLE handle));

	public:
		PURE_VIRTUAL(IZ_BOOL SetDefaults(CGraphicsDevice* device));

		PURE_VIRTUAL(IZ_BOOL SetBool(CGraphicsDevice* device, SHADER_PARAM_HANDLE handle, IZ_BOOL b));
		PURE_VIRTUAL(IZ_BOOL SetBoolArray(CGraphicsDevice* device, SHADER_PARAM_HANDLE handle, const IZ_BOOL* b, IZ_UINT num));

		PURE_VIRTUAL(IZ_BOOL SetFloat(CGraphicsDevice* device, SHADER_PARAM_HANDLE handle, IZ_FLOAT f));
		PURE_VIRTUAL(IZ_BOOL SetFloatArray(CGraphicsDevice* device, SHADER_PARAM_HANDLE handle, const IZ_FLOAT* f, IZ_UINT num));

		PURE_VIRTUAL(IZ_BOOL SetInt(CGraphicsDevice* device, SHADER_PARAM_HANDLE handle, IZ_INT n));
		PURE_VIRTUAL(IZ_BOOL SetIntArray(CGraphicsDevice* device, SHADER_PARAM_HANDLE handle, const IZ_INT* n, IZ_UINT num));

		PURE_VIRTUAL(IZ_BOOL SetMatrix(CGraphicsDevice* device, SHADER_PARAM_HANDLE handle, const SMatrix& m));
		PURE_VIRTUAL(IZ_BOOL SetMatrixArray(CGraphicsDevice* device, SHADER_PARAM_HANDLE handle, const SMatrix* m, IZ_UINT num));

		PURE_VIRTUAL(IZ_BOOL SetVector(CGraphicsDevice* device, SHADER_PARAM_HANDLE handle, const SVector& v));
		PURE_VIRTUAL(IZ_BOOL SetVectorArray(CGraphicsDevice* device, SHADER_PARAM_HANDLE handle, const SVector* v, IZ_UINT num));

		PURE_VIRTUAL(IZ_BOOL SetValue(CGraphicsDevice* device, SHADER_PARAM_HANDLE handle, const void* p, IZ_UINT size));

    protected:
        IMemoryAllocator* m_Allocator;
	};
}   // namespace graph
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_GRAPH_INTERNAL_SHADER_BASE_H__)
