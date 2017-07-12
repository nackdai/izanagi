#version 450
precision highp float;
precision highp int;

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec4 color_0;

layout(location = 0) out vec4 albedo;
layout(location = 1) out vec3 cameraSpaceNormal;

uniform mat4 mtxL2W;
uniform mat4 mtxW2C;
uniform mat4 mtxW2V;

void main()
{
    gl_Position = mtxW2C * mtxL2W * position;

    albedo = color_0;

    cameraSpaceNormal = normalize(mtxW2V * mtxL2W * vec4(normal, 0)).xyz;
}
