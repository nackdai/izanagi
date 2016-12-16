#version 330
precision highp float;
precision highp int;

attribute vec4 position;
attribute vec4 color_0;

out vec4 varColor;

uniform mat4 mtxL2W;
uniform mat4 mtxW2C;

void main()
{
    gl_Position = mtxW2C * mtxL2W * position;

    varColor = color_0;
}
