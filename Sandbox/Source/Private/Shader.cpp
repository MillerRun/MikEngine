#include "Shader.hpp"

#include "Common.hpp"

#include <fstream>
#include <string>
#include <print>

namespace
{
   constexpr const char *k_sShadersPath = "../Resources/Shaders/";
   constexpr const char *k_sVertexShaderFileResolution = ".vert";
   constexpr const char *k_sFragmentShaderFileResolution = ".frag";

   [[nodiscard]]
   std::string GetFileContent( std::string_view a_sFileName )
   {
      if( std::ifstream file{ std::string{ k_sShadersPath } + a_sFileName.data(), std::ios::binary } )
      {
         std::string sContent;
         file.seekg( 0, std::ios::end );
         sContent.resize( file.tellg() );
         file.seekg( 0, std::ios::beg );
         file.read( sContent.data(), sContent.size() );
         return sContent;
      }
      std::string sErrorMessage = "Failed to open shader file" + std::string{ k_sShadersPath } + std::string{ a_sFileName };
      throw std::runtime_error( sErrorMessage );
   }
}

Shader::Shader( const std::string_view a_sShaderName )
{
   const auto sVertexShaderContent = GetFileContent( a_sShaderName.data() + std::string{ k_sVertexShaderFileResolution } );
   const auto sFragmentShaderContent = GetFileContent( a_sShaderName.data() + std::string{ k_sFragmentShaderFileResolution } );

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
         __debugbreak();
      }
   }
   else
   {
      GLCHECK( glGetShaderiv( a_iShaderID, GL_COMPILE_STATUS, &bHasCompiled ) );
      if( bHasCompiled == GL_FALSE )
      {
         GLCHECK( glGetShaderInfoLog( a_iShaderID, sizeof( sInfoMessage ), nullptr, sInfoMessage ) );
         std::println( stderr, "{}", sInfoMessage );
         __debugbreak();
      }
   }
}
