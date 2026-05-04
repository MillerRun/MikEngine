#version 330 core

layout (location = 0) in vec3 a_v3Pos;
layout (location = 1) in vec3 a_v3Color;

out vec3 v3Color;

void main()
{
   gl_Position = vec4( a_v3Pos, 1.0 );
   v3Color = a_v3Color;
}
