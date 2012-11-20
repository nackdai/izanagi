#include "graph/dx9/VertexShader_DX9.h"
#include "graph/dx9/GraphicsDevice_DX9.h"

namespace izanagi
{
namespace graph
{
    // インスタンス作成
    CVertexShader* CVertexShaderDX9::CreateVertexShader(
	    CGraphicsDeviceDX9* device,
	    IMemoryAllocator* allocator,
	    const void* program)
    {
	    CVertexShaderDX9* instance = IZ_NULL;
	    IZ_UINT8* buf = IZ_NULL;
	    IZ_BOOL result = IZ_TRUE;

	    // メモリ確保
	    buf = (IZ_UINT8*)ALLOC_ZERO(allocator, sizeof(CVertexShaderDX9));
	    result = (buf != IZ_NULL);
	    if (!result) {
		    IZ_ASSERT(IZ_FALSE);
		    goto __EXIT__;
	    }

	    // インスタンス作成
	    instance = new(buf) CVertexShaderDX9;
	    {
		    instance->AddRef();
		    instance->m_Allocator = allocator;
	    }

	    //IZ_C_ASSERT(sizeof(DWORD) == sizeof(IZ_DWORD));

	    // シェーダ作成
	    D3D_DEVICE* d3dDevice = device->GetRawInterface();
	    HRESULT hr = d3dDevice->CreateVertexShader(
					    (const DWORD*)program,
					    &instance->m_VS);
	    result = SUCCEEDED(hr);
	    if (!result) {
		    IZ_ASSERT(IZ_FALSE);
		    goto __EXIT__;
	    }

	    // シェーダ定数テーブル取得
	    hr = D3DXGetShaderConstantTable(
			    (const DWORD*)program,
			    &instance->m_ConstTable);
	    result = SUCCEEDED(hr);
	    if (!result) {
		    IZ_ASSERT(IZ_FALSE);
		    goto __EXIT__;
	    }

    __EXIT__:
	    if (!result) {
		    if (instance != IZ_NULL) {
			    SAFE_RELEASE(instance);
		    }
		    else if (buf != IZ_NULL) {
			    allocator->Free(buf);
		    }
	    }

	    return instance;
    }

    CVertexShaderDX9::CVertexShaderDX9()
    {
        m_VS = IZ_NULL;
    }

    CVertexShaderDX9::~CVertexShaderDX9()
    {
        SAFE_RELEASE(m_VS);
    }
}   // namespace graph
}   // namespace izanagi
