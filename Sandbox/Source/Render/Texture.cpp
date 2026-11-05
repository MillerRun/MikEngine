#include "Texture.hpp"

#include "Common.hpp"
#include "Shader.hpp"

#include <string>
#include <stb/stb_image.h>

Texture::Texture( const std::string_view a_sImageName, const EType a_eType, const GLenum a_eSlot, const GLenum a_eFormat, const GLenum a_ePixelType )
   : m_eType( a_eType )
   , m_iUnit( a_eSlot )
{
   const std::string sFullPath = "../Resources/" + std::string( a_sImageName );
   int iImageWidth, iImageHeigth, iColorChannels;
   stbi_set_flip_vertically_on_load( true );
   auto pBytes = stbi_load( sFullPath.c_str(), &iImageWidth, &iImageHeigth, &iColorChannels, STBI_default );

   GLCHECK( glGenTextures( 1, &m_iID ) );
   GLCHECK( glActiveTexture( GL_TEXTURE0 + a_eSlot ) );
   GLCHECK( glBindTexture( GL_TEXTURE_2D, m_iID ) );
   
   GLCHECK( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR ) );
   GLCHECK( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST ) );

   GLCHECK( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT ) );
   GLCHECK( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT ) );

   GLCHECK( glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, iImageWidth, iImageHeigth, 0, a_eFormat, a_ePixelType, pBytes ) );
   GLCHECK( glGenerateMipmap( GL_TEXTURE_2D ) );

   GLCHECK( glBindTexture( GL_TEXTURE_2D, 0 ) );

   stbi_image_free( pBytes );
   pBytes = nullptr;
}

void Texture::TextureUnit( Shader& a_Shader, const std::string_view a_sUniformName, const GLuint a_iUnit )
{
   GLuint uTex = glGetUniformLocation( a_Shader.GetID(), a_sUniformName.data() );
   a_Shader.Activate();
   GLCHECK( glUniform1i( uTex, a_iUnit ) );
}

void Texture::Bind()
{
   GLCHECK( glActiveTexture( GL_TEXTURE0 + m_iUnit ) );
   GLCHECK( glBindTexture( GL_TEXTURE_2D, m_iID ) );
}

void Texture::Unbind()
{
   GLCHECK( glBindTexture( GL_TEXTURE_2D, 0 ) );
}

void Texture::Delete()
{
   GLCHECK( glDeleteTextures( 1, &m_iID ) );
}

Texture::EType Texture::GetType() const
{
    return m_eType;
}
