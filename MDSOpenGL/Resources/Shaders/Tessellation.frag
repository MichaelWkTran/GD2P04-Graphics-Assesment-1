/************************************************************************************************************************/
/*	NAME: Michael Wai Kit Tran																							*/
/*	ORGN: Bachelor of Software Engineering, Media Design School															*/
/*	FILE: Diffuse.Frag																									*/
/*  DATE: Aug 23rd, 2022																								*/
/************************************************************************************************************************/

#version 430 core

out vec4 color;
uniform sampler2D uni_samp2DHeightMap;
in vec2 uvs;
in vec3 vs_v3Position;
in vec3 vs_v3Normal;

//Light Structs
struct stDirectionalLight
{
	vec3 v3LightDirection;
	vec4 v4LightColour;
	mat4 mat4VPMatrix;
	sampler2D samp2DShadowMap;
};

//Light Uniforms
uniform vec3 uni_v3CameraPosition;
uniform vec4 uni_v4AmbientColour;

#define MAX_DIRECTIONAL_LIGHT 10
uniform int uni_iDirectionalLightNum = 0;
uniform stDirectionalLight uni_DirectionalLight[MAX_DIRECTIONAL_LIGHT];

float g_fShadow = 0;

void DirectionalLight(stDirectionalLight _Light)
{
	vec3 v3LightDirection = -normalize(_Light.v3LightDirection);
	mat4 mat4VPMatrix = _Light.mat4VPMatrix;

	//Calculate Shadows
	vec4 fragPosLightSpace = mat4VPMatrix * vec4(vs_v3Position, 1.0f);

	// Perspective Divide To get NDC [-1, 1]
	vec3 ndcSpace = fragPosLightSpace.xyz/fragPosLightSpace.w;

	// Sets ndcSpace to cull space
	if(ndcSpace.z <= 1.0f)
	{
		// Get from [-1, 1] range to [0, 1] range just like the shadow map
		ndcSpace = (ndcSpace + 1.0f) / 2.0f;
		float currentDepth = ndcSpace.z;
		// Prevents shadow acne
		float bias = max(0.025f * (1.0f - dot(vs_v3Normal, v3LightDirection)), 0.0005f);

		// Smoothens out the shadows
		int sampleRadius = 2;
		vec2 pixelSize = 1.0 / textureSize(_Light.samp2DShadowMap, 0);
		for(int y = -sampleRadius; y <= sampleRadius; y++)
		{
		    for(int x = -sampleRadius; x <= sampleRadius; x++)
		    {
		        float closestDepth = texture(_Light.samp2DShadowMap, ndcSpace.xy + vec2(x, y) * pixelSize).r;
				if (currentDepth > closestDepth + bias)
					g_fShadow += 1.0f;     
		    }    
		}
		// Get average shadow
		g_fShadow /= pow((sampleRadius * 2 + 1), 2);
	}
}

void main()
{
	color =  mix(vec4(1.0f, 0.0f, 0.0f, 1.0f), vec4(0.0f, 1.0f, 0.0f, 1.0f), texture(uni_samp2DHeightMap, uvs).y);
	color *= 1.0f - g_fShadow;
}