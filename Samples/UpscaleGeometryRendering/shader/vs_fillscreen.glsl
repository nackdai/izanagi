#version 450
precision highp float;
precision highp int;

layout(location = 0) in vec4 position;

out vec4 varPos;

uniform vec4 invScreen;

void main()
{
    gl_Position = position;

    // [0, w], [0, h] -> [0, 1]
    gl_Position.xy *= invScreen.xy;

    varPos = vec4(gl_Position.x, gl_Position.y, 0.0, 1.0);

    // [0, 1] -> [-1, 1]
    gl_Position.xy = gl_Position.xy * 2.0 - 1.0;
    gl_Position.y *= -1.0;
}
