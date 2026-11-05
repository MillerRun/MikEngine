#include "Shader.hpp"

#include "Common.hpp"
#include "Utils/Assert.hpp"
#include "Utils/File.hpp"

#include <fstream>
#include <string>
#include <print>

namespace
{
   constexpr const char *k_sVertexShaderFileResolution = ".vert";
   constexpr const char *k_sFragmentShaderFileResolution = ".frag";
   constexpr const char *k_sShaderSubDir = "Shaders/";
}

Shader::Shader( const std::string_view a_sShaderName )
{
   const auto GetShaderContent = [a_sShaderName]( const std::string_view a_sResolution ) -> std::string
   {
      auto sFullShaderName = std::format( "{}{}", a_sShaderName, a_sResolution );
      const auto result = MK::File::GetFileContent( k_sShaderSubDir, sFullShaderName );
      if( !result )
      {
         MKASSERT( false, "Failed to read {} shader file with error {}", a_sShaderName, result.error() );
         return {};
      }
      return result.value();
   };
   const auto sVertexShaderContent = GetShaderContent( k_sVertexShaderFileResolution );
   const auto sFragmentShaderContent = GetShaderContent( k_sFragmentShaderFileResolution );

   // vertex shader
   const GLuint vs = glCreateShader( GL_VERTEX_SHADER );
   const char *sVertexSource = sVertexShaderContent.c_str();
   GLCHECK( glShaderSource( vs, 1, &sVertexSource, nullptr ) ); // 1 screen for the shader
   GLCHECK( glCompileShader( vs ) );
   CheckForErrors( vs, "VERTEX" );

   // fragment shader
   const GLuint fs = glCreateShader( GL_FRAGMENT_SHADER );
   const char *sFragmentSource = sFragmentShaderContent.c_str();
   GLCHECK( glShaderSource( fs, 1, &sFragmentSource, nullptr ) );
   GLCHECK( glCompileShader( fs ) );
   CheckForErrors( fs, "FRAGMENT" );

   // create program
   m_iID = glCreateProgram();
   GLCHECK( glAttachShader( m_iID, vs ) ); // attach vertex shader
   GLCHECK( glAttachShader( m_iID, fs ) ); // attach fragment shader
   GLCHECK( glLinkProgram( m_iID ) ); // assemble program
   CheckForErrors( m_iID, "PROGRAM" );

   // delete shaders, because program contains them already
   GLCHECK( glDetachShader( m_iID, vs ) );
   GLCHECK( glDetachShader( m_iID, fs ) );
   GLCHECK( glDeleteShader( vs ) );
   GLCHECK( glDeleteShader( fs ) );
}

void Shader::Activate()
{
   GLCHECK( glUseProgram( m_iID ) );
}

void Shader::Deactivate()
{
   GLCHECK( glDeleteProgram( m_iID ) );
}

GLuint Shader::GetID() const
{
   return m_iID;
}

void Shader::CheckForErrors( const GLuint a_iShaderID, const std::string_view a_sShaderType ) const
{
   GLint bHasCompiled = 1;
   char sInfoMessage[1024]{};

   if( a_sShaderType == "PROGRAM" )
   {
      GLCHECK( glGetProgramiv( a_iShaderID, GL_LINK_STATUS, &bHasCompiled ) );
      if( bHasCompiled == GL_FALSE )
      {
         GLCHECK( glGetProgramInfoLog( a_iShaderID, sizeof( sInfoMessage ), nullptr, sInfoMessage ) );
         std::println( stderr, "{}", sInfoMessage);
         DEBUGBREAK();
      }
   }
   else
   {
      GLCHECK( glGetShaderiv( a_iShaderID, GL_COMPILE_STATUS, &bHasCompiled ) );
      if( bHasCompiled == GL_FALSE )
      {
         GLCHECK( glGetShaderInfoLog( a_iShaderID, sizeof( sInfoMessage ), nullptr, sInfoMessage ) );
         std::println( stderr, "{}", sInfoMessage );
         DEBUGBREAK();
      }
   }
}
