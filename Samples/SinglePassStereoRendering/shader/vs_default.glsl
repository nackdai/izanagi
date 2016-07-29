#version 450
precision highp float;
precision highp int;

attribute vec4 position;
attribute vec4 color_0;
attribute vec4 texcoord_0;

varying vec4 var_Color;
varying vec2 var_TexCoord_0;

uniform mat4 mtxL2W;
uniform mat4 mtxW2C;

void main()
{
    gl_Position = mtxW2C * mtxL2W * position;

    var_Color = color_0;

    var_TexCoord_0 = texcoord_0.xy;
}
