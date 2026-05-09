#version 330 core

layout (location = 0) in vec3 a_v3Pos;

uniform mat4 u_m4Model;
uniform mat4 u_m4Camera;

void main()
{
   gl_Position = u_m4Camera * u_m4Model * vec4( a_v3Pos, 1.0f );
}
