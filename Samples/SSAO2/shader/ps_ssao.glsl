#version 450
precision highp float;
precision highp int;

in vec4 varPos;

layout(location = 0) out vec4 outColor;

uniform mat4 mtxC2V;
uniform mat4 mtxV2W;
uniform float depthFar;
uniform float radius;

uniform mat4 mtxW2C;
uniform mat4 mtxV2C;

uniform sampler2D s0;   // albedo
uniform sampler2D s1;   // normal
uniform sampler2D s2;   // depth

#define KernelSize  (32)
uniform vec4 kernels[KernelSize];

#define FallOff (0.000002)

void main()
{
    vec4 uv = varPos;

    uv.y = 1.0 - uv.y;

    // Depth format is R32F.
    float depth = texture(s2, uv.xy).r;

    if (depth <= 0.0f) {
        discard;
    }

    // Convert depth [0, 1] -> [0, far] in view-space.
    float centerDepth = depth * depthFar;

    // NOTE
    // Pview = (Xview, Yview, Zview, 1)
    // mtxV2C = W 0 0 0
    //          0 H 0 0
    //          0 0 A 1
    //          0 0 B 0
    // Pview * mtxV2C = (Xclip, Yclip, Zclip, Wclip) = (Xclip, Yclip, Zclip, Zview)
    //  Wclip = Zview = depth
    // Xscr = Xclip / Wclip = Xclip / Zview = Xclip / depth
    // Yscr = Yclip / Wclip = Yclip / Zview = Yclip / depth
    //
    // Xscr * depth = Xclip
    // Xview = Xclip * mtxC2V

    vec4 pos = vec4(varPos.xy, 0, 1);

    // [0, 1] -> [-1, 1]
    pos.xy = pos.xy * 2.0 - 1.0;

    // Screen-space -> Clip-space
    pos.xy *= centerDepth;

    // Clip-space -> View-space
    pos = mtxC2V * pos;
    pos.z = centerDepth;

    // View-space -> World-space
    pos = mtxV2W * pos;
    pos.w = 1.0;

#if 0
    // Debug..
    pos = mtxW2C * pos;
    pos /= pos.w;
    pos.xy = (pos.xy + 1.0) * 0.5;

    pos.y = 1.0 - pos.y;
    
    outColor = texture(s1, pos.xy);
#else
    vec3 normal = texture(s1, uv.xy).xyz * 2.0 - 1.0;
    normal = normalize(normal);

    // 平行にならない軸を決める.
    vec3 tangent = abs(normal.x) > 0.9 ? vec3(0, 1, 0) : vec3(1, 0, 0);
    tangent = normalize(cross(tangent, normal));

    // NOTE
    // Left hand coordinate.

    vec3 bitangent = cross(tangent, normal);

    mat3 tbn = mat3(tangent, bitangent, normal);

    float occlusion = 0.0;

    for (int i = 0; i < KernelSize; i++) {
        // Get sample position.
        vec3 samplePos = tbn * kernels[i].xyz;
        samplePos = samplePos * radius + pos.xyz;

        // Compute offset uv.
        vec4 offset = vec4(samplePos, 1.0);
        offset = mtxW2C * offset;
        offset.xy /= offset.w;
        offset.xy = offset.xy * 0.5 + 0.5;  // [-1, 1] -> [0, 1]
        offset.y = 1.0 - offset.y;

        float sampleDepth = texture(s2, offset.xy).r;
        sampleDepth *= depthFar;

        // Range check.
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(centerDepth - sampleDepth));

        vec3 ocNml = texture(s1, offset.xy).xyz;
        ocNml = normalize(ocNml * 2.0f - 1.0f);

        // 遮蔽するピクセルと現在のピクセルでの法線の角度の差を計算
        // 同方向なら1.0、反対なら0.0
        // 同方向を向いていたら遮蔽されていない、角度が大きいほど遮蔽されている、とみなす
        //  -> 同方向を向いている = 同じ面上にある -> 遮蔽していない
        //  -> 角度が大きい = 同じ面上にない -> 遮蔽している
        float nmlDiff = 1.0 - dot(ocNml, normal);

        // 正：遮蔽するピクセルが現在のピクセルより前 -> 現在のピクセルは遮蔽される
        // 負：遮蔽するピクセルが現在のピクセルより奥 -> 現在のピクセルは遮蔽されない
        float depthDiff = centerDepth - sampleDepth;

        // step(falloff, depthDiff) -> falloff より小さければ 0.0、大きければ 1.0
        //  -> ある程度以上深度が離れていない場合は遮蔽されていることとする
        occlusion += step(FallOff, depthDiff) * nmlDiff * rangeCheck;
    }

    occlusion = 1.0 - (occlusion / KernelSize);

    outColor = vec4(occlusion, occlusion, occlusion, occlusion);
#endif
}
