#version 430 core

out vec4 FragColor;

in vec3 m_v3CurrentPosition;
in vec3 m_v3Normal;
in vec2 m_v2TextureCoord;
in vec3 m_v3Color;

#define DIFFUSE_NUMBER 1
uniform sampler2D uni_samp2DDiffuse[DIFFUSE_NUMBER];
#define SPECULAR_NUMBER 1
uniform sampler2D uni_samp2DSpecular[SPECULAR_NUMBER];

uniform vec3 uni_v3CameraPosition;

uniform vec3 uni_v3LightPosition;
uniform vec4 uni_v4LightColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
uniform float fAmbientStrength = 0.3f;
uniform float uni_fSpecularStrength = 0.5f;
uniform float uni_fShininess = 16.0f;

vec4 DiffuseColour()
{
	vec4 v4Colour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	for (int iTextureNumber = 0; iTextureNumber < DIFFUSE_NUMBER; iTextureNumber++) v4Colour *= texture(uni_samp2DDiffuse[iTextureNumber], m_v2TextureCoord);

	return v4Colour * vec4(m_v3Color, 1.0f);
}

vec4 SpecularColour()
{
	vec4 v4Colour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	for (int iTextureNumber = 0; iTextureNumber < SPECULAR_NUMBER; iTextureNumber++) v4Colour *= texture(uni_samp2DSpecular[iTextureNumber], m_v2TextureCoord);

	return v4Colour;
}

vec4 PointLight()
{
	vec3 v3LightVec = uni_v3LightPosition - m_v3CurrentPosition;
	float fLightDistance = length(v3LightVec);
	float fA = 3.0f;
	float fB = 0.7f;
	float fIntensity = 1.0f / ((fA * fLightDistance * fLightDistance) + (fB * fLightDistance) + 1.0f);

	vec3 v3Normal = normalize(m_v3Normal);
	vec3 v3lightDirection = normalize(v3LightVec);
	float fDiffuse = max(dot(v3Normal, v3lightDirection), 0.0f);

	vec3 v3ViewDirection = normalize(uni_v3CameraPosition - m_v3CurrentPosition);
	vec3 v3ReflectionDirection = reflect(-v3lightDirection, v3Normal);
	float fSpecAmount = pow(max(dot(v3ViewDirection, v3ReflectionDirection), 0.0f), uni_fShininess);
	float fSpecular = fSpecAmount * uni_fSpecularStrength;

	return (DiffuseColour() * ((fDiffuse * fIntensity) + fAmbientStrength)) + (SpecularColour().r * fSpecular * fIntensity);
}

vec4 InfinitePointLight()
{
	vec3 v3LightVec = uni_v3LightPosition - m_v3CurrentPosition;
	float fLightDistance = length(v3LightVec);
	float fA = 3.0f;
	float fB = 0.7f;
	float fIntensity = 1.0f / ((fA * fLightDistance * fLightDistance) + (fB * fLightDistance) + 1.0f);

	vec3 v3Normal = normalize(m_v3Normal);
	vec3 v3lightDirection = normalize(v3LightVec);
	float fDiffuse = max(dot(v3Normal, v3lightDirection), 0.0f);

	vec3 v3ViewDirection = normalize(uni_v3CameraPosition - m_v3CurrentPosition);
	vec3 v3ReflectionDirection = reflect(-v3lightDirection, v3Normal);
	float fSpecAmount = pow(max(dot(v3ViewDirection, v3ReflectionDirection), 0.0f), uni_fShininess);
	float fSpecular = fSpecAmount * uni_fSpecularStrength;

	return (DiffuseColour() * (fDiffuse + fAmbientStrength)) + (SpecularColour().r * fSpecular);
}

vec4 DirectionalLight()
{
	vec3 v3Normal = normalize(m_v3Normal);
	vec3 v3lightDirection = normalize(vec3(-1.0f, -1.0f, -1.0f));
	float fDiffuse = max(dot(v3Normal, v3lightDirection), 0.0f);

	vec3 v3ViewDirection = normalize(uni_v3CameraPosition - m_v3CurrentPosition);
	vec3 v3ReflectionDirection = reflect(-v3lightDirection, v3Normal);
	float fSpecAmount = pow(max(dot(v3ViewDirection, v3ReflectionDirection), 0.0f), uni_fShininess);
	float fSpecular = fSpecAmount * uni_fSpecularStrength;

	return (DiffuseColour() * (fDiffuse + fAmbientStrength)) * (SpecularColour().r * fSpecular * uni_v4LightColor);
}

vec4 SpotLight()
{
	float fOuterCone = 0.9f;
	float fInnerCone = 0.95f;

	vec3 v3Normal = normalize(m_v3Normal);
	vec3 v3lightDirection = normalize(uni_v3LightPosition - m_v3CurrentPosition);
	float fDiffuse = max(dot(v3Normal, v3lightDirection), 0.0f);

	vec3 v3ViewDirection = normalize(uni_v3CameraPosition - m_v3CurrentPosition);
	vec3 v3ReflectionDirection = reflect(-v3lightDirection, v3Normal);
	float fSpecAmount = pow(max(dot(v3ViewDirection, v3ReflectionDirection), 0.0f), uni_fShininess);
	float fSpecular = fSpecAmount * uni_fSpecularStrength;

	float fAngle = dot(vec3(0.0f, -1.0f, 0.0f), -v3lightDirection);
	float fIntensity = clamp((fAngle - fOuterCone) / (fInnerCone - fOuterCone), 0.0f, 1.0f);

	return (DiffuseColour() * uni_v4LightColor * ((fDiffuse * fIntensity) + fAmbientStrength)) + (SpecularColour().r * fSpecular * fIntensity);
}

void main()
{
	float fAmbient = 0.3f;

	FragColor = PointLight();
}