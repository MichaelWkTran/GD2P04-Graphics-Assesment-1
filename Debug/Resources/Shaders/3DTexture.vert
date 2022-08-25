#version 430 core

layout (location = 0) in vec3 in_v3Position;

uniform mat4 uni_mat4CameraMatrix;
uniform mat4 uni_mat4Model;

out vec3 vs_v3Position;

void main()
{
	vs_v3Position = uni_mat4Model.xyz * in_v3Position;
	gl_Position = uni_mat4CameraMatrix * vec4(vs_v3Position, 1.0);
}