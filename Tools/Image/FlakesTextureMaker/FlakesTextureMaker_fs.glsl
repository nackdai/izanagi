#version 450
precision highp float;
precision highp int;

uniform vec4 u_resolution;
uniform float u_time;

vec2 hash(vec2 p)
{
    p = vec2(dot(p, vec2(127.1, 311.7)), dot(p, vec2(269.5, 183.3)));
    return fract(sin(p)*18.5453);
}

#define K (6.2831)

// return distance, and cell id
vec2 voronoi(in vec2 x)
{
    vec2 n = floor(x);
    vec2 f = fract(x);

    float flake_size = 0.05;
    vec3 m = vec3(flake_size);

    bool isValid = false;

    for (int j = -1; j <= 1; j++)
    {
        for (int i = -1; i <= 1; i++)
        {
            vec2  g = vec2(float(i), float(j));
            vec2  o = hash(n + g);
            vec2  r = g - f + (0.5 + 0.5*sin(u_time + K * o));

            // distance.
            float d = dot(r, r);

            if (d < m.x) {
                m = vec3(d, o);
                isValid = true;
            }
        }
    }
    return isValid ? vec2(sqrt(m.x), m.y + m.z) : vec2(0, 0);
}

void main()
{
    vec2 ndcPos = gl_FragCoord.xy / u_resolution.xy;
    ndcPos = ndcPos * 2.0 - 1.0;    // [0, 1] -> [-1, 1]

    float flake_scale = 1;
    vec2 p = ndcPos * 1.0 / flake_scale;
    
    vec2 c = voronoi((14.0 + 6.0*sin(0.2*u_time))*p);

    vec3 nv = vec3(0, 0, 1);

    if (dot(c, c) > 0) {
        float x = 0.5 + 0.5 * cos(c.y * K);
        float y = 0.5 + 0.5 * sin(c.y * K);

        // calculate normal vector
        nv = normalize(vec3(x * 2.0 - 1.0, y * 2.0 - 1.0, 1.0));

        float flake_orientation = 0.1;
        nv = mix(nv, vec3(0, 0, 1), flake_orientation);
        nv = normalize(nv);
    }

    // encode normal vector
    gl_FragColor = vec4(nv * 0.5 + 0.5, 1.0);
}