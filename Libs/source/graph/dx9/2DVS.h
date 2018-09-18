#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 9.29.952.3111
//
//   fxc /T vs_2_0 /E mainVS /Fh
//    D:\work\izanagi\Libs\source\graph\2d\shader\/../../dx9/2DVS.h /nologo
//    D:\work\izanagi\Libs\source\graph\2d\shader\2DVS.hlsl
//
//
// Parameters:
//
//   float4 g_vInvScreen;
//
//
// Registers:
//
//   Name         Reg   Size
//   ------------ ----- ----
//   g_vInvScreen c0       1
//

    vs_2_0
    def c1, 2, -1, 1, 0
    dcl_position v0
    dcl_color v1
    dcl_texcoord v2
    mul r0.xy, v0, c0
    mad r0.xy, r0, c1.x, c1.y
    mul oPos.xy, r0, c1.zyzw
    mov oPos.zw, v0
    mov oD0, v1
    mov oT0.xy, v2

// approximately 6 instruction slots used
#endif

const BYTE g_vs20_mainVS[] =
{
      0,   2, 254, 255, 254, 255, 
     39,   0,  67,  84,  65,  66, 
     28,   0,   0,   0, 103,   0, 
      0,   0,   0,   2, 254, 255, 
      1,   0,   0,   0,  28,   0, 
      0,   0,   0,   1,   0,   0, 
     96,   0,   0,   0,  48,   0, 
      0,   0,   2,   0,   0,   0, 
      1,   0,   0,   0,  64,   0, 
      0,   0,  80,   0,   0,   0, 
    103,  95, 118,  73, 110, 118, 
     83,  99, 114, 101, 101, 110, 
      0, 171, 171, 171,   1,   0, 
      3,   0,   1,   0,   4,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 128,  63, 
      0,   0, 128,  63,   0,   0, 
    128,  63,   0,   0, 128,  63, 
    118, 115,  95,  50,  95,  48, 
      0,  77, 105,  99, 114, 111, 
    115, 111, 102, 116,  32,  40, 
     82,  41,  32,  72,  76,  83, 
     76,  32,  83, 104,  97, 100, 
    101, 114,  32,  67, 111, 109, 
    112, 105, 108, 101, 114,  32, 
     57,  46,  50,  57,  46,  57, 
     53,  50,  46,  51,  49,  49, 
     49,   0,  81,   0,   0,   5, 
      1,   0,  15, 160,   0,   0, 
      0,  64,   0,   0, 128, 191, 
      0,   0, 128,  63,   0,   0, 
      0,   0,  31,   0,   0,   2, 
      0,   0,   0, 128,   0,   0, 
     15, 144,  31,   0,   0,   2, 
     10,   0,   0, 128,   1,   0, 
     15, 144,  31,   0,   0,   2, 
      5,   0,   0, 128,   2,   0, 
     15, 144,   5,   0,   0,   3, 
      0,   0,   3, 128,   0,   0, 
    228, 144,   0,   0, 228, 160, 
      4,   0,   0,   4,   0,   0, 
      3, 128,   0,   0, 228, 128, 
      1,   0,   0, 160,   1,   0, 
     85, 160,   5,   0,   0,   3, 
      0,   0,   3, 192,   0,   0, 
    228, 128,   1,   0, 230, 160, 
      1,   0,   0,   2,   0,   0, 
     12, 192,   0,   0, 228, 144, 
      1,   0,   0,   2,   0,   0, 
     15, 208,   1,   0, 228, 144, 
      1,   0,   0,   2,   0,   0, 
      3, 224,   2,   0, 228, 144, 
    255, 255,   0,   0
};
