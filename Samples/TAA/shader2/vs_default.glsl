#version 330
precision highp float;
precision highp int;

attribute vec4 position;
attribute vec3 normal;

out vec4 var_Color;

uniform mat4 mtxL2W;
uniform mat4 mtxW2C;
uniform mat4 mtxOffset;
uniform vec4 color;

void main()
{
    gl_Position = mtxOffset * mtxW2C * mtxL2W * position;

    var_Color = color;
}
