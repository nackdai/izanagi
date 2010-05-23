#include "std/animation/Tweener.h"
#include "math/MathDefs.h"

using namespace uranus;

/////////////////////////////////////////////////////////////////////////
// Linear

UN_FLOAT CTweenerLinear::EaseNone (UN_FLOAT t, UN_FLOAT d)
{
	return t / d;
}
UN_FLOAT CTweenerLinear::EaseIn (UN_FLOAT t, UN_FLOAT d)
{
	return t / d;
}
UN_FLOAT CTweenerLinear::EaseOut(UN_FLOAT t, UN_FLOAT d)
{
	return t / d;
}

UN_FLOAT CTweenerLinear::EaseInOut(UN_FLOAT t, UN_FLOAT d)
{
	return t / d;
}

/////////////////////////////////////////////////////////////////////////
// Sine

UN_FLOAT CTweenerSine::EaseIn (UN_FLOAT t, UN_FLOAT d)
{
	return -1.0f * cos(t / d * UN_MATH_PI1_2) + 1.0f;
}
UN_FLOAT CTweenerSine::EaseOut(UN_FLOAT t, UN_FLOAT d)
{
	return sin(t / d * UN_MATH_PI1_2);
}

UN_FLOAT CTweenerSine::EaseInOut(UN_FLOAT t, UN_FLOAT d)
{
	return -0.5f * (cos(UN_MATH_PI * t / d) - 1);
}

/////////////////////////////////////////////////////////////////////////
// Quint

UN_FLOAT CTweenerQuint::EaseIn (UN_FLOAT t, UN_FLOAT d)
{
	t /= d;
	return t * t * t * t * t;
}
UN_FLOAT CTweenerQuint::EaseOut(UN_FLOAT t, UN_FLOAT d)
{
	t = t / d - 1;
	return t * t * t * t * t + 1;
}

UN_FLOAT CTweenerQuint::EaseInOut(UN_FLOAT t, UN_FLOAT d)
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

UN_FLOAT CTweenerQuart::EaseIn (UN_FLOAT t, UN_FLOAT d)
{
	t /= d;
	return t * t * t * t;
}
UN_FLOAT CTweenerQuart::EaseOut(UN_FLOAT t, UN_FLOAT d)
{
	t = t / d - 1;
	return -1.0f * (t * t * t * t - 1);
}

UN_FLOAT CTweenerQuart::EaseInOut(UN_FLOAT t, UN_FLOAT d)
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

UN_FLOAT CTweenerQuad::EaseIn (UN_FLOAT t, UN_FLOAT d)
{
	t /= d;
	return t * t;
}
UN_FLOAT CTweenerQuad::EaseOut(UN_FLOAT t, UN_FLOAT d)
{
	t /= d;
	return -1.0f * t * (t - 2);
}

UN_FLOAT CTweenerQuad::EaseInOut(UN_FLOAT t, UN_FLOAT d)
{
	t /= d * 0.5f;

	if (t < 1) {
		return 0.5f * t * t;
	}

	return -0.5f * t * (t -2);
}

/////////////////////////////////////////////////////////////////////////
// Expo

UN_FLOAT CTweenerExpo::EaseIn (UN_FLOAT t, UN_FLOAT d)
{
	return (t == 0
			? 0.0f
			: pow(2, 10 * (t / d - 1)));
}
UN_FLOAT CTweenerExpo::EaseOut(UN_FLOAT t, UN_FLOAT d)
{
	return (t == d
			? 1.0f
			: -pow(2, -10 * t / d) + 1);
}

UN_FLOAT CTweenerExpo::EaseInOut(UN_FLOAT t, UN_FLOAT d)
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

UN_FLOAT CTweenerElastic::EaseIn (UN_FLOAT t, UN_FLOAT d)
{
	if (t == 0) {
		return 0.0f;
	}

	t /= d;

	if (t == 1) {
		return 1.0f;
	}

	UN_FLOAT p = d * 0.3f;
	UN_FLOAT s = p * 0.25f;

	t -= 1;

	return -pow(2, 10 * t) * sin((t * d - s) * UN_MATH_PI2 / p);
}

