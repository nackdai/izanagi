#if !defined(__URANUS_MATH_MATH_FIXED_H__)
#define __URANUS_MATH_MATH_FIXED_H__

namespace uranus {
	/**
	* ŒÅ’è¬” 16bit
	*/
	struct SFixed16 {
		union {
			UN_UIN16 val;
			struct {
				UN_UINT16 sign_bit	: 1;	// •„†•”
				UN_UINT16 int_bit	: 8;	// ®”•”
				UN_UINT16 dec_bit	: 8;	// ¬”•”
			};
		};
	};

	/**
	* ŒÅ’è¬” 16bit
	*/
	class CFixed16 : public SFixed16 {
	public:
		CFixed16();
		CFixed16(UN_FLOAT f);
		CFixed16(UN_UINT16 n);
		CFixed16(const SFixed16& rhs);

		~CFixed16() {}

	public:
		const CFixed16& operator=(UN_FLOAT f);
		const CFixed16& operator=(UN_UINT16 n);
		const CFixed16& operator=(const SFixed16& rhs);

		UN_BOOL operator==(const SFixed16& rhs);
		UN_BOOL operator!=(const SFixed16& rhs);
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_MATH_MATH_FIXED_H__)
