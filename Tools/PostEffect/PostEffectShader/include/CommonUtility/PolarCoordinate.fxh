#if !defined(__KSG2_POSTEFFECT_POLAR_COORDINATE_FXH__)
#define __KSG2_POSTEFFECT_POLAR_COORDINATE_FXH__

// �������W <=> �ɍ��W

#define _PI	3.141592f

/////////////////////////////////////////////
// �������W => �ɍ��W
float2 CartesianToPolar(float2 vIn)
{
	float2 vOut = vIn * 2.0f - 1.0f;
	vOut.y *= -1.0f;

	float fR = 0.5f - vOut.y * 0.5f;
	float fTheta = -vOut.x * _PI;

	float fS, fC;
	sincos(fTheta, fS, fC);

	vOut.x = -fR * fS;
	vOut.y = fR * fC;

	vOut = vOut * 0.5f + 0.5f;

	return vOut;
}

/////////////////////////////////////////////
// �ɍ��W => �������W
float2 PolarToCartesian(float2 vIn)
{
	float2 vOut = vIn * 2.0f - 1.0f;

	float fTheta = atan2(vOut.x, vOut.y);
	float fR = vOut.y / cos(fTheta);
	fR = 1.0f - fR * 2.0f;

	vOut.x = fTheta / _PI;
	vOut.y = -fR;

	vOut = vOut * 0.5f + 0.5f;

	return vOut;
}

#endif	// #if !defined(__KSG2_POSTEFFECT_POLAR_COORDINATE_FXH__)
