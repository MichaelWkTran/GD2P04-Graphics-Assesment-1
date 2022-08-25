#version 430 core

layout (location = 0) in vec3 in_v3Position;
layout (location = 3) in vec3 in_v3Color;

out vec3 m_v3Position;
out vec3 m_v3Color;

uniform vec3 uni_v3CameraPosition;
uniform mat4 uni_mat4CameraMatrix;

void main()
{
	m_v3Position = in_v3Position;
	m_v3Color = in_v3Color;

	gl_Position = uni_mat4CameraMatrix * vec4(in_v3Position + uni_v3CameraPosition, 1.0);
}