#if !defined(__IZANAGI_STD_STD_COLOR_H__)
#define __IZANAGI_STD_STD_COLOR_H__

#include "izDefs.h"

namespace izanagi {
    /**
    */
    struct SColor {
        union {
            struct {
                IZ_UINT8 a;
                IZ_UINT8 r;
                IZ_UINT8 g;
                IZ_UINT8 b;
            };
            IZ_UINT v;
        };
    };

    /**
    */
    class CColor : public SColor {
    public:
        enum {
            WHITE = IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff),
            BLACK = IZ_COLOR_RGBA(0x00, 0x00, 0x00, 0xff),
            RED   = IZ_COLOR_RGBA(0xff, 0x00, 0x00, 0xff),
            GREEN = IZ_COLOR_RGBA(0x00, 0xff, 0x00, 0xff),
            BLUE  = IZ_COLOR_RGBA(0x00, 0x00, 0xff, 0xff),
            GRAY  = IZ_COLOR_RGBA(0x80, 0x80, 0x80, 0xff),
        };

    public:
        inline CColor();
        inline CColor(IZ_UINT8 _r, IZ_UINT8 _g, IZ_UINT8 _b, IZ_UINT8 _a);
        inline CColor(IZ_UINT _v);

        inline CColor(const SColor& rhs);
        inline CColor(const CColor& rhs);

    public:
        inline void SetRGBA(IZ_UINT8 _r, IZ_UINT8 _g, IZ_UINT8 _b, IZ_UINT8 _a);
        inline void SetRGBA(IZ_UINT _v);

        inline void Set(const SColor& color);
        inline void Set(const CColor& color);

        inline void SetAlpha(IZ_UINT8 _a);

        inline const CColor& operator=(IZ_UINT _v);
        inline const CColor& operator=(const SColor& rhs);
        inline const CColor& operator=(const CColor& rhs);

        inline IZ_BOOL operator==(const SColor& rhs) const;
        inline IZ_BOOL operator==(const CColor& rhs) const;

        inline IZ_BOOL operator!=(const SColor& rhs) const;
        inline IZ_BOOL operator!=(const CColor& rhs) const;

        inline operator IZ_COLOR() const;

    public:
        void SetYUV(IZ_FLOAT _y, IZ_FLOAT _u, IZ_FLOAT _v);
        void GetYUV(IZ_FLOAT* _y, IZ_FLOAT* _u, IZ_FLOAT* _v);

        void SetYCbCr(IZ_FLOAT y, IZ_FLOAT cb, IZ_FLOAT cr);
        void GetYCbCr(IZ_FLOAT* y, IZ_FLOAT* cb, IZ_FLOAT* cr);

        void SetYIQ(IZ_FLOAT y, IZ_FLOAT i, IZ_FLOAT q);
        void GetYIQ(IZ_FLOAT* y, IZ_FLOAT* i, IZ_FLOAT* q);

        void SetHLS(IZ_FLOAT h, IZ_FLOAT l, IZ_FLOAT s);
        void GetHLS(IZ_FLOAT* h, IZ_FLOAT* l, IZ_FLOAT* s);

        void SetCMY(IZ_UINT8 c, IZ_UINT8 m, IZ_UINT8 y);
        void GetCMY(IZ_UINT8* c, IZ_UINT8* m, IZ_UINT8* y);
    };

    // inline ******************************

    CColor::CColor()
    {
        v = WHITE;
    }

    CColor::CColor(IZ_UINT8 _r, IZ_UINT8 _g, IZ_UINT8 _b, IZ_UINT8 _a)
    {
        SetRGBA(_r, _g, _b, _a);
    }

    CColor::CColor(IZ_UINT _v)
    {
        v = _v;
    }

    CColor::CColor(const SColor& rhs)
    {
        Set(rhs);
    }

    CColor::CColor(const CColor& rhs)
    {
        Set(rhs);
    }

    void CColor::SetRGBA(IZ_UINT8 _r, IZ_UINT8 _g, IZ_UINT8 _b, IZ_UINT8 _a)
    {
        r = _r;
        g = _g;
        b = _b;
        a = _a;
    }

    void CColor::SetRGBA(IZ_UINT _v)
    {
        v = _v;
    }

    void CColor::Set(const SColor& color)
    {
        v = color.v;
    }

    void CColor::Set(const CColor& color)
    {
        v = color.v;
    }

    void CColor::SetAlpha(IZ_UINT8 _a)
    {
        a = _a;
    }

    const CColor& CColor::operator=(IZ_UINT _v)
    {
        Set(_v);
    }

    const CColor& CColor::operator=(const SColor& rhs)
    {
        Set(rhs);
    }

    const CColor& CColor::operator=(const CColor& rhs)
    {
        Set(rhs);
    }

    IZ_BOOL CColor::operator==(const SColor& rhs) const
    {
        return (v == rhs.v);
    }

    IZ_BOOL CColor::operator==(const CColor& rhs) const
    {
        return (v == rhs.v);
    }

    IZ_BOOL CColor::operator!=(const SColor& rhs) const
    {
        return !(*this == rhs);
    }

    IZ_BOOL CColor::operator!=(const CColor& rhs) const
    {
        return !(*this == rhs);
    }

    CColor::operator IZ_COLOR() const
    {
        return v;
    }
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_STD_STD_COLOR_H__)
