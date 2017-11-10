#version 450
precision highp float;
precision highp int;

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color_0;
layout(location = 2) in vec3 normal;

uniform mat4 mtxL2W;
uniform mat4 mtxW2C;

out vec4 varColor;
out vec3 varNormal;
out float depth;

void main()
{
    gl_Position = mtxW2C * mtxL2W * position;

	varColor = color_0;

	varNormal = normalize(mtxL2W * vec4(normal, 0)).xyz;

	depth = gl_Position.w;
}
