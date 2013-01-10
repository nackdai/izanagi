#if !defined(__IZANAGI_STD_STD_KEY_H__)
#define __IZANAGI_STD_STD_KEY_H__

#include "izDefs.h"

namespace izanagi {
    /**
    * 一意のキー値を扱うクラス
    */
    class CKey {
    protected:
        static IZ_UINT s_nKey;

    public:
        static CKey ZERO;

        // 新しいキーを生成する
        inline static CKey GenerateKey();
        inline static CKey GenerateKey(IZ_PCSTR pszStr);

        // 新しいキー値を生成する
        inline static IZ_UINT GenerateValue();
        inline static IZ_UINT GenerateValue(IZ_PCSTR pszStr);
        inline static IZ_UINT64 GenerateValue64(IZ_PCSTR pszStr);

        inline static IZ_UINT GenerateValue(IZ_UINT* p, IZ_UINT num);
        inline static IZ_UINT64 GenerateValue64(IZ_UINT* p, IZ_UINT num);

    protected:
        template <typename _T>
        static _T GenerateValue(IZ_PCSTR pszStr)
        {
            if (pszStr == NULL) {
                IZ_ASSERT(IZ_FALSE);
                return 0;
            }

            _T ret = *pszStr;
            _T k = 1;

            for (IZ_UINT i = 0; ; ++i) {
                if (pszStr[i] == '\0') {
                    break;
                }
                ret += (pszStr[i] * (k *= 31L));
            }

            return ret;
        }

        template <typename _T, typename _U>
        static _T GenerateValue(_U* p, IZ_UINT num)
        {
            if (p == NULL) {
                IZ_ASSERT(IZ_FALSE);
                return 0;
            }

            IZ_BYTE* pp = reinterpret_cast<IZ_BYTE*>(p);
            num *= sizeof(_U);

            _T ret = *pp;
            _T k = 1;

            for (IZ_UINT i = 0; i < num; ++i) {
                ret += (pp[i] * (k *= 31L));
            }

            return ret;
        }

    public:
        inline CKey();
        inline CKey(IZ_PCSTR pszStr);
        inline CKey(IZ_UINT nKey);
        
        ~CKey() {}

        inline const CKey& operator=(const CKey& rhs);

    protected:
        CKey(const CKey& rhs) { IZ_ASSERT(IZ_FALSE); }

    public:
        inline IZ_UINT GetValue() const;

        inline void SetValue(IZ_PCSTR pszStr);
        inline void SetValue(IZ_UINT nKey);

        inline IZ_BOOL operator==(const CKey& rhs) const;
        inline IZ_BOOL operator==(IZ_PCSTR pszStr) const;
        inline IZ_BOOL operator==(IZ_UINT nKey) const;

        inline IZ_BOOL operator!=(const CKey& rhs) const;
        inline IZ_BOOL operator!=(IZ_PCSTR pszStr) const;
        inline IZ_BOOL operator!=(IZ_UINT nKey) const;

        inline IZ_UINT operator%(IZ_UINT n) const;

        operator IZ_UINT() const { return m_nKey; }

    protected:
        IZ_UINT m_nKey;;
    };

    /**
    * 新しいキー値を生成する
    */
    IZ_UINT CKey::GenerateValue()
    {
        return s_nKey++;
    }

    /**
    * 新しいキー値を生成する
    */
    IZ_UINT CKey::GenerateValue(IZ_PCSTR pszStr)
    {
        IZ_UINT ret = GenerateValue<IZ_UINT>(pszStr);
        return ret;
    }

    /**
    * 新しいキー値を生成する
    */
    IZ_UINT64 CKey::GenerateValue64(IZ_PCSTR pszStr)
    {
        IZ_UINT64 ret = GenerateValue<IZ_UINT64>(pszStr);
        return ret;
    }

    IZ_UINT CKey::GenerateValue(IZ_UINT* p, IZ_UINT num)
    {
        IZ_UINT ret = GenerateValue<IZ_UINT>(p, num);
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
    CKey CKey::GenerateKey(IZ_PCSTR pszStr)
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
    CKey::CKey(IZ_PCSTR pszStr)
    {
        m_nKey = GenerateValue(pszStr);
    }

    /**
    * コンストラクタ
    */
    CKey::CKey(IZ_UINT nKey)
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
    IZ_UINT CKey::GetValue() const
    {
        return m_nKey;
    }

    /**
    * キー値をセット
    */
    void CKey::SetValue(IZ_PCSTR pszStr)
    {
        m_nKey = GenerateValue(pszStr);
    }

    /**
    * キー値をセット
    */
    void CKey::SetValue(IZ_UINT nKey)
    {
        m_nKey = nKey;
    }

    /**
    * キーを比較する
    */
    IZ_BOOL CKey::operator==(const CKey& rhs) const
    {
        return (m_nKey == rhs.m_nKey);
    }

    /**
    * キーを比較する
    */
    IZ_BOOL CKey::operator==(IZ_PCSTR pszStr) const
    {
        IZ_UINT nKey = GenerateValue(pszStr);
        return (m_nKey == nKey);
    }

    /**
    * キーを比較する
    */
    IZ_BOOL CKey::operator==(IZ_UINT nKey) const
    {
        return (m_nKey == nKey);
    }

    /**
    * キーを比較する
    */
    IZ_BOOL CKey::operator!=(const CKey& rhs) const
    {
        return !(*this == rhs);
    }

    /**
    * キーを比較する
    */
    IZ_BOOL CKey::operator!=(IZ_PCSTR pszStr) const
    {
        return !(*this == pszStr);
    }

    /**
    * キーを比較する
    */
    IZ_BOOL CKey::operator!=(IZ_UINT nKey) const
    {
        return !(*this == nKey);
    }

    IZ_UINT CKey::operator%(IZ_UINT n) const
    {
        return m_nKey % n;
    }
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_STD_STD_KEY_H__)
