#version 450
precision highp float;
precision highp int;

varying vec4 varColor;

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

    gl_FragColor = varColor;
}
