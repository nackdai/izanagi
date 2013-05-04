#include "graph/gles2/VertexShader_GLES2.h"
#include "graph/gles2/GraphicsDevice_GLES2.h"

namespace izanagi
{
namespace graph
{
    // インスタンス作成
    CVertexShader* CVertexShaderGLES2::CreateVertexShader(
        CGraphicsDeviceGLES2* device,
        IMemoryAllocator* allocator,
        const void* program)
    {
        CVertexShaderGLES2* instance = IZ_NULL;
        IZ_UINT8* buf = IZ_NULL;
        IZ_BOOL result = IZ_TRUE;

        // メモリ確保
        buf = (IZ_UINT8*)ALLOC_ZERO(allocator, sizeof(CVertexShaderGLES2));
        result = (buf != IZ_NULL);
        if (!result) {
            IZ_ASSERT(IZ_FALSE);
            goto __EXIT__;
        }

        // インスタンス作成
        instance = new(buf) CVertexShaderGLES2;
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

    CVertexShaderGLES2::CVertexShaderGLES2()
    {
        m_VS = IZ_NULL;
    }

    CVertexShaderGLES2::~CVertexShaderGLES2()
    {
        SAFE_RELEASE(m_VS);
    }
}   // namespace graph
}   // namespace izanagi
