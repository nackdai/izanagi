#if !defined(__IZANAGI_STD_STD_STRING_H__)
#define __IZANAGI_STD_STD_STRING_H__

#include "izDefs.h"
#include "StdKey.h"

namespace izanagi {
    /**
    */
    template <typename _T, IZ_UINT _NUM>
    class CStdString {
    public:
        typedef _T STR_TYPE;

        enum {
            STR_NUM = _NUM,
        };

        static IZ_BOOL IsValid(const _T* str)
        {
            // TODO
            size_t len = strlen(str);
            IZ_BOOL ret = (len <= _NUM);
            return ret;
        }

    public:
        CStdString()
        {
            //Clear();
            m_String[0] = 0;
        }
        CStdString(const _T* str)
        {
            if (str != IZ_NULL) {
                SetString(str);
            }
            else {
                Clear();
            }
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
        IZ_UINT GetLen() const
        {
            return _NUM;
        }

        // 文字列取得
        const _T* GetString() const
        {
            return m_String;
        }

        // キー値取得
        IZ_UINT GetKeyValue() const
        {
            IZ_UINT ret = CKey::GenerateValue(m_String);
            return ret;
        }

        // 文字列セット
        void SetString(const _T* str)
        {
            size_t nCpyLen = IZ_MIN(_NUM, strlen(str));
            memset(m_String, 0, SIZE);
            memcpy(m_String, str, nCpyLen);
        }

        // 文字列クリア
        void Clear()
        {
            FILL_ZERO(m_String, SIZE);
        }

        // 文字列がセットされているか
        IZ_BOOL IsSetString() const
        {
            return (m_String[0] != '\0');
        }

        IZ_BOOL operator==(const CStdString& rhs)
        {
            size_t nCmpLen = strlen(m_String);
            IZ_BOOL ret = (memcmp(m_String, rhs.m_String, nCmpLen) == 0);
            return ret;
        }
        IZ_BOOL operator==(const _T* rhs)
        {
            return (*this == CStdString(rhs));
        }

        IZ_BOOL operator!=(const CStdString& rhs)
        {
            return !((*this) == rhs);
        }
        IZ_BOOL operator!=(const _T* rhs)
        {
            return !((*this) == rhs);
        }

        IZ_UINT operator%(IZ_UINT rhs) const
        {
            IZ_UINT nKey = GetKeyValue();
            return nKey % rhs;
        }

        const CStdString<_T, _NUM>& operator=(const _T* rhs)
        {
            SetString(rhs);
            return *this;
        }

    protected:
        enum {
            SIZE = sizeof(_T) * (_NUM + 1),
        };

        _T m_String[_NUM + 1];

        // NOTE
        // 4の倍数であること
        IZ_C_ASSERT(((_NUM + 1) & 0x03) == 0);
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_STD_STD_STRING_H__)
