#version 430 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 8) out;

in vec2 a_v2TextureCoord[];

out vec2 m_v2Position;
out float m_fOffset;
out float m_fWidth;
out vec2 m_v2TextureCoord; 

uniform float uni_fOffset;
uniform float uni_fWidth;
uniform mat4 uni_mat4Projection;

void main()
{
	for (int i = 0; i < 4; i++)
	{
		gl_Position = uni_mat4Projection * gl_in[i].gl_Position;
		m_v2Position = gl_in[i].gl_Position.xy;
		m_v2TextureCoord = a_v2TextureCoord[i];
		m_fOffset = uni_fOffset;
		m_fWidth = uni_fWidth;
		EmitVertex();
	}
	
	EndPrimitive();

	for (int i = 0; i < 4; i++)
	{
		m_v2Position = gl_in[i].gl_Position.xy - vec2(uni_fWidth + uni_fOffset, 0.0f);
		gl_Position = uni_mat4Projection * vec4(m_v2Position.xy, gl_in[i].gl_Position.zw);
		m_v2TextureCoord = a_v2TextureCoord[i];
		m_fOffset = uni_fOffset;
		m_fWidth = uni_fWidth;
		EmitVertex();
	}
}