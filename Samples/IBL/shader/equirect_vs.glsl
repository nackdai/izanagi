#version 450
precision highp float;
precision highp int;

layout(location = 0) in vec4 position;

layout(location = 0) out vec3 ref;

uniform mat4 mtxL2W;
uniform mat4 mtxW2C;

uniform vec4 eye;

void main()
{
    gl_Position = mtxL2W * position;

    // Compute reflection vector.
    ref = normalize(gl_Position.xyz - eye.xyz);

    gl_Position = mtxW2C * gl_Position;
}
