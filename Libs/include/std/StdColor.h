#if !defined(__URANUS_STD_STD_COLOR_H__)
#define __URANUS_STD_STD_COLOR_H__

#include "unDefs.h"

namespace uranus {
	/**
	*/
	struct SColor {
		union {
			struct {
				UN_UINT8 a;
				UN_UINT8 r;
				UN_UINT8 g;
				UN_UINT8 b;
			};
			UN_UINT v;
		};
	};

	/**
	*/
	class CColor : public SColor {
	public:
		enum {
			WHITE = UN_COLOR_RGBA(0xff, 0xff, 0xff, 0xff),
			BLACK = UN_COLOR_RGBA(0x00, 0x00, 0x00, 0xff),
			RED   = UN_COLOR_RGBA(0xff, 0x00, 0x00, 0xff),
			GREEN = UN_COLOR_RGBA(0x00, 0xff, 0x00, 0xff),
			BLUE  = UN_COLOR_RGBA(0x00, 0x00, 0xff, 0xff),
			GRAY  = UN_COLOR_RGBA(0x80, 0x80, 0x80, 0xff),
		};

	public:
		inline CColor();
		inline CColor(UN_UINT8 _r, UN_UINT8 _g, UN_UINT8 _b, UN_UINT8 _a);
		inline CColor(UN_UINT _v);

		inline CColor(const SColor& rhs);
		inline CColor(const CColor& rhs);

	public:
		inline void SetRGBA(UN_UINT8 _r, UN_UINT8 _g, UN_UINT8 _b, UN_UINT8 _a);
		inline void SetRGBA(UN_UINT _v);

		inline void Set(const SColor& color);
		inline void Set(const CColor& color);

		inline void SetAlpha(UN_UINT8 _a);

		inline const CColor& operator=(UN_UINT _v);
		inline const CColor& operator=(const SColor& rhs);
		inline const CColor& operator=(const CColor& rhs);

		inline UN_BOOL operator==(const SColor& rhs) const;
		inline UN_BOOL operator==(const CColor& rhs) const;

		inline UN_BOOL operator!=(const SColor& rhs) const;
		inline UN_BOOL operator!=(const CColor& rhs) const;

		inline operator UN_COLOR() const;

	public:
		void SetYUV(UN_FLOAT _y, UN_FLOAT _u, UN_FLOAT _v);
		void GetYUV(UN_FLOAT* _y, UN_FLOAT* _u, UN_FLOAT* _v);

		void SetYCbCr(UN_FLOAT y, UN_FLOAT cb, UN_FLOAT cr);
		void GetYCbCr(UN_FLOAT* y, UN_FLOAT* cb, UN_FLOAT* cr);

		void SetYIQ(UN_FLOAT y, UN_FLOAT i, UN_FLOAT q);
		void GetYIQ(UN_FLOAT* y, UN_FLOAT* i, UN_FLOAT* q);

		void SetHLS(UN_FLOAT h, UN_FLOAT l, UN_FLOAT s);
		void GetHLS(UN_FLOAT* h, UN_FLOAT* l, UN_FLOAT* s);

		void SetCMY(UN_UINT8 c, UN_UINT8 m, UN_UINT8 y);
		void GetCMY(UN_UINT8* c, UN_UINT8* m, UN_UINT8* y);
	};

	// inline ******************************

	CColor::CColor()
	{
		v = WHITE;
	}

	CColor::CColor(UN_UINT8 _r, UN_UINT8 _g, UN_UINT8 _b, UN_UINT8 _a)
	{
		SetRGBA(_r, _g, _b, _a);
	}

	CColor::CColor(UN_UINT _v)
	{
		Set(_v);
	}

	CColor::CColor(const SColor& rhs)
	{
		Set(rhs);
	}

	CColor::CColor(const CColor& rhs)
	{
		Set(rhs);
	}

	void CColor::SetRGBA(UN_UINT8 _r, UN_UINT8 _g, UN_UINT8 _b, UN_UINT8 _a)
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}

	void CColor::SetRGBA(UN_UINT _v)
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

	void CColor::SetAlpha(UN_UINT8 _a)
	{
		a = _a;
	}

	const CColor& CColor::operator=(UN_UINT _v)
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

	UN_BOOL CColor::operator==(const SColor& rhs) const
	{
		return (v == rhs.v);
	}

	UN_BOOL CColor::operator==(const CColor& rhs) const
	{
		return (v == rhs.v);
	}

	UN_BOOL CColor::operator!=(const SColor& rhs) const
	{
		return !(*this == rhs);
	}

	UN_BOOL CColor::operator!=(const CColor& rhs) const
	{
		return !(*this == rhs);
	}

	CColor::operator UN_COLOR() const
	{
		return v;
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_STD_STD_COLOR_H__)
