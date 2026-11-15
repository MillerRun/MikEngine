#pragma once

#include "Resource.hpp"
#include <glad/glad.h>
#include <string_view>

void ShaderDeleter( const MK::Render::Handle a_Handle );

class Shader : public MK::Render::Resource<ShaderDeleter>
{
public:
   explicit Shader( const std::string_view a_sShaderName );
   void Activate();

private:
   static void CheckForErrors( const GLuint a_iShaderID, const std::string_view a_sShaderType );
};