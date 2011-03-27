#if !defined(__IZANAGI_EFFECT_EFFECT_SHAPE_H__)
#define __IZANAGI_EFFECT_EFFECT_SHAPE_H__

#include "izStd.h"
#include "izMath.h"

namespace izanagi {
	/**
	*/
	class CEffectShape : public CObject {
	protected:
		CEffectShape() {}
		virtual ~CEffectShape() {}

		NO_COPIABLE(CEffectShape);

	public:
		// Return random position that is in the shape.
		virtual void GetPos(SVector* pPos) = 0;

		// Return whether position is in the shape.
		virtual IZ_BOOL IsInShape(SVector* pPos) = 0;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_EFFECT_EFFECT_SHAPE_H__)
