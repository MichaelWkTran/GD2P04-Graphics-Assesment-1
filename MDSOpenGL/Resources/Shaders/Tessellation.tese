#version 430 core

layout (triangles, equal_spacing, ccw) in;

uniform mat4 uni_mat4CameraMatrix;
uniform sampler2D uni_samp2DHeightMap;

in vec2 TextureCoord[];
//out vec2 uvs;

void main(void)
{
	vec2 uvs = (gl_TessCoord.x * TextureCoord[0]) + 
		(gl_TessCoord.y * TextureCoord[1]) + 
		(gl_TessCoord.z * TextureCoord[2]);

	gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position) + 
		(gl_TessCoord.y * gl_in[1].gl_Position) + 
		(gl_TessCoord.z * gl_in[2].gl_Position);

	gl_Position += vec3(0.0f, texture(uni_samp2DHeightMap, uvs).y, 0.0f);
	gl_Position *= uni_mat4CameraMatrix;
}