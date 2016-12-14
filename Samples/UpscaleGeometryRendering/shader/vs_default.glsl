#version 450
precision highp float;
precision highp int;

attribute vec4 position;
attribute vec3 normal;
attribute vec4 color_0;

out vec4 var_Color;

uniform mat4 mtxL2W;
uniform mat4 mtxW2C;

void main()
{
    gl_Position = mtxW2C * mtxL2W * position;

    var_Color = color_0;
}
