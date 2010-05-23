#if !defined(__URANUS_STD_STD_VALUE_H__)
#define __URANUS_STD_STD_VALUE_H__

#include "unDefs.h"

namespace uranus {
	/**
	* ���Ԑ����������l�N���X
	*/
	class CValue {
	public:
		enum TYPE {
			TYPE_INT32,
			TYPE_UINT32,
			TYPE_FLOAT,
			TYPE_PTR,

			TYPE_NUM,
			TYPE_FORCE_INT32 = 0x7fffffff,
		};

	public:
		inline CValue();
		~CValue() {}

		CValue(UN_INT32 n) { SetValue(n); }
		CValue(UN_UINT32 n) { SetValue(n); }
		CValue(UN_FLOAT f) { SetValue(f); }
		CValue(void* p) { SetValue(p); }

		inline CValue(const CValue& rhs);
		inline const CValue& operator=(const CValue& rhs);

	public:
		// �^�C�v�擾
		inline TYPE GetType() const;

		// �l�擾
		template <typename _T>
		_T GetValue() const
		{
			return *(reinterpret_cast<_T*>(m_ValPtr));
		}

		// �l�擾
		inline UN_INT32 GetValueAsInt32() const;
		inline UN_UINT32 GetValueAsUInt32() const;
		inline UN_FLOAT GetValueAsFloat() const;
		inline void* GetValueAsPtr() const;
		
		// �l�Z�b�g
		inline void SetValue(UN_INT32 v);
		inline void SetValue(UN_UINT32 v);
		inline void SetValue(UN_FLOAT v);
		inline void SetValue(void* v);
		
	protected:
		TYPE m_Type;
		union {
			UN_INT32  m_nValInt32;
			UN_UINT32 m_nValUInt32;
			UN_FLOAT  m_fValFloat;
			void*     m_pValPtr;
		};
	};

	// �R���X�g���N�^
	CValue::CValue()
	{
		m_Type = TYPE_UINT32;
		m_nValInt32 = 0;
	}

	// �R�s�[�R���X�g���N�^
	CValue::CValue(const CValue& rhs)
	{
		*this = rhs;
	}

	// operator=
	const CValue& CValue::operator=(const CValue& rhs)
	{
		m_Type = rhs.m_Type;
		m_nValInt32 = rhs.m_nValInt32;
	}

	// �l�擾
	UN_INT32 CValue::GetValueAsInt32() const
	{
		return m_nValInt32;
	}

	UN_UINT32 CValue::GetValueAsUInt32() const
	{
		return m_nValUInt32;
	}

	UN_FLOAT CValue::GetValueAsFloat() const
	{
		return m_fValFloat;
	}

	void* CValue::GetValueAsPtr() const
	{
		return m_pValPtr;
	}

	// �l�Z�b�g
	void CValue::SetValue(UN_INT32 v)
	{
		m_Type = TYPE_INT32;
		m_nValInt32 = v;
	}

	void CValue::SetValue(UN_UINT32 v)
	{
		m_Type = TYPE_UINT32;
		m_nValUInt32 = v;
	}

	void CValue::SetValue(UN_FLOAT v)
	{
		m_Type = TYPE_FLOAT;
		m_fValFloat = v;
	}

	void CValue::SetValue(void* v)
	{
		m_Type = TYPE_PTR;
		m_pValPtr = v;
	}

}	// namespace uranus

#endif	// #if !defined(__URANUS_STD_STD_VALUE_H__)
