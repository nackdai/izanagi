#version 450
precision highp float;
precision highp int;

in vec4 varPos;

layout(location = 0) out vec4 outColor;

uniform sampler2D s0;

void main()
{
	outColor = texelFetch(s0, ivec2(gl_FragCoord.xy), 0);
	outColor.w = 1;
}
