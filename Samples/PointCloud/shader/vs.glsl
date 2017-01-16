#version 450
precision highp float;
precision highp int;

attribute vec4 position;
attribute vec4 color_0;

varying vec4 worldSpace;
varying float radius;
varying float linearDepth;
varying vec4 varColor;

uniform vec4 mtxW2V[4];
uniform vec4 mtxW2C[4];
uniform float size;
uniform float fov;
uniform float screenHeight;

void main()
{
    vec4 viewSpace;
    viewSpace.x = dot(position, mtxW2V[0]);
    viewSpace.y = dot(position, mtxW2V[1]);
    viewSpace.z = dot(position, mtxW2V[2]);
    viewSpace.w = dot(position, mtxW2V[3]);

    worldSpace = position;

    gl_Position.x = dot(position, mtxW2C[0]);
    gl_Position.y = dot(position, mtxW2C[1]);
    gl_Position.z = dot(position, mtxW2C[2]);
    gl_Position.w = dot(position, mtxW2C[3]);

    linearDepth = viewSpace.z;
    //linearDepth = gl_Position.z / gl_Position.w;

    // NOTE
    // S = (H/2) / tan(ƒ¦/2) => Distance to screen
    // Zview => Z in view space
    // r  => size at Z in view space
    // r' => size on scree
    // r : r' = Zv : S <=> r'/r = S/Zv <=> r' = r * S/Zv = r * projFactor
    // projFactor = S/Zv = ((H/2) / tan(ƒ¦/2)) / Zv
    float projFactor = 1.0 / tan(fov / 2.0);
    projFactor /= viewSpace.z;
    projFactor *= screenHeight / 2.0;
     
    gl_PointSize = size * projFactor;

    radius = size;

    varColor = color_0;
}