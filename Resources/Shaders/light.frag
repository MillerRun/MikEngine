#version 330 core

out vec4 FragColor;

uniform vec4 u_v4LightColor;

void main()
{
   FragColor = u_v4LightColor;
}
