#version 430 core

out vec4 fs_v4Colour;

in vec3 vs_v3Position;
in vec3 vs_v3Normal;
in vec2 vs_v2TextureCoord;

//Unique Uniforms---------------------------------------------------------
//Texture Uniforms
uniform sampler2D uni_samp2DDiffuse0;
uniform sampler2D uni_samp2DSpecular0;
uniform sampler2D uni_samp2DReflect0;

//Light Uniforms
uniform float uni_fSpecularStrength = 0.3f;
uniform float uni_fShininess = 16.0f;

uniform float uni_fRimExponent = 16.0f;
uniform vec4 uni_v4RimColour = vec4(1.0f, 1.0f, 1.0f, 0.0f);

uniform float uni_fReflectionStrength = 0;

//Shared Uniforms---------------------------------------------------------
//Texture Uniforms
uniform samplerCube uni_sampCubeSkybox;

//Light Structs
struct stInfinitePointLight
{
	vec3 v3LightPosition;
	vec4 v4LightColour;
};

struct stPointLight
{
	vec3 v3LightPosition;
	vec4 v4LightColour;
	float fAttenuationExponent;
	float fAttenuationLinear;
	float fAttenuationConstant;
};

struct stDirectionalLight
{
	vec3 v3LightDirection;
	vec4 v4LightColour;
};

struct stSpotLight
{
	vec3 v3LightPosition;
	vec3 v3LightDirection;
	vec4 v4LightColour;
	float fOuterCone;
	float fInnerCone;
};

//Light Uniforms
uniform vec3 uni_v3CameraPosition;
uniform vec4 uni_v4AmbientColour;

#define MAX_INF_POINT_LIGHT 10
uniform int uni_iInfPointLightNum = 0;
uniform stInfinitePointLight uni_InfinitePointLight[MAX_INF_POINT_LIGHT];

#define MAX_POINT_LIGHT 10
uniform int uni_iPointLightNum = 0;
uniform stPointLight uni_PointLight[MAX_POINT_LIGHT];

#define MAX_DIRECTIONAL_LIGHT 10
uniform int uni_iDirectionalLightNum = 0;
uniform stDirectionalLight uni_DirectionalLight[MAX_DIRECTIONAL_LIGHT];

#define MAX_SPOT_LIGHT 10
uniform int uni_iSpotLightNum = 0;
uniform stSpotLight uni_SpotLight[MAX_SPOT_LIGHT];

//Fog Uniforms
uniform vec3 uni_v3FogColour = vec3(0.0f, 0.0f, 0.0f);
uniform float uni_fFogStart = 5.0f;
uniform float uni_fFogRange = 10.0f;

//Global Variables
vec3 g_v3Diffuse = vec3(0);
vec3 g_v3Specular = vec3(0);

void UpdateDiffuseSpecular(vec3 _v3LightColour, vec3 _v3LightDirection, float _fIntensity)
{
	//Update Diffuse
	g_v3Diffuse += _v3LightColour * max(dot(vs_v3Normal, _v3LightDirection), 0.0f);

	//Update Specular
	if (_fIntensity <= 0 || uni_fSpecularStrength <= 0) return;

	vec3 v3ViewDirection = normalize(uni_v3CameraPosition - vs_v3Position);
	vec3 v3HalfwayVector = normalize(-_v3LightDirection + v3ViewDirection);
	float fSpecularAmount = pow(max(dot(vs_v3Normal, v3HalfwayVector), 0.0f), uni_fShininess);

	g_v3Specular += vec3(1.0f,1.0f,1.0f) * fSpecularAmount * uni_fSpecularStrength * _fIntensity;
}

vec3 RimColour()
{
	if (uni_v4RimColour.w <= 0) return vec3(0);

	float fRimFactor = 1.0f - dot(vs_v3Normal, uni_v3CameraPosition - vs_v3Position);
	fRimFactor = smoothstep(0.0f, 1.0f, fRimFactor);
	fRimFactor = pow(fRimFactor, uni_fRimExponent);

	return fRimFactor * (uni_v4RimColour.rgb * uni_v4RimColour.a);
}

vec4 MixFogColour(vec4 _v4Colour)
{
	float fCameraDistance = distance(vs_v3Position, uni_v3CameraPosition);

	float fFogContribution = (fCameraDistance - uni_fFogStart)/uni_fFogRange;
	fFogContribution = clamp(fFogContribution, 0.0, 1.0);

	return mix(_v4Colour, vec4(uni_v3FogColour, 1.0f), fFogContribution);
}

