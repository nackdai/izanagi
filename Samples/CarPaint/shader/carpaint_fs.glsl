#version 450
precision highp float;
precision highp int;

layout(location = 0) in vec4 albedo;
layout(location = 1) in vec3 cameraSpaceNormal;

layout(location = 0) out vec4 outColor;

void main()
{
    outColor = albedo;
}
