#if !defined(__URANUS_STD_STD_STRING_H__)
#define __URANUS_STD_STD_STRING_H__

#include "unDefs.h"
#include "StdKey.h"

namespace uranus {
	/**
	*/
	template <typename _T, UN_UINT _NUM>
	class CStdString : public CPlacementNew {
	public:
		typedef _T STR_TYPE;

		enum {
			STR_NUM = _NUM,
		};

		static UN_BOOL IsValid(const _T* str)
		{
			// TODO
			size_t len = strlen(str);
			UN_BOOL ret = (len <= _NUM);
			return ret;
		}

	public:
		CStdString()
		{
			Clear();
		}
		CStdString(const _T* str)
		{
			SetString(str);
		}

		~CStdString() {}

		CStdString(const CStdString& rhs)
		{
			*this = rhs;
		}

		const CStdString& operator=(const CStdString& rhs)
		{
			SetString(rhs.m_String);
			return *this;
		}

	public:
		// 長さ取得
		UN_UINT GetLen() const
		{
			return _NUM;
		}

		// 文字列取得
		const _T* GetString() const
		{
			return m_String;
		}

		// キー値取得
		UN_UINT GetKeyValue() const
		{
			UN_UINT ret = CKey::GenerateValue(m_String);
			return ret;
		}

		// 文字列セット
		void SetString(const _T* str)
		{
			// TODO
			size_t nCpyLen = MIN(_NUM, strlen(str));
			memcpy(m_String, str, nCpyLen);

			memset(m_String + nCpyLen, 0, _NUM - nCpyLen);
		}

		// 文字列クリア
		void Clear()
		{
			FILL_ZERO(m_String, SIZE);
		}

		// 文字列がセットされているか
		UN_BOOL IsSetString() const
		{
			return (m_String[0] != '\0');
		}

		UN_BOOL operator==(const CStdString& rhs)
		{
			UN_BOOL ret = (memcmp(m_String, rhs.m_String, SIZE) == 0);
			return ret;
		}
		UN_BOOL operator==(const _T* rhs)
		{
			// TODO
			size_t nCmpLen = MIN(_NUM, strlen(rhs));
			UN_BOOL ret = (memcmp(m_String, rhs, nCmpLen) == 0);
			return ret;
		}

		UN_BOOL operator!=(const CStdString& rhs)
		{
			return !((*this) == rhs);
		}
		UN_BOOL operator!=(const _T* rhs)
		{
			return !((*this) == rhs);
		}

	protected:
		enum {
			SIZE = sizeof(_T) * (_NUM + 1),
		};

		_T m_String[_NUM + 1];

		// NOTE
		// 4の倍数であること
		UN_C_ASSERT(((_NUM + 1) & 0x03) == 0);
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_STD_STD_STRING_H__)
