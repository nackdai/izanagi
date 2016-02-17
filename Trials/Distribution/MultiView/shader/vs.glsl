#version 450
precision highp float;
precision highp int;

attribute vec4 position;
attribute vec4 color_0;
attribute vec4 texcoord_0;

varying vec4 var_Color;
varying vec2 var_TexCoord_0;

uniform mat4 mtxW2C;

vec2 EyeClipEdge = vec2(1.0, -1.0);
vec2 EyeOffsetScale = vec2(0.5, -0.5);

// NOTE
// https://docs.google.com/presentation/d/19x9XDjUvkW_9gsfsMQzt3hZbRNziVsoCEHOn4AercAc/mobilepresent?slide=id.g5791d9ed1_015

void main()
{
    gl_Position = mtxW2C * position;

    gl_ClipDistance[0] = dot(gl_Position, vec4(EyeClipEdge[gl_InstanceID], 0.0, 0.0, 1.0));
    gl_CullDistance[0] = gl_ClipDistance[0];

    gl_Position.x *= 0.5;
    gl_Position.x += EyeOffsetScale[gl_InstanceID] * gl_Position.w;

    var_Color = color_0;

    var_TexCoord_0 = texcoord_0.xy;
}
