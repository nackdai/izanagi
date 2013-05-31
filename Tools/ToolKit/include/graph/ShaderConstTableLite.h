#if !defined(__IZANAGI_TOOL_KIT_SHADER_CONST_TABLE_LITE_H__)
#define __IZANAGI_TOOL_KIT_SHADER_CONST_TABLE_LITE_H__

#include <d3d9.h>
#include <d3dx9.h>

#include "izDefs.h"
#include "izStd.h"
#include "izGraph.h"
#include "graph/ToolkitGraphDefs.h"

namespace izanagi {
namespace tool {
    /**
    */
    class CShaderConstTableLite : public CObject {
    public:
        static CShaderConstTableLite* CreateShaderConstTableLite(
            IMemoryAllocator* allocator,
            IZ_PCSTR path);

    private:
        CShaderConstTableLite();
        ~CShaderConstTableLite();

        NO_COPIABLE(CShaderConstTableLite);

        IZ_DEFINE_INTERNAL_RELEASE();

    public:
        IZ_UINT GetSamplerIndex(IZ_PCSTR name);
        
    private:
        IMemoryAllocator* m_Allocator;

        ID3DXConstantTable* m_ConstTable;
    };
}   // namespace tool
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_TOOL_KIT_SHADER_CONST_TABLE_LITE_H__)
