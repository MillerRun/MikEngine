#version 330 core

in vec3 v3Color;
in vec2 v2TexCoords;

out vec4 FragColor;

uniform sampler2D u_Tex0;

void main()
{
   FragColor = texture( u_Tex0, v2TexCoords );
}
