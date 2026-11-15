#pragma once

#include <string_view>
#include <glad/glad.h>

namespace MK::Render
{
   class Shader;
}


class Texture
{
public:
   enum class EType : int
   {
      DIFFUSE,
      SPECULAR,
   };

public:
   explicit Texture( const std::string_view a_sImageName, const EType a_eType, const GLenum a_eSlot, const GLenum a_eFormat, const GLenum a_ePixelType );

   void TextureUnit( MK::Render::Shader &a_Shader, const char* a_sUniformName, const GLuint a_iUnit );
   void Bind();
   void Unbind();
   void Delete();
   [[nodiscard]] EType GetType() const;

private:
   GLuint m_iID = 0;
   GLenum m_iUnit = 0;
   EType m_eType = EType::DIFFUSE;
};