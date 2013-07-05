#include "graph/ShaderConstTableLite.h"
#include "ToolKitIo.h"

namespace izanagi {
namespace tool {
    CShaderConstTableLite* CShaderConstTableLite::CreateShaderConstTableLite(
        IMemoryAllocator* allocator,
        IZ_PCSTR path)
    {
        void *buf = ALLOC(allocator, sizeof(CShaderConstTableLite));
        VRETURN_NULL(buf != IZ_NULL);

        IZ_BOOL result = IZ_TRUE;

        CShaderConstTableLite* instance = new(buf) CShaderConstTableLite;
        {
            instance->AddRef();
            instance->m_Allocator = allocator;

            izanagi::CFileInputStream input;
            result = input.Open(path);
            VGOTO(result, __EXIT__);

            void* program = ALLOC(allocator, (size_t)input.GetSize());
            result = (program != IZ_NULL);
            VGOTO(result, __EXIT__);

            input.Read(program, 0, (size_t)input.GetSize());

            // シェーダ定数テーブル取得
            HRESULT hr = D3DXGetShaderConstantTable(
                    (const DWORD*)program,
                    &instance->m_ConstTable);
            result = SUCCEEDED(hr);

            result = (instance->m_ConstTable != IZ_NULL);
            IZ_ASSERT(result);

            FREE(allocator, program);
        }

__EXIT__:
        if (!result) {
            SAFE_RELEASE(instance);
        }
        return instance;
    }

    CShaderConstTableLite::CShaderConstTableLite()
    {
        m_ConstTable = IZ_NULL;
    }

    CShaderConstTableLite::~CShaderConstTableLite()
    {
        SAFE_RELEASE(m_ConstTable);
    }

    IZ_UINT CShaderConstTableLite::GetSamplerIndex(IZ_PCSTR name)
    {
        IZ_ASSERT(m_ConstTable != IZ_NULL);

        D3DXHANDLE handle = m_ConstTable->GetConstantByName(NULL, name);
        IZ_ASSERT(handle != IZ_NULL);

        IZ_UINT ret = m_ConstTable->GetSamplerIndex(handle);
        return ret;
    }
}   // namespace tool
}   // namespace izanagi
