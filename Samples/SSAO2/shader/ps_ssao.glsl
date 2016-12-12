#version 450
precision highp float;
precision highp int;

in vec4 varPos;

layout(location = 0) out vec4 outColor;

uniform mat4 mtxC2V;
uniform mat4 mtxV2W;
uniform float depthFar;

uniform mat4 mtxW2C;

uniform sampler2D s0;   // albedo
uniform sampler2D s1;   // normal
uniform sampler2D s2;   // depth

void main()
{
    vec4 uv = varPos;

    uv.y = 1.0 - uv.y;

    // Depth format is R32F.
    float depth = texture2D(s2, uv.xy).r;

    if (depth <= 0.0f) {
        discard;
    }

    // Convert depth [0, 1] -> [0, far] in view-space.
    depth *= depthFar;

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
    pos.xy *= depth;

    // Clip-space -> View-space
    pos = mtxC2V * pos;
    pos.z = depth;

    // View-space -> World-space
    pos = mtxV2W * pos;
    pos.w = 1.0;

#if 1
    // Debug..
    pos = mtxW2C * pos;
    pos /= pos.w;
    pos.xy = (pos.xy + 1.0) * 0.5;

    pos.y = 1.0 - pos.y;
    
    outColor = texture2D(s1, pos.xy);
#else

#endif
}
