#if !defined(__URANUS_STD_STD_KEY_H__)
#define __URANUS_STD_STD_KEY_H__

#include "unDefs.h"

namespace uranus {
	/**
	* ��ӂ̃L�[�l�������N���X
	*/
	class CKey {
	protected:
		static UN_UINT s_nKey;

	public:
		static CKey ZERO;

		// �V�����L�[�𐶐�����
		inline static CKey GenerateKey();
		inline static CKey GenerateKey(UN_PCSTR pszStr);

		// �V�����L�[�l�𐶐�����
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
	* �V�����L�[�l�𐶐�����
	*/
	UN_UINT CKey::GenerateValue()
	{
		return s_nKey++;
	}

	/**
	* �V�����L�[�l�𐶐�����
	*/
	UN_UINT CKey::GenerateValue(UN_PCSTR pszStr)
	{
		UN_UINT ret = GenerateValue<UN_UINT>(pszStr);
		return ret;
	}

	/**
	* �V�����L�[�l�𐶐�����
	*/
	UN_UINT64 CKey::GenerateValue64(UN_PCSTR pszStr)
	{
		UN_UINT64 ret = GenerateValue<UN_UINT64>(pszStr);
		return ret;
	}

	/**
	* �V�����L�[�𐶐�����
	*/
	CKey CKey::GenerateKey()
	{
		CKey ret;
		return ret;
	}

	/**
	* �V�����L�[�𐶐�����
	*/
	CKey CKey::GenerateKey(UN_PCSTR pszStr)
	{
		CKey ret(pszStr);
		return ret;
	}

	/**
	* �R���X�g���N�^
	*/
	CKey::CKey()
	{
		m_nKey = 0;
	}

	/**
	* �R���X�g���N�^
	*/
	CKey::CKey(UN_PCSTR pszStr)
	{
		m_nKey = GenerateValue(pszStr);
	}

	/**
	* �R���X�g���N�^
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
	* �L�[�l���擾
	*/
	UN_UINT CKey::GetValue() const
	{
		return m_nKey;
	}

	/**
	* �L�[�l���Z�b�g
	*/
	void CKey::SetValue(UN_PCSTR pszStr)
	{
		m_nKey = GenerateValue(pszStr);
	}

	/**
	* �L�[�l���Z�b�g
	*/
	void CKey::SetValue(UN_UINT nKey)
	{
		m_nKey = nKey;
	}

	/**
	* �L�[���r����
	*/
	UN_BOOL CKey::operator==(const CKey& rhs) const
	{
		return (m_nKey == rhs.m_nKey);
	}

	/**
	* �L�[���r����
	*/
	UN_BOOL CKey::operator==(UN_PCSTR pszStr) const
	{
		UN_UINT nKey = GenerateValue(pszStr);
		return (m_nKey == nKey);
	}

	/**
	* �L�[���r����
	*/
	UN_BOOL CKey::operator==(UN_UINT nKey) const
	{
		return (m_nKey == nKey);
	}

	/**
	* �L�[���r����
	*/
	UN_BOOL CKey::operator!=(const CKey& rhs) const
	{
		return !(*this == rhs);
	}

	/**
	* �L�[���r����
	*/
	UN_BOOL CKey::operator!=(UN_PCSTR pszStr) const
	{
		return !(*this == pszStr);
	}

	/**
	* �L�[���r����
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
