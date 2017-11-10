#version 450
precision highp float;
precision highp int;

// NOTE
// http://roar11.com/2015/07/screen-space-glossy-reflections/

in vec4 varColor;
in vec3 varNormal;

layout(location = 0) out vec4 outColor;

uniform sampler2D s0;   // color
uniform sampler2D s1;   // depth

uniform mat4 mtxW2V;
uniform mat4 mtxV2C;

uniform mat4 mtxC2V;
uniform mat4 mtxV2W;

uniform vec3 camPos;
uniform float nearPlaneZ;
uniform float maxDistance;
uniform float zThickness;
uniform int maxSteps;

float squaredLength(vec2 a, vec2 b)
{
	a -= b;
	return dot(a, a);
}

bool intersectsDepthBuffer(float z, float minZ, float maxZ)
{
	/*
	* Based on how far away from the camera the depth is,
	* adding a bit of extra thickness can help improve some
	* artifacts. Driving this value up too high can cause
	* artifacts of its own.
	*/

	// �w��͈͓��i���C�̎n�_�ƏI�_�j�� z ������΁A����̓��C�Ƀq�b�g�����Ƃ݂Ȃ���.

	z += zThickness;
	return (maxZ >= z) && (minZ - zThickness <= z);
}

bool traceScreenSpaceRay(
	vec3 csOrig,
	vec3 csDir,
	out vec2 hitPixel,
	out vec3 hitPoint)
{
	// Clip to the near plane.
	float rayLength = (csOrig.z + csDir.z * maxDistance) < nearPlaneZ
		? (nearPlaneZ - csOrig.z) / csDir.z
		: maxDistance;

	vec3 csEndPoint = csOrig + csDir * rayLenght;

	// Project into homogeneous clip space.
	vec4 H0 = mtxV2C * vec4(csOrig, 1);
	vec4 H1 = mtxV2C * vec4(csEndPoint, 1);

	float k0 = 1.0 / HO.w;
	float k1 = 1.0 / H1.w;

	// The interpolated homogeneous version of the camera-space points.
	vec3 Q0 = csOrig * k0;
	vec3 Q1 = csEndPoint * k1;

	// Screen space point.
	vec2 P0 = H0.xy * k0;
	vec2 P1 = H1.xy * k1;

	ivec2 texsize = textureSize(s0, 0);
	vec2 texel = vec2(1) / texsize;

	P0 *= vec2(texsize.x, texsize.y);
	P1 *= vec2(texsize.x, texsize.y);

	// If the line is degenerate, make it cover at least one pixel to avoid handling zero-pixel extent as a special case later.
	// 2�_�Ԃ̋�����������x�����悤�ɂ���.
	P1 += squaredLength(P0, P1) < 0.0001
		? vec2(0.01, 0.01)
		: vec2(0.0);
	vec2 delta = P1 - P0;

	// Permute so that the primary iteration is in x to collapse all quadrant-specific DDA cases later.
	bool permute = false;
	if (abs(delta.x) < abs(delta.y))
	{
		permute = true;

		delta = delta.yx;
		P0 = P0.yx;
		P1 = P1.yx;
	}

	float stepDir = sign(delta.x);
	float invdx = stepDir / delta.x;

	// Track the derivatives of Q and k.
	vec3 dQ = (Q1 - Q0) / invdx;
	float dk = (k1 - k0) / invdx;

	// y is slope.
	// slope = (y1 - y0) / (x1 - x0)
	vec2 dP = vec2(stepDir, delta.y / invdx);	

	// Adjust end condition for iteration direction
	float end = P1.x * stepDir;

	int stepCount = 0;

	float prevZMaxEstimate = csOrig.z;
	
	float rayZMin = prevZMaxEstimate;
	float rayZMax = prevZMaxEstimate;
	
	float sceneZMax = rayZMax + 100.0f;

	vec4 PQk = vec4(P0.x, P0.y, Q0.z, k0);
	vec4 dPQk = vec4(dP0.x, dP0.y, dQ0.z, dk0);
	vec3 Q = Q0;

	for (;
		((PQk.x * stepDir) <= end)	// �I�_�ɓ��B������.
		&& (stepCount < maxSteps)	// �ő又�����ɓ��B������.
		&& !intersectsDepthBuffer(sceneZMax, rayZMin, rayZMax)	// ���C���Փ˂�����.
		&& (sceneZMax != 0.0);	// �����Ȃ��Ƃ���ɓ��B���ĂȂ���.
		++stepCount)
	{
		// �O���Z�̍ő�l�����̍ŏ��l�ɂȂ�.
		rayZMin = prevZMaxEstimate;

		// ����Z�̍ő�l���v�Z����.
		// �������A1/2 pixel�� �]�T����������.
		// Q��w�����ŏ��Z����Ă��āA������1/w�ŏ��Z����̂ŁA���iView���W�n�j�ɖ߂邱�ƂɂȂ�.
		rayZMax = (PQk.z + dPGk.z * 0.5) / (PQk.w + dPQk.w * 0.5);

		// ���Ɍ����čő�l��ێ�.
		prevZMaxEstimate = rayZMax;

		if (rayZMin > rayZMax) {
			// �O�̂���.
			float tmp = rayZMin;
			rayZMin = rayZMax;
			rayZMax = tmp;
		}

		hitPixel = permute ? PQk.yx : PQk.xy;

		// �V�[�����̌����_�ł̐[�x�l���擾.
		sceneZMax = texture(s1, hitPixel * texel).r;

		PQk += dPQk;
	}

	// Advance Q based on the number of steps
	Q.xy += dQ.xy * stepCount;

	// Q��w�����ŏ��Z����Ă��āA������1 / w�ŏ��Z����̂ŁA���iView���W�n�j�ɖ߂邱�ƂɂȂ�.
	hitPoint = Q * (1.0f / PQk.w);

	return intersectsDepthBuffer(sceneZMax, rayZMin, rayZMax);
}

