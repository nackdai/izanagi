#if !defined(__URANUS_FONT_FONT_DEFS_H__)
#define __URANUS_FONT_FONT_DEFS_H__

namespace uranus {
	/**
	*/
	enum E_FONT_CHAR_ENCODE {
		E_FONT_CHAR_ENCODE_SJIS = 0,	// SJIS
		E_FONT_CHAR_ENCODE_UTF16,		// UTF16
		E_FONT_CHAR_ENCODE_UTF8,		// UTF8

		E_FONT_CHAR_ENCODE_NUM,
		E_FONT_CHAR_ENCODE_FORCE_INT32 = 0x7fffffff,
	};
}	// enum uranus

#endif	// #if !defined(__URANUS_FONT_FONT_DEFS_H__)
