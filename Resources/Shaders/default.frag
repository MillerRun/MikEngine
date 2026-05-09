#version 330 core

in vec3 v3Color;
in vec2 v2TexCoords;
in vec3 v3Normal;
in vec3 v3CurrentPosition;

out vec4 FragColor;

uniform sampler2D u_Tex0;
uniform vec4 u_v4LightColor;
uniform vec3 u_v3LightPosition;
uniform vec3 u_v3CameraPosition;

void main()
{
   float fAmbientColor = 0.1f;
   vec3 v3Normalized = normalize( v3Normal );
   vec3 v3LightDirection = normalize( u_v3LightPosition - v3CurrentPosition );
   float fDiffuse = max( dot( v3Normalized, v3LightDirection ), 0.0f );

   float fSpecularLight = 0.5f;
   vec3 v3ViewDirection = normalize( u_v3CameraPosition - v3CurrentPosition );
   vec3 v3ReflectionDirection = reflect( -v3LightDirection, v3Normal );
   float fSpecularAmount = pow( max( dot( v3ViewDirection, v3ReflectionDirection ), 0.0 ), 8 );
   float fSpecular = fSpecularAmount * fSpecularLight;

   FragColor = texture( u_Tex0, v2TexCoords ) * u_v4LightColor * ( fDiffuse + fAmbientColor + fSpecular );
}
