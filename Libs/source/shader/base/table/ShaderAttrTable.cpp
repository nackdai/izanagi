#include "shader/base/table/ShaderAttrTable.h"

namespace izanagi
{
namespace shader
{
    size_t CShaderAttrTable::ComputeBufferSize(IZ_UINT16 numAttrs)
    {
        size_t ret = sizeof(SAttributeHolder) * numAttrs;
        return ret;
    }

    CShaderAttrTable::CShaderAttrTable()
    {
        FILL_ZERO(&m_Header, sizeof(m_Header));
        m_Attrs = IZ_NULL;
        m_AttrHolder = IZ_NULL;
    }

    // Initialize table.
    IZ_UINT8* CShaderAttrTable::Init(IZ_UINT8* p)
    {
        memcpy(&m_Header, p, sizeof(m_Header));
        p += sizeof(m_Header);

        if (m_Header.numAttrs > 0) {
            m_Attrs = (S_SHD_ATTRIBUTE*)p;
            p += sizeof(S_SHD_ATTRIBUTE) * m_Header.numAttrs;
        }

        return p;
    }

    IZ_UINT8* CShaderAttrTable::SetInternalBuffer(IZ_UINT8* p)
    {
        if (m_Header.numAttrs > 0) {
            m_AttrHolder = (SAttributeHolder*)p;
            p += sizeof(SAttributeHolder) * m_Header.numAttrs;

            // Add to hash list.
            for (IZ_UINT i = 0; i < m_Header.numAttrs; i++) {
                m_AttrHolder[i].attr = &m_Attrs[i];
                m_AttrHolder[i].hashItem.Init(m_Attrs[i].keyName, &m_AttrHolder[i]);

                m_AttrHash.Add(&m_AttrHolder[i].hashItem);
            }
        }

        return p;
    }

    // Return attribute's description by index.
    const S_SHD_ATTRIBUTE* CShaderAttrTable::GetAttrByIdx(IZ_UINT idx) const
    {
        IZ_ASSERT(idx < m_Header.numAttrs);
        
        return &m_Attrs[idx];
    }

    // Return attribute's description by key.
    const S_SHD_ATTRIBUTE* CShaderAttrTable::GetAttrByKey(IZ_UINT key)
    {
        CStdHash<IZ_UINT, SAttributeHolder, SHD_ATTR_HASH_MAX>::Item* pItem = m_AttrHash.Find(key);
        IZ_ASSERT(pItem != IZ_NULL);

        S_SHD_ATTRIBUTE* ret = IZ_NULL;

        if (pItem != IZ_NULL) {
            SAttributeHolder* holder = pItem->GetData();
            ret = holder->attr;
        }
        
        return ret;
    }
}   // namespace shader
}   // namespace izanagi
