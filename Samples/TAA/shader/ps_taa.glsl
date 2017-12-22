#version 330
precision highp float;
precision highp int;

in vec4 varPos;

layout(location = 0) out vec4 outColor;

uniform sampler2D s0;   // normal
uniform sampler2D s1;	// depth

uniform vec4 invScreen;

void main()
{
	vec2 uv = gl_FragCoord.xy * invScreen.xy;

	outColor.xyz = texture2D(s1, uv).xyz;
	outColor.w = 1;
}
