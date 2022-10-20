#version 430 core

layout (triangles, equal_spacing, ccw) in;

uniform mat4 uni_mat4CameraMatrix;
uniform sampler2D uni_samp2DHeightMap;

in vec2 TextureCoord[];

//out sampler2D samp2DHeightMap;
out vec2 uvs;
out vec3 vs_v3Position;
out vec3 vs_v3Normal;

void main(void)
{
	uvs = (gl_TessCoord.x * TextureCoord[0]) + 
		  (gl_TessCoord.y * TextureCoord[1]) + 
		  (gl_TessCoord.z * TextureCoord[2]);

	gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position) + 
				  (gl_TessCoord.y * gl_in[1].gl_Position) + 
				  (gl_TessCoord.z * gl_in[2].gl_Position);

	//samp2DHeightMap = uni_samp2DHeightMap;
	gl_Position += vec4(0.0f, texture(uni_samp2DHeightMap, uvs).y * 50.0f, 0.0f, 0.0f);

	vs_v3Position = gl_Position.xyz;
	vs_v3Normal = normalize(cross(gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz, gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz));
	gl_Position = uni_mat4CameraMatrix * gl_Position;
}