#include "math/Tweener.h"
#include "math/MathDefs.h"

namespace izanagi {
namespace math {

	/////////////////////////////////////////////////////////////////////////
	// Linear

	IZ_FLOAT CTweenerLinear::EaseNone (IZ_FLOAT t, IZ_FLOAT d)
	{
		return t / d;
	}
	IZ_FLOAT CTweenerLinear::EaseIn (IZ_FLOAT t, IZ_FLOAT d)
	{
		return t / d;
	}
	IZ_FLOAT CTweenerLinear::EaseOut(IZ_FLOAT t, IZ_FLOAT d)
	{
		return t / d;
	}

	IZ_FLOAT CTweenerLinear::EaseInOut(IZ_FLOAT t, IZ_FLOAT d)
	{
		return t / d;
	}

	/////////////////////////////////////////////////////////////////////////
	// Sine

	IZ_FLOAT CTweenerSine::EaseIn (IZ_FLOAT t, IZ_FLOAT d)
	{
		return -1.0f * cos(t / d * IZ_MATH_PI1_2) + 1.0f;
	}
	IZ_FLOAT CTweenerSine::EaseOut(IZ_FLOAT t, IZ_FLOAT d)
	{
		return sin(t / d * IZ_MATH_PI1_2);
	}

	IZ_FLOAT CTweenerSine::EaseInOut(IZ_FLOAT t, IZ_FLOAT d)
	{
		return -0.5f * (cos(IZ_MATH_PI * t / d) - 1);
	}

	/////////////////////////////////////////////////////////////////////////
	// Quint

	IZ_FLOAT CTweenerQuint::EaseIn (IZ_FLOAT t, IZ_FLOAT d)
	{
		t /= d;
		return t * t * t * t * t;
	}
	IZ_FLOAT CTweenerQuint::EaseOut(IZ_FLOAT t, IZ_FLOAT d)
	{
		t = t / d - 1;
		return t * t * t * t * t + 1;
	}

	IZ_FLOAT CTweenerQuint::EaseInOut(IZ_FLOAT t, IZ_FLOAT d)
	{
		t /= d * 0.5f;

		if (t < 1) {
			return 0.5f * t * t * t * t * t;
		}

		t -= 2;
		return 0.5f * (t * t * t * t * t + 2);
	}

	/////////////////////////////////////////////////////////////////////////
	// Quart

	IZ_FLOAT CTweenerQuart::EaseIn (IZ_FLOAT t, IZ_FLOAT d)
	{
		t /= d;
		return t * t * t * t;
	}
	IZ_FLOAT CTweenerQuart::EaseOut(IZ_FLOAT t, IZ_FLOAT d)
	{
		t = t / d - 1;
		return -1.0f * (t * t * t * t - 1);
	}

	IZ_FLOAT CTweenerQuart::EaseInOut(IZ_FLOAT t, IZ_FLOAT d)
	{
		t /= d * 0.5f;

		if (t < 1) {
			return 0.5f * t * t * t * t;
		}

		t -= 2;
		return -0.5f * (t * t * t * t - 2);
	}

	/////////////////////////////////////////////////////////////////////////
	// Quad

	IZ_FLOAT CTweenerQuad::EaseIn (IZ_FLOAT t, IZ_FLOAT d)
	{
		t /= d;
		return t * t;
	}
	IZ_FLOAT CTweenerQuad::EaseOut(IZ_FLOAT t, IZ_FLOAT d)
	{
		t /= d;
		return -1.0f * t * (t - 2);
	}

	IZ_FLOAT CTweenerQuad::EaseInOut(IZ_FLOAT t, IZ_FLOAT d)
	{
		t /= d * 0.5f;

		if (t < 1) {
			return 0.5f * t * t;
		}

		return -0.5f * t * (t -2);
	}

	/////////////////////////////////////////////////////////////////////////
	// Expo

	IZ_FLOAT CTweenerExpo::EaseIn (IZ_FLOAT t, IZ_FLOAT d)
	{
		return (t == 0
				? 0.0f
				: pow(2, 10 * (t / d - 1)));
	}
	IZ_FLOAT CTweenerExpo::EaseOut(IZ_FLOAT t, IZ_FLOAT d)
	{
		return (t == d
				? 1.0f
				: -pow(2, -10 * t / d) + 1);
	}

	IZ_FLOAT CTweenerExpo::EaseInOut(IZ_FLOAT t, IZ_FLOAT d)
	{
		if (t == 0) {
			return 0.0f;
		}
		if (t == d) {
			return 1.0f;
		}

		t /= d * 0.5f;

		if (t < 1) {
			return 0.5f * pow(2, 10 * (t - 1));
		}

		t -= 1;
		return 0.5f * (-pow(2, -10 * t) + 2);
	}

	/////////////////////////////////////////////////////////////////////////
	// Elastic

	IZ_FLOAT CTweenerElastic::EaseIn (IZ_FLOAT t, IZ_FLOAT d)
	{
		if (t == 0) {
			return 0.0f;
		}

		t /= d;

		if (t == 1) {
			return 1.0f;
		}

		IZ_FLOAT p = d * 0.3f;
		IZ_FLOAT s = p * 0.25f;

		t -= 1;

		return -pow(2, 10 * t) * sin((t * d - s) * IZ_MATH_PI2 / p);
	}

