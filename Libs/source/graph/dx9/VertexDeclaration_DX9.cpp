#include "graph/dx9/VertexDeclaration_DX9.h"
#include "graph/internal/ParamValueConverter.h"
#include "graph/dx9/GraphicsDevice_DX9.h"

namespace izanagi
{
namespace graph
{
    // インスタンス作成
    CVertexDeclaration* CVertexDeclarationDX9::CreateVertexDeclaration(
	    CGraphicsDeviceDX9* device,
	    IMemoryAllocator* allocator,
	    const D3D_VTX_ELEMENT* elem)
    {
	    CVertexDeclarationDX9* instance = IZ_NULL;
	    IZ_UINT8* buf = IZ_NULL;
	    IZ_BOOL result = IZ_FALSE;

	    // メモリ確保
	    buf = (IZ_UINT8*)ALLOC_ZERO(allocator, sizeof(CVertexDeclarationDX9));
	    result = (buf != IZ_NULL);
	    if (!result) {
		    IZ_ASSERT(IZ_FALSE);
		    goto __EXIT__;
	    }

	    // インスタンス作成
	    instance = new(buf) CVertexDeclarationDX9;
	    {
		    instance->AddRef();
		    instance->m_Allocator = allocator;
	    }

	    // 頂点宣言本体作成
	    D3D_DEVICE* pRawDevice = device->GetRawInterface();
	    HRESULT hr = pRawDevice->CreateVertexDeclaration(
					    elem,
					    &instance->m_VD);
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

    // インスタンス作成
    CVertexDeclaration* CVertexDeclarationDX9::CreateVertexDeclaration(
	    CGraphicsDeviceDX9* device,
	    IMemoryAllocator* allocator,
	    const SVertexElement* elem,
	    IZ_UINT elemNum)
    {
	    static const IZ_UINT ElemBufferSize = 32;
	    static D3D_VTX_ELEMENT ElemBuffer[ElemBufferSize];

	    IZ_ASSERT(elemNum < ElemBufferSize - 1);

	    IZ_UINT nPos = 0;

	    for (nPos = 0; nPos < elemNum; ++nPos) {
		    ElemBuffer[nPos].Stream = elem[nPos].Stream;
		    ElemBuffer[nPos].Offset = elem[nPos].Offset;
		    ElemBuffer[nPos].Type = IZ_GET_TARGET_VTX_DECL_TYPE(elem[nPos].Type);
		    ElemBuffer[nPos].Method = 0;
		    ElemBuffer[nPos].Usage = IZ_GET_TARGET_VTX_DECL_USAGE(elem[nPos].Usage);
		    ElemBuffer[nPos].UsageIndex = elem[nPos].UsageIndex;
	    }

	    FILL_ZERO(&ElemBuffer[nPos], sizeof(ElemBuffer[nPos]));
	    ElemBuffer[nPos].Stream = 0xff;
	    ElemBuffer[nPos].Type = D3DDECLTYPE_UNUSED;

	    CVertexDeclaration* ret = CreateVertexDeclaration(
								    device,
								    allocator,
								    ElemBuffer);
	    IZ_ASSERT(ret != IZ_NULL);

	    return ret;
    }

    // コンストラクタ
	CVertexDeclarationDX9::CVertexDeclarationDX9()
	{
        m_VD = IZ_NULL;
	}

	// デストラクタ
	CVertexDeclarationDX9::~CVertexDeclarationDX9()
	{
		SAFE_RELEASE(m_VD);
	}
}   // namespace graph
}   // namespace izanagi