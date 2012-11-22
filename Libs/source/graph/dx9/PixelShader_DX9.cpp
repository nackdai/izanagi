#include "graph/dx9/PixelShader_DX9.h"
#include "graph/dx9/GraphicsDevice_DX9.h"

namespace izanagi
{
namespace graph
{
    // インスタンス作成
    CPixelShader* CPixelShaderDX9::CreatePixelShader(
	    CGraphicsDeviceDX9* device,
	    IMemoryAllocator* allocator,
	    const void* program)
    {
	    CPixelShaderDX9* instance = IZ_NULL;
	    IZ_UINT8* buf = IZ_NULL;
	    IZ_BOOL result = IZ_TRUE;

	    // メモリ確保
	    buf = (IZ_UINT8*)ALLOC_ZERO(allocator, sizeof(CPixelShaderDX9));
	    result = (buf != IZ_NULL);
	    if (!result) {
		    IZ_ASSERT(IZ_FALSE);
		    goto __EXIT__;
	    }

	    // インスタンス作成
	    instance = new(buf) CPixelShaderDX9;
	    {
		    instance->AddRef();
		    instance->m_Allocator = allocator;
	    }

	    //IZ_C_ASSERT(sizeof(DWORD) == sizeof(IZ_DWORD));

	    // シェーダ作成
	    D3D_DEVICE* pRawDevice = device->GetRawInterface();
	    HRESULT hr = pRawDevice->CreatePixelShader(
					    (const DWORD*)program,
					    &instance->m_PS);
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

    CPixelShaderDX9::CPixelShaderDX9()
    {
        m_PS = IZ_NULL;
    }

    CPixelShaderDX9::~CPixelShaderDX9()
    {
        SAFE_RELEASE(m_PS);
    }
}   // namespace graph
}   // namespace izanagi
