#if !defined(__IZANAGI_POSTEFFECT_COLOR_FILETER_USER_DEFS_FXH__)
#define __IZANAGI_POSTEFFECT_COLOR_FILETER_USER_DEFS_FXH__

//////////////////////////////////
// 任意のカラーフィルタ

float4 ColorFilterUserDefs(float4 vIn, float3x3 mFilter)
{
	return float4(mul(vIn.rgb, mFilter), vIn.a);
}

float4 ColorFilterUserDefs(float4 vIn, float3 vFilterR, float3 vFilterG, float3 vFilterB)
{
	return float4(
			dot(vIn.rgb, vFilterR),
			dot(vIn.rgb, vFilterG),
			dot(vIn.rgb, vFilterB),
			vIn.a);
}

// 線形補間あり
float4 ColorFilterUserDefs(float4 vIn, float3x3 mFilter, float fWeight)
{
	float3 tmp = lerp(vIn.rgb, mul(vIn.rgb, mFilter), fWeight);
	return float4(tmp, vIn.a);
}

// 線形補間あり
float4 ColorFilterUserDefs(float4 vIn, float3 vFilterR, float3 vFilterG, float3 vFilterB, float fWeight)
{
	float3 tmp = float3(
					dot(vIn.rgb, vFilterR),
					dot(vIn.rgb, vFilterG),
					dot(vIn.rgb, vFilterB));
	tmp = lerp(vIn.rgb, tmp, fWeight);
	
	return float4(tmp, vIn.a);
}

float4 ColorFilterUserDefs(float4 vIn, float4x4 mFilter)
{
	return mul(vIn, mFilter);
}

float4 ColorFilterUserDefs(float4 vIn, float4 vFilterR, float4 vFilterG, float4 vFilterB, float4 vFilterA)
{
	return float4(
			dot(vIn, vFilterR),
			dot(vIn, vFilterG),
			dot(vIn, vFilterB),
			dot(vIn, vFilterA));
}

// 線形補間あり
float4 ColorFilterUserDefs(float4 vIn, float4x4 mFilter, float fWeight)
{
	return lerp(vIn, mul(vIn, mFilter), fWeight);
}

// 線形補間あり
float4 ColorFilterUserDefs(float4 vIn, float4 vFilterR, float4 vFilterG, float4 vFilterB, float4 vFilterA, float fWeight)
{
	float4 tmp =  float4(
					dot(vIn, vFilterR),
					dot(vIn, vFilterG),
					dot(vIn, vFilterB),
					dot(vIn, vFilterA));
	return lerp(vIn, tmp, fWeight);
}

#endif	// #if !defined(__IZANAGI_POSTEFFECT_COLOR_FILETER_USER_DEFS_FXH__)
