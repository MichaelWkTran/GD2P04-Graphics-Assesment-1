/************************************************************************************************************************/
/*	NAME: Michael Wai Kit Tran																							*/
/*	ORGN: Bachelor of Software Engineering, Media Design School															*/
/*	FILE: Diffuse.Frag																									*/
/*  DATE: Aug 23rd, 2022																								*/
/************************************************************************************************************************/

#version 430 core

layout (location = 0) in vec3 in_v3Position;

void main()
{
	gl_Position = vec4(in_v3Position, 1.0f);
}