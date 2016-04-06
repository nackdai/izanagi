//
// Generated by Microsoft (R) HLSL Shader Compiler 9.30.9200.20499
//
//
///
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_POSITION              0   xyzw        0      POS   float   xyzw
// VTX_TEXCOORD             0   xy          1     NONE   float   xy  
// RTARRAYINDEX             0   x           2     NONE    uint   x   
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_POSITION              0   xyzw        0      POS   float   xyzw
// TEXCOORD                 0   xy          1     NONE   float   xy  
// SV_RenderTargetArrayIndex     0   x           2  RTINDEX    uint   x   
//
gs_4_0
dcl_input_siv v[3][0].xyzw, position
dcl_input v[3][1].xy
dcl_input v[3][2].x
dcl_temps 1
dcl_inputprimitive triangle 
dcl_outputtopology trianglestrip 
dcl_output_siv o0.xyzw, position
dcl_output o1.xy
dcl_output_siv o2.x, rendertarget_array_index
dcl_maxout 3
mov r0.x, l(0)
loop 
  ige r0.y, r0.x, l(3)
  breakc_nz r0.y
  mov o0.xyzw, v[r0.x + 0][0].xyzw
  mov o1.xy, v[r0.x + 0][1].xyxx
  mov o2.x, v[r0.x + 0][2].x
  emit 
  iadd r0.x, r0.x, l(1)
endloop 
ret 
// Approximately 11 instruction slots used