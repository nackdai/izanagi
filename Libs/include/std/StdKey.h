#if !defined(__URANUS_STD_STD_KEY_H__)
#define __URANUS_STD_STD_KEY_H__

#include "unDefs.h"

namespace uranus {
	/**
	* 一意のキー値を扱うクラス
	*/
	class CKey {
	protected:
		static UN_UINT s_nKey;

	public:
		static CKey ZERO;

		// 新しいキーを生成する
		inline static CKey GenerateKey();
		inline static CKey GenerateKey(UN_PCSTR pszStr);

		// 新しいキー値を生成する
		inline static UN_UINT GenerateValue();
		inline static UN_UINT GenerateValue(UN_PCSTR pszStr);
		inline static UN_UINT64 GenerateValue64(UN_PCSTR pszStr);

	protected:
		template <typename _T>
		static _T GenerateValue(UN_PCSTR pszStr)
		{
			if (pszStr == NULL) {
				UN_ASSERT(UN_FALSE);
				return 0;
			}

			_T ret = *pszStr;
			_T k = 1;

			for (UN_UINT i = 0; ; i++) {
				if (pszStr[i] == '\0') {
					break;
				}
				ret += (pszStr[i] * (k *= 31L));
			}

			return ret;
		}

	public:
		inline CKey();
		inline CKey(UN_PCSTR pszStr);
		inline CKey(UN_UINT nKey);
		
		~CKey() {}

		inline const CKey& operator=(const CKey& rhs);

	protected:
		CKey(const CKey& rhs);

	public:
		inline UN_UINT GetValue() const;

		inline void SetValue(UN_PCSTR pszStr);
		inline void SetValue(UN_UINT nKey);

		inline UN_BOOL operator==(const CKey& rhs) const;
		inline UN_BOOL operator==(UN_PCSTR pszStr) const;
		inline UN_BOOL operator==(UN_UINT nKey) const;

		inline UN_BOOL operator!=(const CKey& rhs) const;
		inline UN_BOOL operator!=(UN_PCSTR pszStr) const;
		inline UN_BOOL operator!=(UN_UINT nKey) const;

		inline UN_UINT operator%(UN_UINT n) const;

	protected:
		UN_UINT m_nKey;;
	};

	/**
	* 新しいキー値を生成する
	*/
	UN_UINT CKey::GenerateValue()
	{
		return s_nKey++;
	}

	/**
	* 新しいキー値を生成する
	*/
	UN_UINT CKey::GenerateValue(UN_PCSTR pszStr)
	{
		UN_UINT ret = GenerateValue<UN_UINT>(pszStr);
		return ret;
	}

	/**
	* 新しいキー値を生成する
	*/
	UN_UINT64 CKey::GenerateValue64(UN_PCSTR pszStr)
	{
		UN_UINT64 ret = GenerateValue<UN_UINT64>(pszStr);
		return ret;
	}

	/**
	* 新しいキーを生成する
	*/
	CKey CKey::GenerateKey()
	{
		CKey ret;
		return ret;
	}

	/**
	* 新しいキーを生成する
	*/
	CKey CKey::GenerateKey(UN_PCSTR pszStr)
	{
		CKey ret(pszStr);
		return ret;
	}

	/**
	* コンストラクタ
	*/
	CKey::CKey()
	{
		m_nKey = 0;
	}

	/**
	* コンストラクタ
	*/
	CKey::CKey(UN_PCSTR pszStr)
	{
		m_nKey = GenerateValue(pszStr);
	}

	/**
	* コンストラクタ
	*/
	CKey::CKey(UN_UINT nKey)
	{
		m_nKey = nKey;
	}

	/**
	* operator =
	*/
	const CKey& CKey::operator=(const CKey& rhs)
	{
		m_nKey = rhs.m_nKey;
		return *this;
	}

	/**
	* キー値を取得
	*/
	UN_UINT CKey::GetValue() const
	{
		return m_nKey;
	}

	/**
	* キー値をセット
	*/
	void CKey::SetValue(UN_PCSTR pszStr)
	{
		m_nKey = GenerateValue(pszStr);
	}

	/**
	* キー値をセット
	*/
	void CKey::SetValue(UN_UINT nKey)
	{
		m_nKey = nKey;
	}

	/**
	* キーを比較する
	*/
	UN_BOOL CKey::operator==(const CKey& rhs) const
	{
		return (m_nKey == rhs.m_nKey);
	}

	/**
	* キーを比較する
	*/
	UN_BOOL CKey::operator==(UN_PCSTR pszStr) const
	{
		UN_UINT nKey = GenerateValue(pszStr);
		return (m_nKey == nKey);
	}

	/**
	* キーを比較する
	*/
	UN_BOOL CKey::operator==(UN_UINT nKey) const
	{
		return (m_nKey == nKey);
	}

	/**
	* キーを比較する
	*/
	UN_BOOL CKey::operator!=(const CKey& rhs) const
	{
		return !(*this == rhs);
	}

	/**
	* キーを比較する
	*/
	UN_BOOL CKey::operator!=(UN_PCSTR pszStr) const
	{
		return !(*this == pszStr);
	}

	/**
	* キーを比較する
	*/
	UN_BOOL CKey::operator!=(UN_UINT nKey) const
	{
		return !(*this == nKey);
	}

	UN_UINT CKey::operator%(UN_UINT n) const
	{
		return m_nKey % n;
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_STD_STD_KEY_H__)
