#version 450
precision highp float;
precision highp int;

varying vec4 worldSpace;
varying vec4 varColor;
varying float linearDepth;
varying float radius;

uniform float farClip;
uniform vec4 screen;

uniform sampler2D depthMap;

void main()
{
    vec3 n;

    // ���W�l�� [0, 1] �� [-1, 1] �ɕϊ�����.
    n.xy = gl_PointCoord * 2.0 - 1.0;

    // ���a�P�̉~�̒��ɂ��邩�ǂ���.
    n.z = 1.0 - dot(n.xy, n.xy);
    if (n.z < 0.0) {
        // ���ʂ����Ȃ�~�̊O�ɂ���̂Ńt���O�����g���̂Ă�.
        discard;
    }

    vec2 uv = gl_FragCoord.xy / screen.xy;
    //uv.y = 1.0 - uv.y;

    float depth = texture2D(depthMap, uv).r;
    depth *= farClip;
    
    // ���ɂ����ĉB���ꍇ�͎̂Ă�.
    if (linearDepth > depth + radius){
        discard;
    }

#if 1
    float w = pow(1.0 - dot(n.xy, n.xy), 2.0);

    gl_FragColor.rgb = varColor.rgb * w;
    gl_FragColor.a = w;
#else
    gl_FragColor = vec4(vec3(linearDepth / farClip), 1.0);
#endif
}
