#version 330
precision highp float;
precision highp int;

uniform sampler2D s0;

varying vec4 var_Color;
varying vec2 var_TexCoord_0;

void main()
{
    gl_FragColor = texture2D(s0, var_TexCoord_0.xy) * var_Color;
}
