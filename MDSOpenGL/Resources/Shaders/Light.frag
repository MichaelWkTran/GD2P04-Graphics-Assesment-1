#version 430 core

out vec4 m_v4FragColor;

uniform vec4 uni_v4LightColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);

void main()
{
	m_v4FragColor = uni_v4LightColor;
}