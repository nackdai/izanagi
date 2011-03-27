#if !defined(__CHAR_LIST_H__)
#define __CHAR_LIST_H__

#include <set>
#include <vector>
#include "FontConverterDefs.h"
#include "izFont.h"
#include "ToolKitIoDefs.h"

class IInputStream {
public:
	IInputStream() {}
	virtual ~IInputStream() {}

	virtual IZ_INT Get() = 0;
};

// キャラクタリスト
class CCharList {
private:
	static CCharList s_cInstance;

public:
	static CCharList& GetInstance() { return s_cInstance; }

private:
	CCharList() { m_nCharFmt = izanagi::E_FONT_CHAR_ENCODE_SJIS; }
	~CCharList() {}

	NO_COPIABLE(CCharList);

public:
	// Registers characters from a file.
	void Register(
		izanagi::E_FONT_CHAR_ENCODE nCharFmt,
		izanagi::IInputStream* pIn);

	// Returns character code bytes by character format.
	IZ_UINT GetCharBytes(IZ_UINT code);

private:
	// Registers characters as SJIS.
	void RegisterSJIS(IInputStream* pIn);

	// Registers characters as UTF16.
	void RegisterUTF16(IInputStream* pIn);

	// Registers characters as UTF8.
	void RegisterUTF8(IInputStream* pIn);

	// Returns character code bytes as SJIS
	IZ_UINT GetCharBytesAsSJIS(IZ_UINT code);

	// Returns character code bytes as UTF16
	IZ_UINT GetCharBytesAsUTF16(IZ_UINT code);

	// Returns character code bytes as UTF8
	IZ_UINT GetCharBytesAsUTF8(IZ_UINT code);

public:
	const std::set<IZ_UINT>& GetCharList() const { return m_CharList; }
	IZ_UINT GetCharNum() const { return static_cast<IZ_UINT>(m_CharList.size()); }

protected:
	izanagi::E_FONT_CHAR_ENCODE m_nCharFmt;
	std::set<IZ_UINT> m_CharList;
};

#endif	// #if !defined(__CHAR_LIST_H__)
