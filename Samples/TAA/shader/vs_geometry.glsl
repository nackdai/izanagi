#version 330
precision highp float;
precision highp int;

attribute vec4 position;
attribute vec3 normal;

out float depth;
out vec3 nml;
out vec4 prevCSPos;

uniform mat4 mtxL2W;
uniform mat4 mtxW2C;

uniform mat4 mtxPrevL2W;
uniform mat4 mtxPrevW2C;

void main()
{
    gl_Position = mtxW2C * mtxL2W * position;

	prevCSPos = mtxPrevW2C * mtxPrevL2W * position;

	depth = gl_Position.w;
	nml = normalize(mtxL2W * vec4(normal, 0)).xyz;
}
