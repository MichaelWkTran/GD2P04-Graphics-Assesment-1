#version 430 core

out vec4 FragColor;

in vec2 m_v2Position;
in float m_fOffset;
in float m_fWidth;
in vec2 m_v2TextureCoord; 

uniform sampler2D uni_samp2DTextTexture;
uniform vec3 uni_v3Color; 
uniform float uni_fRootXPosition;

out vec4 FinalColor; 

void main()
{
	float fAlpha = texture(uni_samp2DTextTexture, m_v2TextureCoord).r;
	
	if (m_v2Position.x < uni_fRootXPosition || m_v2Position.x > uni_fRootXPosition + m_fOffset + m_fWidth) fAlpha = 0;

	FragColor = vec4(uni_v3Color, fAlpha); 
}