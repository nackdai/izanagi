#version 450
precision highp float;
precision highp int;

attribute vec4 position;

varying vec4 var_Color;

uniform mat4 mtxL2W;
uniform mat4 mtxW2C;
uniform vec4 color;

void main()
{
    gl_Position = mtxW2C * mtxL2W * position;

    var_Color = color;
}
