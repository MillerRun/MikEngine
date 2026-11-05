#pragma once

#include <glad/glad.h>
#include <string_view>

class Shader
{
public:
   explicit Shader( const std::string_view a_sShaderName );
   void Activate();
   void Deactivate();
   [[nodiscard]] GLuint GetID() const;

private:
   void CheckForErrors( const GLuint a_iShaderID, const std::string_view a_sShaderType ) const;

private:
   GLuint m_iID = 0;
};