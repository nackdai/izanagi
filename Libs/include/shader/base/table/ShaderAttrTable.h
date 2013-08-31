#if !defined(__IZANAGI_SHADER_SHADER_ATTR_TABLE_H__)
#define __IZANAGI_SHADER_SHADER_ATTR_TABLE_H__

#include "shader/base/SHDFormat.h"

namespace izanagi
{
namespace shader
{
    // Table of shader's attributes.
    class CShaderAttrTable {
        friend class CShaderBasic;

        static size_t ComputeBufferSize(IZ_UINT16 numAttrs);

    private:
        CShaderAttrTable();
        ~CShaderAttrTable() {}

        NO_COPIABLE(CShaderAttrTable);

    private:
        // Initialize table.
        IZ_UINT8* Init(IZ_UINT8* p);
        IZ_UINT8* SetInternalBuffer(IZ_UINT8* p);

        const S_SHD_ATTRIBUTE* GetAttrByIdx(IZ_UINT idx) const;
        const S_SHD_ATTRIBUTE* GetAttrByKey(IZ_UINT key);

    private:
        struct SAttributeHolder {
            S_SHD_ATTRIBUTE* attr;
            CStdHash<IZ_UINT, SAttributeHolder, SHD_ATTR_HASH_MAX>::Item hashItem;
        };

    private:
        S_SHD_ATTR_HEADER m_Header;

        S_SHD_ATTRIBUTE* m_Attrs;
        SAttributeHolder* m_AttrHolder;

        CStdHash<IZ_UINT, SAttributeHolder, SHD_ATTR_HASH_MAX> m_AttrHash;
    };
}   // namespace shader
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SHADER_SHADER_ATTR_TABLE_H__)
