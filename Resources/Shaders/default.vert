#version 330 core

layout (location = 0) in vec3 a_v3Pos;
layout (location = 1) in vec3 a_v3Normal;
layout (location = 2) in vec3 a_v3Color;
layout (location = 3) in vec2 a_v2Tex;

out vec3 f_v3CurrentPosition;
out vec3 f_v3Normal;
out vec3 f_v3Color;
out vec2 f_v2TexCoords;

uniform mat4 u_m4Camera;
uniform mat4 u_m4Model;

void main()
{
   f_v3CurrentPosition = vec3( u_m4Model * vec4( a_v3Pos, 1.0 ) );
   f_v3Normal = a_v3Normal;
   f_v3Color = a_v3Color;
   f_v2TexCoords = a_v2Tex;

   gl_Position = u_m4Camera * vec4( f_v3CurrentPosition, 1.0 );
}
