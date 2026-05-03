#pragma once

#include <glad/glad.h>
#include <string_view>

class Shader
{
public:
   explicit Shader( std::string_view a_sShaderName );
   void Activate() const;
   void Deactivate() const;
   [[nodiscard]] GLuint GetID() const;

private:
   GLuint m_iID = 0;
};