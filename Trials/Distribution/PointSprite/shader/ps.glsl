#version 330
precision highp float;
precision highp int;

varying vec4 var_Color;

void main()
{
    gl_FragColor = var_Color;
    //gl_FragColor = vec4(1.0);
}