	IZ_FLOAT CTweenerElastic::EaseOut(IZ_FLOAT t, IZ_FLOAT d)
	{
		if (t == 0) {
			return 0.0f;  
		}
		
		t /= d;

		if (t == 1) {
			return 1.0f;
		}

		IZ_FLOAT p = d * 0.3f;
		IZ_FLOAT s = p * 0.25f;

		return pow(2, -10 * t) * sin((t * d - s) * IZ_MATH_PI2 / p) + 1.0f;
	}

	IZ_FLOAT CTweenerElastic::EaseInOut(IZ_FLOAT t, IZ_FLOAT d)
	{
		if (t == 0) {
			return 0.0f;
		}
		
		t /= d * 0.5f;

		if (t == 2) {
			return 1.0f;
		}

		IZ_FLOAT p = d * (0.3f * 1.5f);
		IZ_FLOAT s = p * 0.25f;

		if (t < 1) 	{
			t -= 1;
			return -0.5f * (pow(2, 10 * t) * sin((t * d - s) * IZ_MATH_PI2 / p));
		}
		else {
			t -= 1;
			return pow(2, -10 * t) * sin((t * d - s) * IZ_MATH_PI2 / p) * 0.5f + 1.0f;
		}
	}

	/////////////////////////////////////////////////////////////////////////
	// Cubic

	IZ_FLOAT CTweenerCubic::EaseIn (IZ_FLOAT t, IZ_FLOAT d)
	{
		t /= d;
		return t * t * t;
	}

	IZ_FLOAT CTweenerCubic::EaseOut(IZ_FLOAT t, IZ_FLOAT d)
	{
		t = t / d - 1;
		return t * t * t + 1;
	}

	IZ_FLOAT CTweenerCubic::EaseInOut(IZ_FLOAT t, IZ_FLOAT d)
	{
		t /= d * 0.5f;
		if (t < 1) {
			return 0.5f * t * t * t;
		}

		t -= 2;
		return 0.5f * (t * t * t + 2);
	}

	/////////////////////////////////////////////////////////////////////////
	// Circ

	IZ_FLOAT CTweenerCirc::EaseIn (IZ_FLOAT t, IZ_FLOAT d)
	{
		t /= d;
		return -1.0f * (sqrt(1 - t * t) - 1);
	}

	IZ_FLOAT CTweenerCirc::EaseOut(IZ_FLOAT t, IZ_FLOAT d)
	{
		t = t /d - 1;
		return sqrt(1 - t * t);
	}

	IZ_FLOAT CTweenerCirc::EaseInOut(IZ_FLOAT t, IZ_FLOAT d)
	{
		t /= d * 0.5f;

		if (t < 1) {
			return -0.5f * (sqrt(1 - t * t) - 1);
		}

		t -= 2;

		return 0.5f * (sqrt(1 - t * t) + 1);
	}

	/////////////////////////////////////////////////////////////////////////
	// Bounce

	IZ_FLOAT CTweenerBounce::EaseIn (IZ_FLOAT t, IZ_FLOAT d)
	{
		return EaseOut(d - t, d);
	}

	IZ_FLOAT CTweenerBounce::EaseOut(IZ_FLOAT t, IZ_FLOAT d)
	{
		static const IZ_FLOAT threshold_0 = 1.0f / 2.75f;
		static const IZ_FLOAT threshold_1 = 2.0f / 2.75f;
		static const IZ_FLOAT threshold_2 = 2.5f / 2.75f;

		static const IZ_FLOAT val_0 = 1.5f / 2.75f;
		static const IZ_FLOAT val_1 = 2.25f / 2.75f;
		static const IZ_FLOAT val_2 = 2.625f / 2.75f;

		t /= d;

		if (t < threshold_0) {
			return 7.5625f * t * t;
		}
		else if (t < threshold_1) {
			t -= val_0;
			return 7.5625f * t * t + 0.75f;
		}
		else if (t < threshold_2) {
			t -= val_1;
			return 7.5625f * t * t + 0.9375f;
		}
		else {
			t -= val_2;
			return 7.5625f * t * t + 0.984375f;
		}
	}

	IZ_FLOAT CTweenerBounce::EaseInOut(IZ_FLOAT t, IZ_FLOAT d)
	{
		if (t < d * 0.5f) {
			return EaseIn(t * 2, d) * 0.5f;
		}
		else {
			return EaseOut(t * 2 - d, d) * 0.5f + 1.0f * 0.5f;
		}
	}

	/////////////////////////////////////////////////////////////////////////
	// Back

	IZ_FLOAT CTweenerBack::EaseIn (IZ_FLOAT t, IZ_FLOAT d)
	{
		t /= d;
		return t * t * ((1.70158f + 1) * t - 1.70158f);
	}

	IZ_FLOAT CTweenerBack::EaseOut(IZ_FLOAT t, IZ_FLOAT d)
	{
		t = t / d - 1;
		return t * t * ((1.70158f + 1) * t + 1.70158f) + 1;
	}

	IZ_FLOAT CTweenerBack::EaseInOut(IZ_FLOAT t, IZ_FLOAT d)
	{
		static const IZ_FLOAT s = 1.70158f * 1.525f;

		t /= d * 0.5f;

		if (t < 1) {
			return 0.5f * (t * t * ((s + 1) * t - s));
		}

		t -= 2;

		return 0.5f * (t * t * ((s + 1) * t + s) + 2);
	}

}	// namespace math
}	// namespace izanagi