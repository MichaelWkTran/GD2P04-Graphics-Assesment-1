#version 430 core

out vec4 fs_v4Colour;

in vec3 vs_v3Position;

uniform sampler3D uni_samp3DDiffuse0;

void main()
{
	fs_v4Colour =  texture(uni_samp3DDiffuse0, (vs_v3Position/2.0f) + 0.5f);
}