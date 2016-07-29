#version 450
precision highp float;
precision highp int;

/* NOTE
���_�o�b�t�@���g�킸�S��ʂɕ`�悷�钸�_�V�F�[�_
https://shobomaru.wordpress.com/2014/12/31/shader-only-fullscreen-quad/
*/

void main()
{
    int x = (gl_VertexID & 1) * 2 - 1;
    int y = (gl_VertexID & 2) * 2 - 1;
    gl_Position = vec4(x, y, 0, 1);
}
