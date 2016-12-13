#version 450
precision highp float;
precision highp int;

in vec4 varAlbedo;
in vec3 varNormal;
in float varDepth;

layout(location = 0) out vec4 outAlbedo;
layout(location = 1) out vec4 outNormal;
layout(location = 2) out vec4 outDepth;

void main()
{
    outAlbedo = varAlbedo;
    outNormal = vec4(varNormal, 1.0);
    outDepth = vec4(varDepth, 1, 1, 1);
}