UN_FLOAT CTweenerElastic::EaseOut(UN_FLOAT t, UN_FLOAT d)
{
	if (t == 0) {
		return 0.0f;  
	}
	
	t /= d;

	if (t == 1) {
		return 1.0f;
	}

	UN_FLOAT p = d * 0.3f;
	UN_FLOAT s = p * 0.25f;

	return pow(2, -10 * t) * sin((t * d - s) * UN_MATH_PI2 / p) + 1.0f;
}

UN_FLOAT CTweenerElastic::EaseInOut(UN_FLOAT t, UN_FLOAT d)
{
	if (t == 0) {
		return 0.0f;
	}
	
	t /= d * 0.5f;

	if (t == 2) {
		return 1.0f;
	}

	UN_FLOAT p = d * (0.3f * 1.5f);
	UN_FLOAT s = p * 0.25f;

	if (t < 1) 	{
		t -= 1;
		return -0.5f * (pow(2, 10 * t) * sin((t * d - s) * UN_MATH_PI2 / p));
	}
	else {
		t -= 1;
		return pow(2, -10 * t) * sin((t * d - s) * UN_MATH_PI2 / p) * 0.5f + 1.0f;
	}
}

/////////////////////////////////////////////////////////////////////////
// Cubic

UN_FLOAT CTweenerCubic::EaseIn (UN_FLOAT t, UN_FLOAT d)
{
	t /= d;
	return t * t * t;
}

UN_FLOAT CTweenerCubic::EaseOut(UN_FLOAT t, UN_FLOAT d)
{
	t = t / d - 1;
	return t * t * t + 1;
}

UN_FLOAT CTweenerCubic::EaseInOut(UN_FLOAT t, UN_FLOAT d)
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

UN_FLOAT CTweenerCirc::EaseIn (UN_FLOAT t, UN_FLOAT d)
{
	t /= d;
	return -1.0f * (sqrt(1 - t * t) - 1);
}

UN_FLOAT CTweenerCirc::EaseOut(UN_FLOAT t, UN_FLOAT d)
{
	t = t /d - 1;
	return sqrt(1 - t * t);
}

UN_FLOAT CTweenerCirc::EaseInOut(UN_FLOAT t, UN_FLOAT d)
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

UN_FLOAT CTweenerBounce::EaseIn (UN_FLOAT t, UN_FLOAT d)
{
	return EaseOut(d - t, d);
}

UN_FLOAT CTweenerBounce::EaseOut(UN_FLOAT t, UN_FLOAT d)
{
	static const UN_FLOAT threshold_0 = 1.0f / 2.75f;
	static const UN_FLOAT threshold_1 = 2.0f / 2.75f;
	static const UN_FLOAT threshold_2 = 2.5f / 2.75f;

	static const UN_FLOAT val_0 = 1.5f / 2.75f;
	static const UN_FLOAT val_1 = 2.25f / 2.75f;
	static const UN_FLOAT val_2 = 2.625f / 2.75f;

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

UN_FLOAT CTweenerBounce::EaseInOut(UN_FLOAT t, UN_FLOAT d)
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

UN_FLOAT CTweenerBack::EaseIn (UN_FLOAT t, UN_FLOAT d)
{
	t /= d;
	return t * t * ((1.70158f + 1) * t - 1.70158f);
}

UN_FLOAT CTweenerBack::EaseOut(UN_FLOAT t, UN_FLOAT d)
{
	t = t / d - 1;
	return t * t * ((1.70158f + 1) * t + 1.70158f) + 1;
}

UN_FLOAT CTweenerBack::EaseInOut(UN_FLOAT t, UN_FLOAT d)
{
	static const UN_FLOAT s = 1.70158f * 1.525f;

	t /= d * 0.5f;

	if (t < 1) {
		return 0.5f * (t * t * ((s + 1) * t - s));
	}

	t -= 2;

	return 0.5f * (t * t * ((s + 1) * t + s) + 2);
}
