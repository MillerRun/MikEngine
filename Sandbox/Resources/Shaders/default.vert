#version 330 core

layout (location = 0) in vec3 v3Pos;

void main()
{
   gl_Position = vec4( v3Pos, 1.0 );
}