void main()
{
	vec3 normal = normalize(varNormal);
	float depth = texFetch(s1, gl_FragCoord.xy, 0).r;

	ivec2 texsize = textureSize(s0, 0);

	// Ray origin is camera origin.
	vec3 rayOrg = camPos;

	// Screen coordinate.
	vec4 pos = vec4(gl_FragCoord.xy / texsize, 0, 1);

	// [0, 1] -> [-1, 1]
	pos.xy = pos.xy * 2.0 - 1.0;

	// Screen-space -> Clip-space
	pos.xy *= depth;

	// Clip-space -> View-space
	pos = mtxC2V * pos;
	pos.z = depth;

	// View-space -> World-space.
	vec3 worldPos = mtxV2W * vec4(pos.xyz, 1);

	// Compute ray direction.
	// From ray origin to world position.
	vec3 rayDir = normalize(worldPos - rayOrg);

	// Compute reflection vector.
	vec3 refDir = reflect(rayDir, normal);

	// Reflection vector origin is world position.
	vec3 refOrg = worldPos;

	// Transform to view coordinate.
	refOrg = (mtxW2V * vec4(refOrg, 1)).xyz;
	refDir = (mtxW2V * vec4(refDir, 0)).xyz;

	vec2 hitPixel = vec2(0, 0);
	vec3 hitPoint = vec3(0, 0, 0);

	// Trace screen space ray.
	bool isIntersect = traceScreenSpaceRay(refOrg, refDir, hitPixel, hitPoint);

	vec2 uv = hitPixel / texsize.xy;

	if (uv.x > 1.0 || uv.x < 0.0f || uv.y > 1.0 || uv.y < 0.0) {
		isIntersect = false;
	}

	if (isIntersect) {
		outColor = varColor * texture(s0, uv);
	}
	else {
		outColor = varColor;
	}
}
