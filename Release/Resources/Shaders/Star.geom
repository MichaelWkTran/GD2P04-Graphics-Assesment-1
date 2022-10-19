#version 430 core

layout (points) in;
layout (triangle_strip, max_vertices = 20) out;

in VS_GS_VERTEX
{
    in vec4 v4Position;
    in mat4 mat4CameraMatrix;
} gs_in[];

out vec2 v2TextureCoordinate;

struct GeometryVertex
{
    vec3 v3Position;
    vec2 v2TextureCoordinate;
};

void main()
{
    GeometryVertex GeometryVerticies[11];
    
    //Set vertex positions
    GeometryVerticies[0].v3Position  = vec3( 0.0f, 8.0f, 0.0f) / 3.0f;
    GeometryVerticies[1].v3Position  = vec3( 2.5f, 3.0f, 0.0f) / 3.0f;
    GeometryVerticies[2].v3Position  = vec3( 7.5f, 2.5f, 0.0f) / 3.0f;
    GeometryVerticies[3].v3Position  = vec3( 4.0f,-1.0f, 0.0f) / 3.0f;
    GeometryVerticies[4].v3Position  = vec3( 5.5f,-6.0f, 0.0f) / 3.0f;
    GeometryVerticies[5].v3Position  = vec3( 0.0f,-3.5f, 0.0f) / 3.0f;
    GeometryVerticies[6].v3Position  = vec3(-5.5f,-6.0f, 0.0f) / 3.0f;
    GeometryVerticies[7].v3Position  = vec3(-4.0f,-1.0f, 0.0f) / 3.0f;
    GeometryVerticies[8].v3Position  = vec3(-7.5f, 2.5f, 0.0f) / 3.0f;
    GeometryVerticies[9].v3Position  = vec3(-2.5f, 3.0f, 0.0f) / 3.0f;
    GeometryVerticies[10].v3Position = vec3( 0.0f, 0.0f, 0.0f) / 3.0f;
    
    //Set texture coordinates
    GeometryVerticies[0].v2TextureCoordinate  = vec2( 0.5f,   1.0f);
    GeometryVerticies[1].v2TextureCoordinate  = vec2( 0.667f, 0.643f);
    GeometryVerticies[2].v2TextureCoordinate  = vec2( 1.0f,   0.607f);
    GeometryVerticies[3].v2TextureCoordinate  = vec2( 0.768f, 0.357f);
    GeometryVerticies[4].v2TextureCoordinate  = vec2( 0.868f, 0.0f);
    GeometryVerticies[5].v2TextureCoordinate  = vec2( 0.5f,   0.178f);
    GeometryVerticies[6].v2TextureCoordinate  = vec2( 0.132f, 0.0f);
    GeometryVerticies[7].v2TextureCoordinate  = vec2( 0.232f, 0.357f);
    GeometryVerticies[8].v2TextureCoordinate  = vec2( 0.0f,   0.607f);
    GeometryVerticies[9].v2TextureCoordinate  = vec2( 0.333f, 0.643f);
    GeometryVerticies[10].v2TextureCoordinate = vec2( 0.5f,   0.429f);

    //Create Triangle Segment
    for (int segmentIndex = 0; segmentIndex < 5; segmentIndex++)
    {
        int iVertexIndex = segmentIndex * 2;
        gl_Position = gs_in[0].mat4CameraMatrix * vec4(GeometryVerticies[iVertexIndex].v3Position, 0.0f);
        gl_Position += gs_in[0].v4Position;
        v2TextureCoordinate = GeometryVerticies[iVertexIndex].v2TextureCoordinate;
        EmitVertex();

        if (segmentIndex == 0) { iVertexIndex = 9; }
        else { iVertexIndex = (segmentIndex * 2) - 1; }
        gl_Position = gs_in[0].mat4CameraMatrix * vec4(GeometryVerticies[iVertexIndex].v3Position, 0.0f);
        gl_Position += gs_in[0].v4Position;
        v2TextureCoordinate = GeometryVerticies[iVertexIndex].v2TextureCoordinate;
        EmitVertex();

        iVertexIndex = (segmentIndex * 2) + 1;
        gl_Position = gs_in[0].mat4CameraMatrix * vec4(GeometryVerticies[iVertexIndex].v3Position, 0.0f);
        gl_Position += gs_in[0].v4Position;
        v2TextureCoordinate = GeometryVerticies[iVertexIndex].v2TextureCoordinate;
        EmitVertex();

        iVertexIndex = 10;
        gl_Position = gs_in[0].mat4CameraMatrix * vec4(GeometryVerticies[iVertexIndex].v3Position, 0.0f);
        gl_Position += gs_in[0].v4Position;
        v2TextureCoordinate = GeometryVerticies[iVertexIndex].v2TextureCoordinate;
        EmitVertex();

        EndPrimitive();
    }
}