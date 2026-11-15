#include "Shader.hpp"

#include "Common.hpp"
#include "Utils/Assert.hpp"
#include "Utils/File.hpp"

#include <string>

namespace
{
   constexpr const char *k_sVertexShaderFileResolution = ".vert";
   constexpr const char *k_sFragmentShaderFileResolution = ".frag";
   constexpr const char *k_sShaderSubDir = "Shaders/";

   void CheckForErrors( const GLuint a_iShaderID, const std::string_view a_sShaderType )
   {
      GLint bHasCompiled = 1;
      char sInfoMessage[1024]{};

      if( a_sShaderType == "PROGRAM" )
      {
         GLCHECK( glGetProgramiv( a_iShaderID, GL_LINK_STATUS, &bHasCompiled ) );
         if( bHasCompiled == GL_FALSE )
         {
            GLCHECK( glGetProgramInfoLog( a_iShaderID, sizeof( sInfoMessage ), nullptr, sInfoMessage ) );
            MKASSERT( false, "{}", sInfoMessage);
         }
      }
      else
      {
         GLCHECK( glGetShaderiv( a_iShaderID, GL_COMPILE_STATUS, &bHasCompiled ) );
         if( bHasCompiled == GL_FALSE )
         {
            GLCHECK( glGetShaderInfoLog( a_iShaderID, sizeof( sInfoMessage ), nullptr, sInfoMessage ) );
            MKASSERT( false, "{}", sInfoMessage );
         }
      }
   }
}

namespace MK::Render
{
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
      m_Program = ProgramHandle{ glCreateProgram() };
      GLCHECK( glAttachShader( this->Get(), vs ) ); // attach vertex shader
      GLCHECK( glAttachShader( this->Get(), fs ) ); // attach fragment shader
      GLCHECK( glLinkProgram( this->Get() ) ); // assemble program
      CheckForErrors( this->Get(), "PROGRAM" );

      // delete shaders, because program contains them already
      GLCHECK( glDetachShader( this->Get(), vs ) );
      GLCHECK( glDetachShader( this->Get(), fs ) );
      GLCHECK( glDeleteShader( vs ) );
      GLCHECK( glDeleteShader( fs ) );
   }

   void Shader::Activate()
   {
      GLCHECK( glUseProgram( this->Get() ) );
   }

   auto Shader::Get() const noexcept -> Handle
   {
      return m_Program.Get();
   }
}
