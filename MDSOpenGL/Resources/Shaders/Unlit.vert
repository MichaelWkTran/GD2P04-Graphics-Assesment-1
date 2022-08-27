#version 430 core

layout (location = 0) in vec3 in_v3Position;
layout (location = 2) in vec2 in_v2TextureCoord;

out vec2 vs_v2TextureCoord;

uniform mat4 uni_mat4CameraMatrix;
uniform mat4 uni_mat4Model;

void main()
{
	vs_v2TextureCoord = in_v2TextureCoord;

	gl_Position = uni_mat4CameraMatrix * uni_mat4Model * vec4(in_v3Position, 1.0);
}