#if !defined(__IZANAGI_EFFECT_EFFECT_DEFS_H__)
#define __IZANAGI_EFFECT_EFFECT_DEFS_H__

namespace izanagi {
	/**
	*/
	enum E_EFFECT_STATE {
		E_EFFECT_STATE_ACTIVE = 0,
		E_EFFECT_STATE_WAIT,

		E_EFFECT_STATE_NUM,
		E_EFFECT_STATE_FORCE_INT32 = 0x7fffffff,
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_EFFECT_EFFECT_DEFS_H__)
