#version 450
precision highp float;
precision highp int;

uniform vec4 invScreen;
uniform vec4 center;

#define RADIUS  180
#define SCREEN_HEIGHT 719

bool isInCircle(vec2 xy)
{
    vec2 tmp = gl_FragCoord.xy - center.xy;
    float dist = dot(tmp, tmp);

    return (dist <= RADIUS* RADIUS);
}

void main()
{
#if 1
    ivec2 xy = ivec2(gl_FragCoord.x, gl_FragCoord.y);
    xy.y = SCREEN_HEIGHT - xy.y;

    vec2 uv = vec2(xy.x + 0.5, xy.y + 0.5) * invScreen.xy;

    if (isInCircle(gl_FragCoord.xy)) {
        gl_FragColor = vec4(1, 1, 0, 1);
    }
    else {
        int xp = xy.x & 0x03;
        int yp = xy.y & 0x03;

        if (yp <= 1) {
            if (xp > 1) {
                gl_FragColor = vec4(1, 0, 0, 1);

#if 0
                if ((xp == 3 && yp == 0) || (xp == 2 && yp ==1)) {
                    gl_FragColor = vec4(0, 0, 1, 1);
                }
#endif
            }
            else {
                gl_FragColor = vec4(0, 0, 0, 1);
            }
        }
        else {
            if (xp <= 1) {
                gl_FragColor = vec4(1, 0, 0, 1);

#if 0
                if ((xp == 0 && yp == 2) || (xp == 1 && yp == 3)) {
                    gl_FragColor = vec4(0, 0, 1, 1);
                }
#endif
            }
            else {
                gl_FragColor = vec4(0, 0, 0, 1);
            }
        }
    }
#else
    ivec2 xy = ivec2(gl_FragCoord.x, gl_FragCoord.y);
    vec2 uv = gl_FragCoord.xy * invScreen.xy;

    uv.y = 1 - uv.y;

    if (isInCircle(gl_FragCoord.xy)) {
        gl_FragColor = vec4(1, 0, 0, 1);
    }
    else {
        int xp = xy.x & 0x03;
        int yp = xy.y & 0x03;

        if (yp <= 1) {
            if (xp > 1) {
                gl_FragColor = vec4(1, 0, 0, 1);
            }
            else {
                gl_FragColor = vec4(0, 0, 0, 1);
            }
        }
        else {
            if (xp <= 1) {
                gl_FragColor = vec4(1, 0, 0, 1);
            }
            else {
                gl_FragColor = vec4(0, 0, 0, 1);
            }
        }
    }
#else
    gl_FragColor = vec4(1, 0, 0, 1);
#endif
}