void InfinitePointLight(stInfinitePointLight _InfinitePointLight)
{
	vec3 v3LightPosition = _InfinitePointLight.v3LightPosition;
	vec4 v4LightColour = _InfinitePointLight.v4LightColour;
	
	UpdateDiffuseSpecular(v4LightColour.rgb, normalize(v3LightPosition - vs_v3Position), v4LightColour.w);
}

void PointLight(stPointLight _PointLight)
{
	vec3 v3LightPosition = _PointLight.v3LightPosition;
	vec4 v4LightColour = _PointLight.v4LightColour;
	float fAttenuationExponent = _PointLight.fAttenuationExponent;
	float fAttenuationLinear = _PointLight.fAttenuationLinear;
	float fAttenuationConstant = _PointLight.fAttenuationConstant;

	vec3 v3LightVector = v3LightPosition - vs_v3Position;
	float fLightDistance = length(v3LightVector);
	float fIntensity = v4LightColour.w / ((fAttenuationExponent * fLightDistance * fLightDistance) + (fAttenuationLinear * fLightDistance) + fAttenuationConstant);
	
	UpdateDiffuseSpecular(v4LightColour.rgb, normalize(v3LightVector), fIntensity);
}

void DirectionalLight(stDirectionalLight _DirectionalLight)
{
	vec3 v3LightDirection = -normalize(_DirectionalLight.v3LightDirection);
	vec4 v4LightColour = _DirectionalLight.v4LightColour;

	UpdateDiffuseSpecular(v4LightColour.rgb, v3LightDirection, v4LightColour.w);
}

void SpotLight(stSpotLight _SpotLight)
{
	vec3 v3LightPosition = _SpotLight.v3LightPosition;
	vec3 v3SpotLightDirection = normalize(_SpotLight.v3LightDirection);
	vec4 v4LightColour = _SpotLight.v4LightColour;
	float fOuterCone = _SpotLight.fOuterCone;
	float fInnerCone = _SpotLight.fInnerCone;

	vec3 v3LightDirection = normalize(v3LightPosition - vs_v3Position);

	float fAngle = dot(v3SpotLightDirection, -v3LightDirection);
	float fIntensity = v4LightColour.w * clamp((fAngle - fOuterCone) / (fInnerCone - fOuterCone), 0.0f, 1.0f);

	UpdateDiffuseSpecular(v4LightColour.rgb, v3LightDirection, fIntensity);
}

void main()
{
	//Get Texture Colours
	vec4 v4DiffuseTexture = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	if (textureSize(uni_samp2DDiffuse0, 0) != vec2(1,1)) v4DiffuseTexture *= texture(uni_samp2DDiffuse0, vs_v2TextureCoord);
	vec4 v4SpecularTexture = vec4(1.0f,1.0f,1.0f,1.0f);
	 if (textureSize(uni_samp2DSpecular0, 0) != vec2(1,1)) v4SpecularTexture = texture(uni_samp2DSpecular0, vs_v2TextureCoord);
	
	//Calculate Diffuse and Specular Colours	
	for (int i = 0; i < uni_iInfPointLightNum; i++) InfinitePointLight(uni_InfinitePointLight[i]);
	
	for (int i = 0; i < uni_iPointLightNum; i++) PointLight(uni_PointLight[i]);
	
	for (int i = 0; i < uni_iDirectionalLightNum; i++) DirectionalLight(uni_DirectionalLight[i]);

	for (int i = 0; i < uni_iSpotLightNum; i++) SpotLight(uni_SpotLight[i]);

	//Calculate Reflection
	if (textureSize(uni_sampCubeSkybox, 0) != ivec2(1,1) && uni_fReflectionStrength > 0)
	{
		vec3 v3ReflectDirection = reflect(normalize(vs_v3Position - uni_v3CameraPosition), vs_v3Normal);
	
		vec4 v4ReflectTexture = vec4(1.0f, 1.0f, 1.0f, 1.0f);
		if (textureSize(uni_samp2DSpecular0, 0) != vec2(1,1)) v4ReflectTexture = texture(uni_samp2DReflect0, vs_v2TextureCoord);
		
		//Mix with v4DiffuseTexture
		v4DiffuseTexture = mix
		(
			v4DiffuseTexture, 
			texture(uni_sampCubeSkybox, v3ReflectDirection),
			uni_fReflectionStrength * v4ReflectTexture.r
		);
	}

	//Calculate Final Fragment Colour
	fs_v4Colour = vec4(v4DiffuseTexture.rgb * (g_v3Diffuse + (uni_v4AmbientColour.rgb * uni_v4AmbientColour.w)), 1.0f);
	fs_v4Colour += vec4(v4SpecularTexture.rgb * g_v3Specular, 1.0f);
	fs_v4Colour += vec4(RimColour(), 1.0f);
	fs_v4Colour = vec4(MixFogColour(fs_v4Colour).rgb, 1.0f);
}