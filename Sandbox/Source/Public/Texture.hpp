#pragma once

#include <glad/glad.h>

#include <string_view>

class Shader;

class Texture
{
public:
   explicit Texture( const std::string_view a_sImageName, const GLenum a_eType, const GLenum a_eSlot, const GLenum a_eFormat, const GLenum a_ePixelType );

   void TextureUnit( Shader& a_Shader, const std::string_view a_sUniformName, GLuint a_iUnit );
   void Bind();
   void Unbind();
   void Delete();

private:
   GLuint m_iID = 0;
   GLenum m_eType = 0;
};