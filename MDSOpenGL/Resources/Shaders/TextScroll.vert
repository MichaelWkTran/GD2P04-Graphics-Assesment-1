#version 430 core

layout (location = 0) in vec4 in_v4Vertex; // (vec2:Position, vec2:TexCoords)

out vec2 a_v2TextureCoord;

uniform float uni_fScroll;

void main()
{
	a_v2TextureCoord = in_v4Vertex.zw;
	gl_Position = vec4(in_v4Vertex.xy + vec2(uni_fScroll, 0.0f), 0.0f, 1.0f);
}