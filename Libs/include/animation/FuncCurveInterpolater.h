#if !defined(__IZANAGI_ANM_FUNC_CURVE_INTERPOLATER_H__)
#define __IZANAGI_ANM_FUNC_CURVE_INTERPOLATER_H__

#include "izDefs.h"
#include "izStd.h"
#include "izMath.h"

namespace izanagi {
namespace animation {
	/**
	 */
	enum E_ANM_FUNC_CURVE_MODE
	{
		E_ANM_FUNC_CURVE_MODE_BEZIER = 0,   ///< ƒxƒWƒF
		E_ANM_FUNC_CURVE_MODE_HERMITE,      ///< Hermite
		E_ANM_FUNC_CURVE_MODE_CATMULL_ROM,  ///< Catmull-Rom

		E_ANM_FUNC_CURVE_MODE_NUM,
		E_ANM_FUNC_CURVE_MODE_FORCE_INT32 = 0x7fffffff,
	};

	class CTimeline;

	/**
	 */
	class CFuncCurveInterpolater {
	public:
		CFuncCurveInterpolater()
		{
			Init(
				E_ANM_FUNC_CURVE_MODE_BEZIER,
				0.0f, 0.0f,
				math::CVector(0.0f, 0.0f, 0.0f, 0.0f));
		}

		CFuncCurveInterpolater(
			E_ANM_FUNC_CURVE_MODE mode,
			IZ_FLOAT start,
			IZ_FLOAT end,
			const math::SVector& p)
		{
			Init(mode, start, end, p);
		}

		~CFuncCurveInterpolater() {}

		NO_COPIABLE(CFuncCurveInterpolater);

	public:
		IZ_FLOAT GetValue(const CTimeline& timeline);

		IZ_FLOAT GetValue(IZ_FLOAT t);

		void Init(
			E_ANM_FUNC_CURVE_MODE mode,
			IZ_FLOAT start,
			IZ_FLOAT end,
			IZ_FLOAT x0,
			IZ_FLOAT x1,
			IZ_FLOAT x2,
			IZ_FLOAT x3)
		{
			Init(
				mode,
				start, end,
				math::CVector(x0, x1, x2, x3));
		}

		void Init(
			E_ANM_FUNC_CURVE_MODE mode,
			IZ_FLOAT start,
			IZ_FLOAT end,
			const math::SVector& p)
		{
			SetRange(start, end);
			SetMode(mode);
			SetParam(p);
		}

		void SetRange(
			IZ_FLOAT start,
			IZ_FLOAT end)
		{
			m_Start = start;
			m_End = end;
		}

		void SetParam(
			IZ_FLOAT x0,
			IZ_FLOAT x1,
			IZ_FLOAT x2,
			IZ_FLOAT x3)
		{
			SetParam(math::CVector(x0, x1, x2, x3));
		}

		void SetParam(const math::SVector& p)
		{
			math::SVector::Copy(m_Param, p);
		}

		IZ_FLOAT GetStart() const { return m_Start; }
		IZ_FLOAT GetEnd() const { return m_End; }

		void SetMode(E_ANM_FUNC_CURVE_MODE mode) { m_Mode = mode; }
		E_ANM_FUNC_CURVE_MODE GetMode() const { return m_Mode; }

	protected:
		E_ANM_FUNC_CURVE_MODE m_Mode;

		IZ_FLOAT m_Start;
		IZ_FLOAT m_End;

		math::SVector m_Param;
	};

}	// namespace animation
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_ANM_FUNC_CURVE_INTERPOLATER_H__)
