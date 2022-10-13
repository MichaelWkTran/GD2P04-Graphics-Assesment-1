/************************************************************************************************************************/
/*	NAME: Michael Wai Kit Tran																							*/
/*	ORGN: Bachelor of Software Engineering, Media Design School															*/
/*	FILE: Unlit.vert																									*/
/*  DATE: Aug 23rd, 2022																								*/
/************************************************************************************************************************/

#version 430 core

uniform mat4 lightVPMatrix;
uniform mat4 model;

void main()
{
	gl_Position = lightVPMatrix * model * vec4(position, 1.0);
}