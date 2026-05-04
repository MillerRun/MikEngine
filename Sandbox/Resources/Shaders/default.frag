#version 330 core

in vec3 v3Color;

out vec4 FragColor;

void main()
{
   FragColor = vec4( v3Color, 1.0 );
}
