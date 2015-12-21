#version 120

uniform int vertexCount;

void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_FrontColor = gl_Color;
    
    gl_PointSize = 20*gl_Normal.x;
}