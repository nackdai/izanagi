#version 450
precision highp float;
precision highp int;

uniform vec4 invScreen;

uniform sampler2D s0;
uniform sampler2D s1;

#define SCREEN_HEIGHT 719

void main()
{
    vec2 texel = invScreen.xy;
    
    ivec2 xy = ivec2(gl_FragCoord.x, gl_FragCoord.y);

    vec2 uv = gl_FragCoord.xy * invScreen.xy;

    vec4 mask = texture2D(s1, uv);

    if (mask.g > 0) {
        gl_FragColor = texture2D(s0, uv);
    }
    else if (mask.r > 0) {
#if 1
        // Average across diagonals.
        vec4 self = texture2D(s0, uv);

        int xp = xy.x & 0x01;
        int yp = xy.y & 0x01;

        if (xp == 0) {
            xy.x -= 1;
        }
        else {
            xy.x += 1;
        }

        if (yp == 0) {
            xy.y -= 1;
        }
        else {
            xy.y += 1;
        }

        uv = vec2(xy.x + 0.5, xy.y + 0.5) * invScreen.xy;

        vec4 opponent = texture2D(s0, uv);

        gl_FragColor = self + opponent;
        gl_FragColor *= 0.5;
        gl_FragColor.a = 1;
#else
        gl_FragColor = vec4(0, 1, 0, 1);
#endif
    }
    else {
#if 1
        // Average 2 neighbors.
        vec2 tmp = uv;

        if ((xy.x & 0x01) == 0) {
            tmp.x -= texel.x;
        }
        else {
            tmp.x += texel.x;
        }

        vec4 clrX = texture2D(s0, tmp);

        tmp = uv;

        if ((xy.y & 0x01) == 0) {
            tmp.y -= texel.y;
        }
        else {
            tmp.y += texel.y;
        }

        vec4 clrY = texture2D(s0, tmp);

        gl_FragColor = clrX + clrY;
        gl_FragColor *= 0.5;
        gl_FragColor.a = 1;
#else
        gl_FragColor = vec4(1, 0, 0, 1);
#endif
    }
}
