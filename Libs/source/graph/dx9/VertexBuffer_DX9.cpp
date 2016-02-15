#include "graph/dx9/VertexBuffer_DX9.h"
#include "graph/dx9/GraphicsDevice_DX9.h"

namespace izanagi
{
namespace graph
{
    // インスタンス作成
    CVertexBufferDX9* CVertexBufferDX9::CreateVertexBuffer(
        CGraphicsDeviceDX9* device,
        IMemoryAllocator* allocator,
        IZ_UINT stride,
        IZ_UINT vtxNum,
        E_GRAPH_RSC_USAGE usage)
    {
        IZ_ASSERT(device != NULL);

        IZ_BOOL result = IZ_TRUE;
        IZ_UINT8* buf = IZ_NULL;
        CVertexBufferDX9* instance = IZ_NULL;

        // メモリ確保
        buf = (IZ_UINT8*)ALLOC_ZERO(allocator, sizeof(CVertexBufferDX9));
        if (!(result = (buf != IZ_NULL))) {
            IZ_ASSERT(IZ_FALSE);
            goto __EXIT__;
        }

        // インスタンス作成
        instance = new (buf)CVertexBufferDX9;
        {
            SAFE_REPLACE(instance->m_Device, device);
            instance->m_Allocator = allocator;
            instance->AddRef();
        }

        // 本体作成
        result = instance->CreateBody(
            device,
            stride,
            vtxNum,
            usage);
        if (!result) {
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

    // コンストラクタ
    CVertexBufferDX9::CVertexBufferDX9()
    {
        m_Device = IZ_NULL;
        m_VB = IZ_NULL;
    }

    // デストラクタ
    CVertexBufferDX9::~CVertexBufferDX9()
    {
        m_Device->RemoveVertexBuffer(this);

        SAFE_RELEASE(m_VB);
        SAFE_RELEASE(m_Device);
    }

    // 本体作成
    IZ_BOOL CVertexBufferDX9::CreateBody(
        CGraphicsDeviceDX9* device,
        IZ_UINT stride,
        IZ_UINT vtxNum,
        E_GRAPH_RSC_USAGE usage)
    {
        IZ_ASSERT(device != IZ_NULL);

        D3D_DEVICE* d3dDev = device->GetRawInterface();

        IZ_DWORD nUsage = (usage == E_GRAPH_RSC_USAGE_DYNAMIC
                            ? D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY
                            : 0);

        D3DPOOL nPool = (usage == E_GRAPH_RSC_USAGE_DYNAMIC
                            ? D3DPOOL_DEFAULT
                            : D3DPOOL_MANAGED);

        // 本体作成
        HRESULT hr = d3dDev->CreateVertexBuffer(
                        stride * vtxNum,
                        nUsage,
                        0,
                        nPool,
                        &m_VB,
                        IZ_NULL);

        IZ_BOOL ret = SUCCEEDED(hr);
        IZ_ASSERT(ret);

        m_Stride = stride;
        m_VtxNum = vtxNum;
    
        m_CreateType = usage;

        return ret;
    }

    // ロック
    IZ_BOOL CVertexBufferDX9::Lock(
        CGraphicsDevice* device,
        IZ_UINT offset,
        IZ_UINT size,
        void** data,
        IZ_BOOL isReadOnly,
        IZ_BOOL isDiscard/*= IZ_FALSE*/)
    {
        // NOTE
        // 高速化のために IsDynamic のときは WRITEONLYで
        // 作成しているので、READONLY は不可

        IZ_ASSERT(m_VB != IZ_NULL);

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

        HRESULT hr = m_VB->Lock(
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
    IZ_BOOL CVertexBufferDX9::Unlock(CGraphicsDevice* device)
    {
        IZ_ASSERT(m_VB != IZ_NULL);
    
        HRESULT hr = m_VB->Unlock();
        IZ_BOOL ret = SUCCEEDED(hr);

        return ret;
    }

    IZ_BOOL CVertexBufferDX9::IsPrepared() const
    {
        return (m_VB != IZ_NULL);
    }

    IZ_BOOL CVertexBufferDX9::Disable()
    {
        SAFE_RELEASE(m_VB);
        return IZ_TRUE;
    }

    // リセット
    IZ_BOOL CVertexBufferDX9::Restore()
    {
        IZ_BOOL ret = IZ_TRUE;

        // NOTE
        // Dynamicでないときは、D3DPOOL_MANAGEDで作成されているので再作成する必要はない

        if (IsDynamic()) {
            SAFE_RELEASE(m_VB);

            // 本体作成
            ret = CreateBody(
                    m_Device,
                    m_Stride,
                    m_VtxNum,
                    m_CreateType);
        }

        return ret;
    }
}   // namespace graph
}   // namespace izanagi
