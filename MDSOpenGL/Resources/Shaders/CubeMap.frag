#version 430 core

out vec4 FragColor;

in vec3 m_v3Position;
in vec3 m_v3Color;

uniform samplerCube uni_sampCube;

void main()
{
	FragColor = texture(uni_sampCube, m_v3Position) * vec4(m_v3Color, 1.0);
}