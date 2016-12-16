#version 330
precision highp float;
precision highp int;

in vec4 varPos;

layout(location = 0) out vec4 outColor;

uniform sampler2D s0;   // color

// TODO
//uniform usampler2D s1;  // id
uniform sampler2D s1;  // id

// NOTE
// +y
// |
// |
// +----->+x

// NOTE
//   +---+---+---+---+
// 3 |   |   |   |   |
//   +---+---+---+---+
// 2 |(1)|   |(3)|   |
//   +---+---+---+---+
// 1 |   |   |   |   |
//   +---+---+---+---+
// 0 |(0)| 1 |(2)|   |
//   +---+---+---+---+
//    0   1   2   3

// NOTE
//   +---+---+---+---+
// 3 |   |   |   |   |
//   +---+---+---+---+
// 2 |(1)|   |(3)|   |
//   +---+---+---+---+
// 1 | 2 |   |   |   |
//   +---+---+---+---+
// 0 |(0)|   |(2)|   |
//   +---+---+---+---+
//    0   1   2   3

// NOTE
//   +---+---+---+---+
// 3 |   |   |   |   |
//   +---+---+---+---+
// 2 |(1)|   |(3)|   |
//   +---+---+---+---+
// 1 |   | 3 |   |   |
//   +---+---+---+---+
// 0 |(0)|   |(2)|   |
//   +---+---+---+---+
//    0   1   2   3

ivec4 offsetIdX[4] = ivec4[](
    ivec4( 0,  0,  2,  2),  // dummy
    ivec4(-1, -1,  1,  1),
    ivec4( 0,  0,  2,  2),
    ivec4(-1, -1,  1,  1)
);

ivec4 offsetIdY[4] = ivec4[](
    ivec4( 0,  2,  0,  2),  // dummy
    ivec4( 0,  2,  0,  2),
    ivec4(-1,  1, -1,  1),
    ivec4(-1,  1, -1,  1)
    );

const vec4 colors[6] = vec4[](
    vec4(0, 0, 0, 1),
    vec4(1, 0, 0, 1),
    vec4(0, 1, 0, 1),
    vec4(0, 0, 1, 1),
    vec4(1, 1, 0, 1),
    vec4(1, 0, 1, 1)
);

uniform bool isFinal;

uniform int capId;

void main()
{
    ivec2 baseXY = ivec2(gl_FragCoord.xy);

    vec2 texelColor = 1.0 / textureSize(s0, 0);
    vec2 texelId = 1.0 / textureSize(s1, 0);

    vec2 uvColor = (baseXY / 2) * texelColor + vec2(0.5) * texelColor;
    vec2 uvId = baseXY * texelId + vec2(0.5) * texelId;

    int idX = baseXY.x & 0x01;
    int idY = baseXY.y & 0x01;

    int id = idY * 2 + idX;

    /*
    if (id == 0) {
        outColor = texture(s0, uvColor);
        return;
    }*/

#if 0
    uint centerId = texture(s1, uvId).r;
#else
    vec4 centerId = texture(s1, uvId);

    if (centerId.rgb == colors[0].rgb) {
        outColor = texture(s0, uvColor);
        return;
    }
#endif

    ivec4 offsetX = offsetIdX[id];
    ivec4 offsetY = offsetIdY[id];

    ivec2 offsetId[4] = ivec2[](
        ivec2(offsetX.x, offsetY.x),
        ivec2(offsetX.y, offsetY.y),
        ivec2(offsetX.z, offsetY.z),
        ivec2(offsetX.w, offsetY.w)
    );

    vec4 sumColor = vec4(0, 0, 0, 0);
    float weight = 0;

    for (int i = 0; i < 4; i++) {
    //int i = capId;
        ivec2 xy = baseXY + offsetId[i];
        vec2 uv = xy * texelId + vec2(0.5) * texelId;

        if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0) {
            //continue;
        }

#if 0
        uint geomId = texture(s1, uv).r;

        if (geomId == centerId) {
#else
        vec4 geomId = texture(s1, uv);

        if (geomId.rgb == centerId.rgb) {
#endif
        if (!isFinal) {
            outColor = vec4(xy, 0, 1);
            return;
        }

            xy = ivec2(xy / 2);
            uv = xy * texelColor + vec2(0.5) * texelColor;

            


            // TODO
            float w = 1.0;

            sumColor += texture(s0, uv) * w;

            weight += w;
        }
    }

    sumColor /= weight;

    outColor = vec4(sumColor.rgb, 1.0);
}
