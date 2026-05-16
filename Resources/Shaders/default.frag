#version 330 core

in vec3 f_v3CurrentPosition;
in vec3 f_v3Normal;
in vec3 f_v3Color;
in vec2 f_v2TexCoords;

out vec4 FragColor;

uniform sampler2D u_TexDiffuse0;
uniform sampler2D u_TexSpecular0;

uniform vec4 u_v4LightColor;
uniform vec3 u_v3LightPosition;
uniform vec3 u_v3CameraPosition;

void main()
{
   float fAmbientColor = 0.2f;
   vec3 v3Normalized = normalize( f_v3Normal );
   vec3 v3LightDirection = normalize( u_v3LightPosition - f_v3CurrentPosition );
   float fDiffuse = max( dot( v3Normalized, v3LightDirection ), 0.0f );

   float fSpecularLight = 0.5f;
   vec3 v3ViewDirection = normalize( u_v3CameraPosition - f_v3CurrentPosition );
   vec3 v3ReflectionDirection = reflect( -v3LightDirection, v3Normalized );
   float fSpecularAmount = pow( max( dot( v3ViewDirection, v3ReflectionDirection ), 0.0f ), 16 );
   float fSpecular = fSpecularAmount * fSpecularLight;

   FragColor = ( texture( u_TexDiffuse0, f_v2TexCoords ) * ( fDiffuse + fAmbientColor ) + texture( u_TexSpecular0, f_v2TexCoords ).r * fSpecular ) * u_v4LightColor;
}
