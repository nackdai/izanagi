#include "graph/gles2/VertexShader_GLES2.h"
#include "graph/GraphicsDevice.h"

namespace izanagi
{
namespace graph
{
    // インスタンス作成
    CVertexShader* CVertexShaderGLES2::CreateVertexShader(
        CGraphicsDevice* device,
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
        result = instance->CreateShader(program, GL_VERTEX_SHADER);


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
    }

    CVertexShaderGLES2::~CVertexShaderGLES2()
    {
    }
}   // namespace graph
}   // namespace izanagi
