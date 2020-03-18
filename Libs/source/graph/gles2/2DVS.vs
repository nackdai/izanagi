const char* mainVS = {

"\n"
"const vec4 vs_c1 = vec4(2.0, -1.0, 1.0, 0.0);\n"
"vec4 vs_r0;\n"
"\n"
"\n"
"varying vec4 var_Color;\n"
"\n"
"varying vec4 var_TexCoord_0;\n"
"\n"
"uniform vec4 g_vInvScreen;\n"
"\n"
"\n"
"attribute vec4 position;\n"
"\n"
"\n"
"attribute vec4 color_0;\n"
"\n"
"\n"
"attribute vec4 texcoord_0;\n"
"\n"
"\n"
"void main()\n"
"{\n"
" vs_r0.xy = position.xy * g_vInvScreen.xy;\n"
" vs_r0.xy = (vs_r0.xy * vs_c1.xx) + vs_c1.yy;\n"
" gl_Position.xy = vs_r0.xy * vs_c1.zy;\n"
" gl_Position.zw = position.zw;\n"
" var_Color = color_0;\n"
" var_TexCoord_0.xy = texcoord_0.xy;\n"
"}\n"};