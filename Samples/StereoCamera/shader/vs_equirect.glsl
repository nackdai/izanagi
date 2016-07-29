#version 450
precision highp float;
precision highp int;

attribute vec4 position;

varying vec3 uv;

uniform mat4 g_mL2W;
uniform mat4 g_mW2C;

uniform vec4 g_vEye;

void main()
{
    gl_Position = g_mL2W * position;

    // Compute reflection vector.
    vec3 ref = normalize(gl_Position.xyz - g_vEye.xyz);

    gl_Position = g_mW2C * gl_Position;

    uv = ref;
}
