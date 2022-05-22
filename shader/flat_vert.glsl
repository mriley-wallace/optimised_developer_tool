#version 460


layout (location = 0) in vec3 VertexPosition;


uniform mat4 MV;
uniform mat4 ProjectionMatrix;

void main()
{
    gl_Position = ProjectionMatrix * MV * vec4(VertexPosition,1.0);
}