#version 450
precision highp float;
precision highp int;

varying vec4 worldSpace;
varying float radius;
varying vec4 varColor;

uniform vec4 mtxW2V[4];
uniform vec4 mtxV2C[4];

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

    vec4 viewSpace;
    viewSpace.x = dot(worldSpace, mtxW2V[0]);
    viewSpace.y = dot(worldSpace, mtxW2V[1]);
    viewSpace.z = dot(worldSpace, mtxW2V[2]);
    viewSpace.w = dot(worldSpace, mtxW2V[3]);

    // �[�Ɍ��݂��c��悤�ɏ�������������.
    float weight = 1.0 - dot(n.xy, n.xy) * 0.99;

    // ���݂̂���ɂȂ�悤�ɂ���.
    viewSpace.z -= weight * radius;

    // TODO
    // �@��������Ζ@�������Ɍ��݂���������.

    vec4 clipSpace;
    clipSpace.x = dot(viewSpace, mtxV2C[0]);
    clipSpace.y = dot(viewSpace, mtxV2C[1]);
    clipSpace.z = dot(viewSpace, mtxV2C[2]);
    clipSpace.w = dot(viewSpace, mtxV2C[3]);

    clipSpace /= clipSpace.w;

    clipSpace.z = max(0.0, clipSpace.z);

    // �V�����[�x�l.
    gl_FragDepth = clipSpace.z;

    gl_FragColor = varColor;
}
