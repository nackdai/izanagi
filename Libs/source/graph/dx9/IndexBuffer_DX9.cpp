#include "graph/dx9/IndexBuffer_DX9.h"
#include "graph/internal/ParamValueConverter.h"
#include "graph/dx9/GraphicsDevice_DX9.h"

namespace izanagi
{
namespace graph
{
    // インスタンス作成
    CIndexBufferDX9* CIndexBufferDX9::CreateIndexBuffer(
	    CGraphicsDeviceDX9* device,
	    IMemoryAllocator* allocator,
	    IZ_UINT idxNum,
	    E_GRAPH_INDEX_BUFFER_FMT fmt,
	    E_GRAPH_RSC_TYPE createType)
    {
	    IZ_ASSERT(device != NULL);

	    IZ_BOOL result = IZ_TRUE;
	    IZ_UINT8* but = IZ_NULL;
	    CIndexBufferDX9* instance = IZ_NULL;

	    // メモリ確保
	    but = (IZ_UINT8*)ALLOC_ZERO(allocator, sizeof(CIndexBufferDX9));
	    if (!(result = (but != IZ_NULL))) {
		    IZ_ASSERT(IZ_FALSE);
		    goto __EXIT__;
	    }

	    // インスタンス作成
	    instance = new (but)CIndexBufferDX9;
	    {
		    instance->m_Allocator = allocator;
		    instance->AddRef();
		    SAFE_REPLACE(instance->m_Device, device);
	    }

	    // 本体作成
	    result = instance->CreateBody(
            device,
            idxNum,
			fmt,
			createType);
	    if (!result) {
		    goto __EXIT__;
	    }

    __EXIT__:
	    if (!result) {
		    if (instance != IZ_NULL) {
			    SAFE_RELEASE(instance);
		    }
		    else if (but != IZ_NULL) {
			    allocator->Free(but);
		    }
	    }

	    return instance;
    }

    // コンストラクタ
	CIndexBufferDX9::CIndexBufferDX9()
	{
        m_Device = IZ_NULL;
		m_IB = IZ_NULL;
	}

	// デストラクタ
	CIndexBufferDX9::~CIndexBufferDX9()
	{
		m_Device->RemoveIndexBuffer(this);

		SAFE_RELEASE(m_IB);
		SAFE_RELEASE(m_Device);
	}

    void CIndexBufferDX9::ReleaseResource()
	{
		SAFE_RELEASE(m_IB);
	}

    // 本体作成
    IZ_BOOL CIndexBufferDX9::CreateBody(
        CGraphicsDeviceDX9* device,
	    IZ_UINT idxNum,
	    E_GRAPH_INDEX_BUFFER_FMT fmt,
	    E_GRAPH_RSC_TYPE createType)
    {
	    IZ_ASSERT(device != IZ_NULL);

	    D3D_DEVICE* d3dDev = device->GetRawInterface();

	    D3DFORMAT d3dFmt = IZ_GET_TARGET_IDX_BUF_FMT(fmt);
	    IZ_UINT stride = (d3dFmt == D3DFMT_INDEX16 ? sizeof(IZ_UINT16) : sizeof(IZ_UINT32));

	    IZ_DWORD usage = (createType == E_GRAPH_RSC_TYPE_DYNAMIC
                            ? D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY
			                : 0);

	    D3DPOOL pool = (createType == E_GRAPH_RSC_TYPE_DYNAMIC
						    ? D3DPOOL_DEFAULT
						    : D3DPOOL_MANAGED);

	    HRESULT hr = d3dDev->CreateIndexBuffer(
            idxNum * stride,
			usage,
			d3dFmt,
			pool,
			&m_IB,
			IZ_NULL);

	    IZ_BOOL ret = SUCCEEDED(hr);
	    IZ_ASSERT(ret);

	    m_IdxNum = idxNum;
	    m_Fmt = fmt;
	
	    m_CreateType = createType;

	    return ret;
    }

    // ロック
    IZ_BOOL CIndexBufferDX9::Lock(
	    IZ_UINT offset,
	    IZ_UINT size,
	    void** data,
	    IZ_BOOL isReadOnly,
	    IZ_BOOL isDiscard/*= IZ_FALSE*/)
    {
	    // NOTE
	    // 高速化のために IsDynamic のときは WRITEONLYで
	    // 作成しているので、READONLY は不可

	    IZ_ASSERT(m_IB != IZ_NULL);

	    DWORD flag = 0;
	    if (IsDynamic()) {
		    // READONLY不可
		    VRETURN(!isReadOnly);
		
		    if (isDiscard) {
			    flag = D3DLOCK_DISCARD;
		    }
		    else {
			    flag = D3DLOCK_NOOVERWRITE;
		    }
	    }
	    else if (isReadOnly) {
		    flag = D3DLOCK_READONLY;
	    }

	    HRESULT hr = m_IB->Lock(
					    offset,
					    size,
					    data,
					    flag);

	    IZ_BOOL ret = SUCCEEDED(hr);

	    return ret;
    }

    /**
    * アンロック
    */
    IZ_BOOL CIndexBufferDX9::Unlock()
    {
	    IZ_ASSERT(m_IB != IZ_NULL);
	
	    HRESULT hr = m_IB->Unlock();
	    IZ_BOOL ret = SUCCEEDED(hr);

	    return ret;
    }

    // リセット
    IZ_BOOL CIndexBufferDX9::Reset()
    {
	    IZ_BOOL ret = IZ_TRUE;

	    // NOTE
	    // Dynamicでないときは、D3DPOOL_MANAGEDで作成されているので再作成する必要はない

	    if (IsDynamic()) {
		    D3D_DEVICE* pD3DDev = m_Device->GetRawInterface();

		    SAFE_RELEASE(m_IB);

		    // 本体作成
		    ret = CreateBody(
                    m_Device,
				    m_IdxNum,
				    m_Fmt,
				    m_CreateType);
	    }

	    return ret;
    }

}   // namespace graph
}   // namespace izanagi
