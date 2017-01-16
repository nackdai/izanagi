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

    // 座標値を [0, 1] → [-1, 1] に変換する.
    n.xy = gl_PointCoord * 2.0 - 1.0;

    // 半径１の円の中にあるかどうか.
    n.z = 1.0 - dot(n.xy, n.xy);
    if (n.z < 0.0) {
        // 結果が負なら円の外にあるのでフラグメントを捨てる.
        discard;
    }

    vec4 viewSpace;
    viewSpace.x = dot(worldSpace, mtxW2V[0]);
    viewSpace.y = dot(worldSpace, mtxW2V[1]);
    viewSpace.z = dot(worldSpace, mtxW2V[2]);
    viewSpace.w = dot(worldSpace, mtxW2V[3]);

    // 端に厚みが残るように少し小さくする.
    float weight = 1.0 - dot(n.xy, n.xy) * 0.99;

    // 厚みのある板になるようにする.
    viewSpace.z -= weight * radius;

    // TODO
    // 法線があれば法線方向に厚みを持たせる.

    vec4 clipSpace;
    clipSpace.x = dot(viewSpace, mtxV2C[0]);
    clipSpace.y = dot(viewSpace, mtxV2C[1]);
    clipSpace.z = dot(viewSpace, mtxV2C[2]);
    clipSpace.w = dot(viewSpace, mtxV2C[3]);

    clipSpace /= clipSpace.w;

    clipSpace.z = max(0.0, clipSpace.z);

    // 新しい深度値.
    gl_FragDepth = clipSpace.z;

    gl_FragColor = varColor;
}
