#version 450
precision highp float;
precision highp int;

varying vec4 worldSpace;
varying vec4 varColor;
varying float linearDepth;
uniform float farClip;

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

    gl_FragData[0] = vec4(vec3(linearDepth / farClip), 1.0);
}
