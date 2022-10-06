#version 430 core

layout (points) in;
layout (triangle_strip, max_vertices = 20) out;

in VS_GS_VERTEX
{
    in vec4 v3Position;
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
    GeometryVerticies[0].v3Position  = vec3( 0.0f, 8.0f, 0.0f);
    GeometryVerticies[1].v3Position  = vec3( 2.5f, 3.0f, 0.0f);
    GeometryVerticies[2].v3Position  = vec3( 7.5f, 2.5f, 0.0f);
    GeometryVerticies[3].v3Position  = vec3( 4.0f,-1.0f, 0.0f);
    GeometryVerticies[4].v3Position  = vec3( 5.5f,-6.0f, 0.0f);
    GeometryVerticies[5].v3Position  = vec3( 0.0f,-3.5f, 0.0f);
    GeometryVerticies[6].v3Position  = vec3(-5.5f,-6.0f, 0.0f);
    GeometryVerticies[7].v3Position  = vec3(-4.0f,-1.0f, 0.0f);
    GeometryVerticies[8].v3Position  = vec3(-7.5f, 2.5f, 0.0f);
    GeometryVerticies[9].v3Position  = vec3(-2.5f, 3.0f, 0.0f);
    GeometryVerticies[10].v3Position = vec3( 0.0f, 0.0f, 0.0f);
    
    //Set texture coordinates
    GeometryVerticies[0].v2TextureCoordinate  = vec2( 0.5f,   1.0f);
    GeometryVerticies[1].v2TextureCoordinate  = vec2( 0.667f, 0.643f);
    GeometryVerticies[2].v2TextureCoordinate  = vec2( 1.0f,   6.07f);
    GeometryVerticies[3].v2TextureCoordinate  = vec2( 0.768f, 0.357f);
    GeometryVerticies[4].v2TextureCoordinate  = vec2( 0.868f, 0.0f);
    GeometryVerticies[5].v2TextureCoordinate  = vec2( 0.5f,   0.178f);
    GeometryVerticies[6].v2TextureCoordinate  = vec2( 0.132f, 0.0f);
    GeometryVerticies[7].v2TextureCoordinate  = vec2( 0.232f, 0.357f);
    GeometryVerticies[8].v2TextureCoordinate  = vec2( 0.0f,   0.607f);
    GeometryVerticies[9].v2TextureCoordinate  = vec2( 0.333f, 0.643f);
    GeometryVerticies[10].v2TextureCoordinate = vec2( 0.0f,   0.0f);

    //Create Triangle Segment
    for (int segmentIndex = 0; segmentIndex < 5; segmentIndex++)
    {
        gl_Position = gs_in[0].mat4CameraMatrix * vec4(GeometryVerticies[segmentIndex * 2].v3Position, 0.0f);
        v2TextureCoordinate = GeometryVerticies[segmentIndex * 2].v2TextureCoordinate;

        if (segmentIndex == 0)
        {
            gl_Position = gs_in[0].mat4CameraMatrix * vec4(GeometryVerticies[9].v3Position, 0.0f);
            v2TextureCoordinate = GeometryVerticies[9].v2TextureCoordinate;
        }
        else
        {
            gl_Position = gs_in[0].mat4CameraMatrix * vec4(GeometryVerticies[(segmentIndex * 2) - 1].v3Position, 0.0f);
            v2TextureCoordinate = GeometryVerticies[(segmentIndex * 2) - 1].v2TextureCoordinate;
        }

        gl_Position = gs_in[0].mat4CameraMatrix * vec4(GeometryVerticies[(segmentIndex * 2) + 1].v3Position, 0.0f);
        v2TextureCoordinate = GeometryVerticies[(segmentIndex * 2) + 1].v2TextureCoordinate;

        gl_Position = gs_in[0].mat4CameraMatrix * vec4(GeometryVerticies[10].v3Position, 0.0f);
        v2TextureCoordinate = GeometryVerticies[10].v2TextureCoordinate;

        EmitVertex();
    }

    ////Create top triangle
    //gl_Position = gs_in[0].v3Position + (gs_in[0].mat4CameraMatrix * vec4( 0.0f, 8.0f, 0.0f, 0.0f));  //Vertex 0
    //    v2TextureCoordinate = vec2(0.5f, 1.0f);
    //    EmitVertex();
    //gl_Position = gs_in[0].v3Position + (gs_in[0].mat4CameraMatrix * vec4(-2.5f, 3.0f, 0.0f, 0.0f)); //Vertex 9
    //    v2TextureCoordinate = vec2(0.0, 0.0);
    //    EmitVertex();
    //gl_Position = gs_in[0].v3Position + (gs_in[0].mat4CameraMatrix * vec4( 2.5f, 3.0f, 0.0f, 0.0f)); //Vertex 1
    //    v2TextureCoordinate = vec2(0.0f, 0.0f);
    //    EmitVertex();
    //gl_Position = gs_in[0].v3Position + (gs_in[0].mat4CameraMatrix * vec4( 0.0f, 0.0f, 0.0f, 0.0f)); //Vertex 10
    //    v2TextureCoordinate = vec2(0.0f, 0.0f);
    //    EmitVertex();
    //EndPrimitive();
//
    ////Create top right triangle
    //gl_Position = gs_in[0].v3Position + (gs_in[0].mat4CameraMatrix * vec4( 7.5f, 2.5f, 0.0f, 0.0f)); //Vertex 2
    //    v2TextureCoordinate = vec2(0.0f, 0.0f);
    //    EmitVertex();
    //gl_Position = gs_in[0].v3Position + (gs_in[0].mat4CameraMatrix * vec4( 2.5f, 3.0f, 0.0f, 0.0f)); //Vertex 1
    //    v2TextureCoordinate = vec2(0.0f, 0.0f);
    //    EmitVertex();
    //gl_Position = gs_in[0].v3Position + (gs_in[0].mat4CameraMatrix * vec4( 4.0f,-1.0f, 0.0f, 0.0f)); //Vertex 3
    //    v2TextureCoordinate = vec2(0.0f, 0.0f);
    //    EmitVertex();
    //gl_Position = gs_in[0].v3Position + (gs_in[0].mat4CameraMatrix * vec4( 0.0f, 0.0f, 0.0f, 0.0f)); //Vertex 9
    //    v2TextureCoordinate = vec2(0.0f, 0.0f);
    //    EmitVertex();
    //EndPrimitive();
//
    ////Create bottom right triangle
    //gl_Position = gs_in[0].v3Position + (gs_in[0].mat4CameraMatrix * vec4( 5.5f,-6.0f, 0.0f, 0.0f)); //Vertex 4
    //    v2TextureCoordinate = vec2(0.0f, 0.0f);
    //    EmitVertex();
    //gl_Position = gs_in[0].v3Position + (gs_in[0].mat4CameraMatrix * vec4( 4.0f,-1.0f, 0.0f, 0.0f)); //Vertex 3
    //    v2TextureCoordinate = vec2(0.0f, 0.0f);
    //    EmitVertex();
    //gl_Position = gs_in[0].v3Position + (gs_in[0].mat4CameraMatrix * vec4( 0.0f,-3.5f, 0.0f, 0.0f)); //Vertex 5
    //    v2TextureCoordinate = vec2(0.0f, 0.0f);
    //    EmitVertex();
    //gl_Position = gs_in[0].v3Position + (gs_in[0].mat4CameraMatrix * vec4( 0.0f, 0.0f, 0.0f, 0.0f)); //Vertex 10
    //    v2TextureCoordinate = vec2(0.0f, 0.0f);
    //    EmitVertex();
    //EndPrimitive();
//
    ////Create bottom left triangle
    //gl_Position = gs_in[0].v3Position + (gs_in[0].mat4CameraMatrix * vec4(-5.5f,-6.0f, 0.0f, 0.0f)); //Vertex 6
    //    v2TextureCoordinate = vec2(0.0f, 0.0f);
    //    EmitVertex();
    //gl_Position = gs_in[0].v3Position + (gs_in[0].mat4CameraMatrix * vec4( 0.0f,-3.5f, 0.0f, 0.0f)); //Vertex 5
    //    v2TextureCoordinate = vec2(0.0f, 0.0f);
    //    EmitVertex();
    //gl_Position = gs_in[0].v3Position + (gs_in[0].mat4CameraMatrix * vec4(-4.0f,-1.0f, 0.0f, 0.0f)); //Vertex 7
    //    v2TextureCoordinate = vec2(0.0f, 0.0f);
    //    EmitVertex();
    //gl_Position = gs_in[0].v3Position + (gs_in[0].mat4CameraMatrix * vec4( 0.0f, 0.0f, 0.0f, 0.0f)); //Vertex 10
    //    v2TextureCoordinate = vec2(0.0f, 0.0f);
    //    EmitVertex();
    //EndPrimitive();
//
    ////Create top left triangle
    //gl_Position = gs_in[0].v3Position + (gs_in[0].mat4CameraMatrix * vec4(-7.5f, 2.5f, 0.0f, 0.0f)); //Vertex 8
    //    v2TextureCoordinate = vec2(0.0f, 0.0f);
    //    EmitVertex();
    //gl_Position = gs_in[0].v3Position + (gs_in[0].mat4CameraMatrix * vec4(-4.0f,-1.0f, 0.0f, 0.0f)); //Vertex 7
    //    v2TextureCoordinate = vec2(0.0f, 0.0f);
    //    EmitVertex();
    //gl_Position = gs_in[0].v3Position + (gs_in[0].mat4CameraMatrix * vec4(-2.5f, 3.0f, 0.0f, 0.0f)); //Vertex 9
    //    v2TextureCoordinate = vec2(0.0f, 0.0f);
    //    EmitVertex();
    //gl_Position = gs_in[0].v3Position + (gs_in[0].mat4CameraMatrix * vec4( 0.0f, 0.0f, 0.0f, 0.0f)); //Vertex 10
    //    v2TextureCoordinate = vec2(0.0f, 0.0f);
    //    EmitVertex();
    //EndPrimitive();
}
