#if !defined(__IZANAGI_STD_STD_BIT_FLAG_H__)
#define __IZANAGI_STD_STD_BIT_FLAG_H__

#include "izDefs.h"

namespace {
	template <typename _T>
	class CBitFlag {
	public:
		CBitFlag() { m_Body = 0; }
		CBitFlag(_T val) { m_Body = val; }
		~CBitFlag() {}

		CBitFlag(const CBitFlag& rhs) { m_Body = rhs.m_Body; }
		const CBitFlag& operator=(const CBitFlag& rhs)
		{
			m_Body = rhs.m_Body;
			return *this;
		}

	public:
		void Set(_T val) { m_Body = val; }
		_T Get() const { return m_Body; }

		void Clear() { m_Body = 0; }

		IZ_BOOL IsOn(_T mask) const { return (m_Body & mask); }
		IZ_BOOL IsOff(_T mask) const { return !IsOn(mask); }

		IZ_BOOL IsOnEx(IZ_UINT bit) const
		{
			IZ_ASSERT(bit < COUNT_BIT(_T));
			return IsOn(1 << bit);
		}
		IZ_BOOL IsOffEx(IZ_UINT bit) const { return !IsOnEx(bit); }

		void OnFlag(_T mask) const { m_Body |= mask; }
		void OffFlag(_T mask) const { m_Body &= ~mask; }

		void OnFlagEx(IZ_UINT bit) const
		{
			IZ_ASSERT(bit < COUNT_BIT(_T));
			m_Body |= (1 << bit);
		}
		void OffFlagEx(IZ_UINT bit) const
		{
			IZ_ASSERT(bit < COUNT_BIT(_T));
			m_Body &= ~(1 << bit);
		}

	private:
		_T m_Body;
	};

	typedef CBitFlag<IZ_UINT32>	CBit32Flag;
	typedef CBitFlag<IZ_UINT16>	CBit16Flag;
	typedef CBitFlag<IZ_UINT8>	CBit8Flag;
}	// namespace
#endif	// #if !defined(__IZANAGI_STD_STD_BIT_FLAG_H__)
