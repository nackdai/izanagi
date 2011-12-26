#if !defined(__IZANAGI_SHADER_SHADER_ATTR_TABLE_H__)
#define __IZANAGI_SHADER_SHADER_ATTR_TABLE_H__

#include "SHDFormat.h"

namespace izanagi {
	// Table of shader's attributes.
	class CShaderAttrTable {
		friend class CShaderBasic;

	private:
		inline CShaderAttrTable();
		~CShaderAttrTable() {}

		NO_COPIABLE(CShaderAttrTable);

	private:
		// Initialize table.
		inline IZ_UINT8* Init(IZ_UINT8* p);

		inline const S_SHD_ATTRIBUTE* GetAttrByIdx(IZ_UINT idx) const;
		inline const S_SHD_ATTRIBUTE* GetAttrByKey(IZ_UINT key);

	private:
		S_SHD_ATTR_HEADER m_Header;

		S_SHD_ATTRIBUTE* m_Attrs;

		CStdHash<IZ_UINT, S_SHD_ATTRIBUTE, SHD_ATTR_HASH_MAX> m_AttrHash;
	};

	// inline ***********************************

	CShaderAttrTable::CShaderAttrTable()
	{
		FILL_ZERO(&m_Header, sizeof(m_Header));
		m_Attrs = IZ_NULL;
	}

	// Initialize table.
	IZ_UINT8* CShaderAttrTable::Init(IZ_UINT8* p)
	{
		memcpy(&m_Header, p, sizeof(m_Header));
		p += sizeof(m_Header);

		if (m_Header.numAttrs > 0) {
			m_Attrs = (S_SHD_ATTRIBUTE*)p;
			p += sizeof(S_SHD_ATTRIBUTE) * m_Header.numAttrs;

			// Add to hash list.
			for (IZ_UINT i = 0; i < m_Header.numAttrs; i++) {
				m_Attrs[i].InitHash();
				m_AttrHash.Add(m_Attrs[i].GetHashItem());
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
		CStdHash<IZ_UINT, S_SHD_ATTRIBUTE, SHD_ATTR_HASH_MAX>::Item* pItem = m_AttrHash.Find(key);
		IZ_ASSERT(pItem != IZ_NULL);
		
		return (pItem != IZ_NULL ? pItem->GetData() : IZ_NULL);
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SHADER_SHADER_ATTR_TABLE_H__)
