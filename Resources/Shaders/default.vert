#version 330 core

layout (location = 0) in vec3 a_v3Pos;
layout (location = 1) in vec3 a_v3Color;
layout (location = 2) in vec2 a_v2Tex;
layout (location = 3) in vec3 a_v3Normal;

out vec3 v3Color;
out vec2 v2TexCoords;
out vec3 v3Normal;
out vec3 v3CurrentPosition;

uniform mat4 u_m4Camera;
uniform mat4 u_m4Model;

void main()
{
   v3CurrentPosition = vec3( u_m4Model * vec4( a_v3Pos, 1.0f ) );
   gl_Position = u_m4Camera * vec4( v3CurrentPosition, 1.0f );
   v3Color = a_v3Color;
   v2TexCoords = a_v2Tex;
   v3Normal = a_v3Normal;
}
