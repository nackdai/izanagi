#version 330
precision highp float;
precision highp int;

in vec4 varColor;

uniform int id;

layout(location = 0) out int outId;
//layout(location = 0) out vec4 outColor;

const vec4 colors[6] = vec4[](
    vec4(0, 0, 0, 1),
    vec4(1, 0, 0, 1),
    vec4(0, 1, 0, 1),
    vec4(0, 0, 1, 1),
    vec4(1, 1, 0, 1),
    vec4(1, 0, 1, 1)
);

void main()
{
    outId = id;
    //outColor = colors[id];
}
