#version 450
precision highp float;
precision highp int;

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec4 color_0;

layout(location = 0) out vec4 albedo;
layout(location = 1) out vec3 reflection;
layout(location = 2) out vec3 wi;
layout(location = 3) out vec3 outNormal;

uniform mat4 mtxL2W;
uniform mat4 mtxW2C;

uniform vec4 eye;

void main()
{
    vec4 P = mtxL2W * position;;
    gl_Position = mtxW2C * P;

    albedo = color_0;

    vec3 N = normalize(mtxL2W * vec4(normal, 0)).xyz;

    vec3 V = normalize(P - eye).xyz;

    reflection = normalize(V - 2 * dot(V, N) * N);
    wi = V;
    outNormal = N;
}
