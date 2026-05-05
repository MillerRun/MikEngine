#version 330 core

layout (location = 0) in vec3 a_v3Pos;
layout (location = 1) in vec3 a_v3Color;
layout (location = 2) in vec2 a_v2Tex;

out vec3 v3Color;
out vec2 v2TexCoords;

uniform float u_fScale;

void main()
{
   gl_Position = vec4( a_v3Pos * u_fScale, 1.0 );
   v3Color = a_v3Color;
   v2TexCoords = a_v2Tex;
}
