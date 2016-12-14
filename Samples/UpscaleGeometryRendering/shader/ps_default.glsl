#version 450
precision highp float;
precision highp int;

in vec4 var_Color;

layout(location = 0) out vec4 outColor;

void main()
{
    outColor = var_Color;
}
