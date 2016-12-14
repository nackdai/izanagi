#version 450
precision highp float;
precision highp int;

in vec4 varPos;

layout(location = 0) out vec4 outColor;

uniform sampler2D s0;   // color
uniform usampler2D s1;  // id

const ivec4 offsetX[3] = ivec4[](
    ivec4(-1, -1, 1, 1),
    ivec4(-2, -2, 0, 0),
    ivec4(-1, -1, 1, 1)
);

const ivec4 offsetY[3] = ivec4[](
    ivec4( 0,  2,  0, 2),
    ivec4(-1,  1, -1, 1),
    ivec4(-1,  1, -1, 1)
);

void main()
{
    ivec2 xy = ivec2(gl_FragCoord.xy);

    vec2 texelColor = 1.0 / textureSize(s0, 0);
    vec2 textelId = 1.0 / textureSize(s1, 0);

    vec2 uvColor = xy * textelId;
    vec2 uvId = xy * textelId;

    int idX = xy.x & 0x01;
    int idY = xy.y & 0x01;

    int id = idY * 2 + idX;

    if (id == 0) {
        outColor = texture(s0, uvColor);;
        return;
    }

    uint centerId = texture(s1, uvId).r;

    vec4 offX = offsetX[id];
    vec4 offY = offsetY[id];

    vec2 uvs[4] = vec2[](
        (xy + ivec2(offX.x, offY.x)) * textelId,
        (xy + ivec2(offX.y, offY.y)) * textelId,
        (xy + ivec2(offX.z, offY.z)) * textelId,
        (xy + ivec2(offX.w, offY.w)) * textelId
    );

    float cnt = 0;
    vec4 sumColor = vec4(0, 0, 0, 0);

    for (int i = 0; i < 4; i++) {
        uint id = texture(s1, uvs[i]).r;

        if (id == centerId) {
            vec4 color = texture(s0, uvs[i]);
            sumColor += color;

            cnt += 1.0f;
        }
    }

    sumColor /= cnt;

    outColor = vec4(sumColor.rgb, 1.0);
}
