#include "Texture.hpp"

#include "Shader.hpp"

#include <string>
#include <stb/stb_image.h>

Texture::Texture( const std::string_view a_sImageName, const GLenum a_eType, const GLenum a_eSlot, const GLenum a_eFormat, const GLenum a_ePixelType )
   : m_eType( a_eType )
{
   const std::string sFullPath = "../Resources/" + std::string( a_sImageName );
   int iImageWidth, iImageHeigth, iColorChannels;
   stbi_set_flip_vertically_on_load( true );
   stbi_uc * const pBytes = stbi_load( sFullPath.c_str(), &iImageWidth, &iImageHeigth, &iColorChannels, STBI_rgb_alpha);

   glGenTextures( 1, &m_iID );
   glActiveTexture( a_eSlot );
   glBindTexture( a_eType, m_iID );

   glTexParameteri( a_eType, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR );
   glTexParameteri( a_eType, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

   glTexParameteri( a_eType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
   glTexParameteri( a_eType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );

   glTexImage2D( a_eType, 0, GL_RGBA, iImageWidth, iImageHeigth, 0, a_eFormat, a_ePixelType, pBytes );
   glGenerateMipmap( a_eType );

   stbi_image_free( pBytes );
   glBindTexture( a_eType, 0 );
}

void Texture::TextureUnit( Shader& a_Shader, const std::string_view a_sUniformName, GLuint a_iUnit )
{
   GLuint uTex = glGetUniformLocation( a_Shader.GetID(), a_sUniformName.data() );
   a_Shader.Activate();
   glUniform1i( uTex, a_iUnit );
}

void Texture::Bind()
{
   glBindTexture( m_eType, m_iID );
}

void Texture::Unbind()
{
   glBindTexture( m_eType, 0 );
}

void Texture::Delete()
{
   glDeleteTextures( 1, &m_iID );
}
