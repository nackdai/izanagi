#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 9.29.952.3111
//
//   fxc /T ps_2_0 /E main_OpModulateAlpha /Fh
//    D:\work\izanagi\Libs\source\graph\2d\shader\/../../dx9/2DPS_OpModulateAlpha.h
//    /nologo D:\work\izanagi\Libs\source\graph\2d\shader\2DPS.hlsl
//
//
// Parameters:
//
//   sampler2D sTex;
//
//
// Registers:
//
//   Name         Reg   Size
//   ------------ ----- ----
//   sTex         s0       1
//

    ps_2_0
    def c0, 1, 0, 0, 0
    dcl v0
    dcl t0.xy
    dcl_2d s0
    texld r0, t0, s0
    mov r0.xyz, c0.x
    mul_sat r0, r0, v0
    mov oC0, r0

// approximately 4 instruction slots used (1 texture, 3 arithmetic)
#endif

const BYTE g_ps20_main_OpModulateAlpha[] =
{
      0,   2, 255, 255, 254, 255, 
     33,   0,  67,  84,  65,  66, 
     28,   0,   0,   0,  79,   0, 
      0,   0,   0,   2, 255, 255, 
      1,   0,   0,   0,  28,   0, 
      0,   0,   0,   1,   0,   0, 
     72,   0,   0,   0,  48,   0, 
      0,   0,   3,   0,   0,   0, 
      1,   0,   2,   0,  56,   0, 
      0,   0,   0,   0,   0,   0, 
    115,  84, 101, 120,   0, 171, 
    171, 171,   4,   0,  12,   0, 
      1,   0,   1,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
    112, 115,  95,  50,  95,  48, 
      0,  77, 105,  99, 114, 111, 
    115, 111, 102, 116,  32,  40, 
     82,  41,  32,  72,  76,  83, 
     76,  32,  83, 104,  97, 100, 
    101, 114,  32,  67, 111, 109, 
    112, 105, 108, 101, 114,  32, 
     57,  46,  50,  57,  46,  57, 
     53,  50,  46,  51,  49,  49, 
     49,   0,  81,   0,   0,   5, 
      0,   0,  15, 160,   0,   0, 
    128,  63,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  31,   0,   0,   2, 
      0,   0,   0, 128,   0,   0, 
     15, 144,  31,   0,   0,   2, 
      0,   0,   0, 128,   0,   0, 
      3, 176,  31,   0,   0,   2, 
      0,   0,   0, 144,   0,   8, 
     15, 160,  66,   0,   0,   3, 
      0,   0,  15, 128,   0,   0, 
    228, 176,   0,   8, 228, 160, 
      1,   0,   0,   2,   0,   0, 
      7, 128,   0,   0,   0, 160, 
      5,   0,   0,   3,   0,   0, 
     31, 128,   0,   0, 228, 128, 
      0,   0, 228, 144,   1,   0, 
      0,   2,   0,   8,  15, 128, 
      0,   0, 228, 128, 255, 255, 
      0,   0
};
