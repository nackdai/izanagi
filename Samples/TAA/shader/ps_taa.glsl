#version 450
precision highp float;
precision highp int;

in vec4 varPos;

layout(location = 0) out vec4 outColor;

uniform sampler2D s0;   // normal
uniform sampler2D s1;	// depth
uniform sampler2D s2;	// motion
uniform sampler2D s3;	// current color
uniform sampler2D s4;	// previous color

uniform vec4 invScreen;

void main()
{
	vec2 uv = gl_FragCoord.xy * invScreen.xy;

	outColor.xyz = texture2D(s3, uv).xyz;
	outColor.w = 1;
}
