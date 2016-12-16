#version 330
precision highp float;
precision highp int;

in vec4 varColor;

uniform int id;

layout(location = 0) out int outId;

void main()
{
    outId = id;
}
