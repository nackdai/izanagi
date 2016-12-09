#version 450
precision highp float;
precision highp int;

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;

out vec4 varAlbedo;
out vec3 varNormal;
out float varDepth;

uniform mat4 mtxL2W;
uniform mat4 mtxW2C;
uniform vec4 color;

uniform float depthFar;

void main()
{
    gl_Position = mtxW2C * mtxL2W * position;

    varAlbedo = color;

    varNormal = normalize(mtxL2W * vec4(normal, 0)).xyz;
    varNormal = (varNormal + 1.0) * 0.5;

    varDepth = gl_Position.w / depthFar;
}
