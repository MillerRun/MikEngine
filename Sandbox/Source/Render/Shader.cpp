#include "Shader.hpp"

#include "Common.hpp"
#include "Utils/Assert.hpp"
#include "Utils/File.hpp"

#include <string>
#include <print>

namespace
{
   constexpr const char *k_sVertexShaderFileResolution = ".vert";
   constexpr const char *k_sFragmentShaderFileResolution = ".frag";
   constexpr const char *k_sShaderSubDir = "Shaders/";

   void CheckLinkStatus( const GLuint a_iID )
   {
      GLint bHasCompiled = 1;
      char sInfoMessage[1024]{};

      GLCHECK( glGetProgramiv( a_iID, GL_LINK_STATUS, &bHasCompiled ) );
      if( bHasCompiled == GL_FALSE )
      {
         GLCHECK( glGetProgramInfoLog( a_iID, sizeof( sInfoMessage ), nullptr, sInfoMessage ) );
         std::println( stderr, "{}", sInfoMessage );
         DEBUGBREAK();
      }
   }

   void CheckCompileStatus( const GLuint a_iID )
   {
      GLint bHasCompiled = 1;
      char sInfoMessage[1024]{};

      GLCHECK( glGetShaderiv( a_iID, GL_COMPILE_STATUS, &bHasCompiled ) );
      if( bHasCompiled == GL_FALSE )
      {
         GLCHECK( glGetShaderInfoLog( a_iID, sizeof( sInfoMessage ), nullptr, sInfoMessage ) );
         std::println( stderr, "{}", sInfoMessage );
         DEBUGBREAK();
      }
   }
}

namespace MK::Render
{
   Shader::Shader( const std::string_view a_sShaderName )
      : m_Program{ glCreateProgram() }
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
      CheckCompileStatus( vs );

      // fragment shader
      const GLuint fs = glCreateShader( GL_FRAGMENT_SHADER );
      const char *sFragmentSource = sFragmentShaderContent.c_str();
      GLCHECK( glShaderSource( fs, 1, &sFragmentSource, nullptr ) );
      GLCHECK( glCompileShader( fs ) );
      CheckCompileStatus( fs );

      // create program
      GLCHECK( glAttachShader( this->Get(), vs ) ); // attach vertex shader
      GLCHECK( glAttachShader( this->Get(), fs ) ); // attach fragment shader
      GLCHECK( glLinkProgram( this->Get() ) ); // assemble program
      CheckLinkStatus( this->Get() );

      // delete shaders, because program contains them already
      GLCHECK( glDetachShader( this->Get(), vs ) );
      GLCHECK( glDetachShader( this->Get(), fs ) );
      GLCHECK( glDeleteShader( vs ) );
      GLCHECK( glDeleteShader( fs ) );
   }

   void Shader::Activate() const
   {
      GLCHECK( glUseProgram( this->Get() ) );
   }

   auto Shader::Get() const noexcept -> Handle
   {
      return m_Program.Get();
   }
}
