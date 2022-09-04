#version 430 core

layout (location = 0) in vec3 in_v3Position;

uniform mat4 uni_mat4CameraMatrix;
uniform mat4 uni_mat4Model;

out vec3 vs_v3Position;

void main()
{
	vs_v3Position = vec3(uni_mat4Model * vec4(in_v3Position, 1.0f));
	gl_Position = uni_mat4CameraMatrix * vec4(vs_v3Position, 1.0);
}