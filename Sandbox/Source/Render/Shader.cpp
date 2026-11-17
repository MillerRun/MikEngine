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

   [[nodiscard]] auto MakeShader( const int a_iShaderType, const std::string_view a_sSourceCode ) -> MK::Render::ShaderHandle
   {
      MK::Render::ShaderHandle shader{ glCreateShader( a_iShaderType ) };

      const char *sSource = a_sSourceCode.data();
      GLCHECK( glShaderSource( shader.Get(), 1, &sSource, nullptr));
      GLCHECK( glCompileShader( shader.Get() ) );

      CheckCompileStatus( shader.Get() );

      return shader;
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
      const auto vs = MakeShader( GL_VERTEX_SHADER, sVertexShaderContent );
      if( !vs.Get() )
      {
         throw std::runtime_error( "Failed to create vertex shader" );
      }
      
      // fragment shader
      const auto fs = MakeShader( GL_FRAGMENT_SHADER, sFragmentShaderContent );
      if( !fs.Get() )
      {
         throw std::runtime_error( "Failed to create fragment shader" );
      }

      // create program
      GLCHECK( glAttachShader( this->Get(), vs.Get() ) ); // attach vertex shader
      GLCHECK( glAttachShader( this->Get(), fs.Get() ) ); // attach fragment shader
      GLCHECK( glLinkProgram( this->Get() ) ); // assemble program
      CheckLinkStatus( this->Get() );

      // delete shaders, because program contains them already
      GLCHECK( glDetachShader( this->Get(), vs.Get() ) );
      GLCHECK( glDetachShader( this->Get(), fs.Get() ) );

      this->CacheUniforms();
   }

   void Shader::Activate() const
   {
      GLCHECK( glUseProgram( this->Get() ) );
   }

   auto Shader::Get() const noexcept -> Handle
   {
      return m_Program.Get();
   }

   void Shader::CacheUniforms()
   {
      GLint iAmount{}, iMaxNameLength{};
      GLCHECK( glGetProgramiv( this->Get(), GL_ACTIVE_UNIFORMS, &iAmount ) );
      GLCHECK( glGetProgramiv( this->Get(), GL_ACTIVE_UNIFORM_MAX_LENGTH, &iMaxNameLength ) );

      std::string sBuffer( iMaxNameLength, '\0' );
      for( GLuint i = 0; i < static_cast<GLuint>( iAmount ); ++i )
      {
         GLsizei iNameLength{};
         GLint iSize{};
         GLenum eType{};

         GLCHECK( glGetActiveUniform( this->Get(), i, iMaxNameLength, &iNameLength, &iSize, &eType, sBuffer.data() ) );

         const GLint iLocation = glGetUniformLocation( this->Get(), sBuffer.c_str() );
         if( iLocation == -1 )
            continue;

         std::string sName( sBuffer.data(), iNameLength );
         if( sName.ends_with( "[0]" ) ) // arrays have postfixes
            sName.resize( sName.size() - 3 );

         m_mUniformCache.emplace( std::move( sName ), UniformInfo{ .iLocation = iLocation, .eType = eType, .iCount = iSize } );
      }
   }
}
