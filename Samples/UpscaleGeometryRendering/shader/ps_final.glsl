#version 450
precision highp float;
precision highp int;

in vec4 varPos;

layout(location = 0) out vec4 outColor;

uniform sampler2D s0;   // color
uniform usampler2D s1;  // id

// NOTE
// +y
// |
// |
// +----->x

// NOTE
//   +---+---+---+---+
// 3 |   |   |   |   |
//   +---+---+---+---+
// 2 |(1)|   |(3)|   |
//   +---+---+---+---+
// 1 |2  |3  |   |   |
//   +---+---+---+---+
// 0 |(0)|1  |(2)|   |
//   +---+---+---+---+
//     0   1   2   3

ivec4 offsetIdX[4] = ivec4[](
    ivec4( 0,  0, 1, 1),
    ivec4(-1, -1, 1, 1),
    ivec4( 0,  0, 2, 2),
    ivec4(-1, -1, 1, 1)
);

ivec4 offsetIdY[4] = ivec4[](
    ivec4( 0, 2,  0, 2),
    ivec4( 0, 2,  0, 2),
    ivec4(-1, 1, -1, 1),
    ivec4(-1, 1, -1, 1)
);

// NOTE
//      +-------+-------+
// 1    |       |       |
// (2-3)|       |       |
//      |(1)    |(3)    |
//      +-------+-------+
// 0    |2   3  |       |
// (0-1)|       |       |
//      |(0) 1  |(2)    |
//      +-------+-------+
//       0(0-1)   1(2-3)

ivec4 offsetColorX = ivec4(0, 0, 1, 1);

ivec4 offsetColorY = ivec4(0, 1, 0, 1);

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
    ivec2 xyColor = ivec2(gl_FragCoord.xy) / 2;
    ivec2 xyId = ivec2(gl_FragCoord.xy);

    vec2 texelColor = 1.0 / vec2(640.0, 360.0);//textureSize(s0, 0);
    vec2 texelId = 1.0 / textureSize(s1, 0);

    vec2 uvColor = xyColor * texelColor;
    vec2 uvId = xyId * texelId;

    outColor = texture(s0, uvColor);
    return;

    int idX = xyId.x & 0x01;
    int idY = xyId.y & 0x01;

    int id = idY * 2 + idX;

#if 0
    uint centerId = texture(s1, uvId).r;
#else
    vec4 centerId = texture(s1, uvId);
#endif

    ivec4 offX = offsetIdX[id];
    ivec4 offY = offsetIdY[id];

    vec2 offsetId[4] = vec2[](
        ivec2(offX.x, offY.x) * texelId,
        ivec2(offX.y, offY.y) * texelId,
        ivec2(offX.z, offY.z) * texelId,
        ivec2(offX.w, offY.w) * texelId
    );

    vec2 offsetColor[4] = vec2[](
        ivec2(offsetColorX.x, offsetColorY.x) * texelColor,
        ivec2(offsetColorX.y, offsetColorY.y) * texelColor,
        ivec2(offsetColorX.z, offsetColorY.z) * texelColor,
        ivec2(offsetColorX.w, offsetColorY.w) * texelColor
    );

    vec4 sumColor = vec4(0, 0, 0, 0);
    float weight = 0;

    for (int i = 0; i < 4; i++) {
        vec2 uvOffId = uvId + offsetId[i];

        if (uvOffId.x < 0.0 || uvOffId.x > 1.0 || uvOffId.y < 0.0 || uvOffId.y > 1.0) {
            continue;
        }

#if 0
        uint geomId = texture(s1, uv).r;

        if (geomId == centerId) {
            sumColor += texture(s0, uv);
            cnt += 1.0;
        }
#else
        vec4 geomId = texture(s1, uvOffId);

        if (geomId.rgb == centerId.rgb) {
            vec2 uvOffColor = uvColor + offsetColor[i];
            float w = length(vec2(2, 2)) - length(offsetId[i]);

            sumColor += texture(s0, uvOffColor) * w;
            weight += w;
        }
        else {
            outColor = vec4(1, 1, 0, 1);
            return;
        }
#endif
    }

    sumColor /= weight;

    outColor = vec4(sumColor.rgb, 1.0);
}
