#version 430 core

out vec4 fs_v4Colour;

in vec3 vs_v3TextureCoord;

uniform samplerCube uni_sampCube;

void main()
{
	fs_v4Colour = texture(uni_sampCube, vs_v3TextureCoord); //vec4(1.0f,1.0f,1.0f,1.0f);// 
}