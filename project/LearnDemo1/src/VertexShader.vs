#version 330 core
#pragma optimize(off)
uniform mat4 MVP;
attribute vec3 vCol0;
attribute vec2 aTex;
attribute vec3 vPos;
varying vec3 color;
varying vec2 TexCoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
	gl_Position = projection * view * model * vec4(vPos, 1.0);
// 	gl_Position = MVP * vec4(vPos, 1.0);
	color = vCol0;
	TexCoord = aTex;
};