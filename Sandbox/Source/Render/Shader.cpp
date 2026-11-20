#include "Shader.hpp"

#include "Common.hpp"
#include "Utils/Assert.hpp"
#include "Utils/File.hpp"

#include <string>
#include <print>
#include <format>

namespace
{
   constexpr const char *k_sVertexShaderFileResolution = ".vert";
   constexpr const char *k_sFragmentShaderFileResolution = ".frag";
   constexpr const char *k_sShaderSubDir = "Shaders/";

   [[nodiscard]] auto CheckLinkStatus( const GLuint a_iID ) -> bool
   {
      GLint bHasLinked = 1;
      auto sInfoMessage = std::make_unique<char[]>( GL_INFO_LOG_LENGTH );

      GLCHECK( glGetProgramiv( a_iID, GL_LINK_STATUS, &bHasLinked ) );
      if( bHasLinked == GL_FALSE )
      {
         GLCHECK( glGetProgramInfoLog( a_iID, GL_INFO_LOG_LENGTH, nullptr, sInfoMessage.get() ) );
         std::println( stderr, "{}", sInfoMessage.get() );
         DEBUGBREAK();
      }
      return bHasLinked == GL_TRUE;
   }

   [[nodiscard]] auto CheckCompileStatus( const GLuint a_iID ) -> bool
   {
      GLint bHasCompiled = 1;
      auto sInfoMessage = std::make_unique<char[]>( GL_INFO_LOG_LENGTH );

      GLCHECK( glGetShaderiv( a_iID, GL_COMPILE_STATUS, &bHasCompiled ) );
      if( bHasCompiled == GL_FALSE )
      {
         GLCHECK( glGetShaderInfoLog( a_iID, GL_INFO_LOG_LENGTH, nullptr, sInfoMessage.get() ) );
         std::println( stderr, "{}", sInfoMessage.get() );
         DEBUGBREAK();
      }
      return bHasCompiled == GL_TRUE;
   }

   [[nodiscard]] auto MakeShader( const GLenum a_iShaderType, const std::string_view a_sSourceCode ) -> MK::Render::ShaderHandle
   {
      MK::Render::ShaderHandle shader{ glCreateShader( a_iShaderType ) };

      const char *sSource = a_sSourceCode.data();
      const GLint iSourceLength = static_cast<GLint>( a_sSourceCode.size() );
      GLCHECK( glShaderSource( shader.Get(), 1, &sSource, &iSourceLength ) );
      GLCHECK( glCompileShader( shader.Get() ) );

      if( !CheckCompileStatus( shader.Get() ) )
         return {};

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
         throw std::runtime_error( std::format( "{}: Failed to create vertex shader", a_sShaderName ) );
      }
      
      // fragment shader
      const auto fs = MakeShader( GL_FRAGMENT_SHADER, sFragmentShaderContent );
      if( !fs.Get() )
      {
         throw std::runtime_error( std::format( "{}: Failed to create fragment shader", a_sShaderName ) );
      }

      // create program
      GLCHECK( glAttachShader( this->Get(), vs.Get() ) ); // attach vertex shader
      GLCHECK( glAttachShader( this->Get(), fs.Get() ) ); // attach fragment shader
      GLCHECK( glLinkProgram( this->Get() ) ); // assemble program

      if( !CheckLinkStatus( this->Get() ) )
      {
         throw std::runtime_error( std::format( "{}: Failed to link shader program", a_sShaderName ) );
      }

      GLCHECK( glDetachShader( this->Get(), vs.Get() ) );
      GLCHECK( glDetachShader( this->Get(), fs.Get() ) );

      this->CacheUniforms();
   }

   void Shader::Activate() const
   {
      GLCHECK( glUseProgram( this->Get() ) );
   }

   void Shader::SetUniform( const std::string_view a_sUniformName, const glm::mat3 &a_m3Value ) const noexcept
   {
      if( const auto iLocation = this->GetUniformLocation( a_sUniformName, GL_FLOAT_MAT3 ); iLocation != -1 )
         GLCHECK( glUniformMatrix3fv( iLocation, 1, GL_FALSE, glm::value_ptr( a_m3Value ) ) );
   }

   void Shader::SetUniform( const std::string_view a_sUniformName, const glm::mat4 &a_m4Value ) const noexcept
   {
      if( const auto iLocation = this->GetUniformLocation( a_sUniformName, GL_FLOAT_MAT4 ); iLocation != -1 )
         GLCHECK( glUniformMatrix4fv( iLocation, 1, GL_FALSE, glm::value_ptr( a_m4Value ) ) );
   }

   void Shader::SetUniform( const std::string_view a_sUniformName, const glm::vec2 &a_v2Value ) const noexcept
   {
      if( const auto iLocation = this->GetUniformLocation( a_sUniformName, GL_FLOAT_VEC2 ); iLocation != -1 )
         GLCHECK( glUniform2fv( iLocation, 1, glm::value_ptr( a_v2Value ) ) );
   }

   void Shader::SetUniform( const std::string_view a_sUniformName, const glm::vec3 &a_v3Value ) const noexcept
   {
      if( const auto iLocation = this->GetUniformLocation( a_sUniformName, GL_FLOAT_VEC3 ); iLocation != -1 )
         GLCHECK( glUniform3fv( iLocation, 1, glm::value_ptr( a_v3Value ) ) );
   }

   void Shader::SetUniform( const std::string_view a_sUniformName, const glm::vec4 &a_v4Value ) const noexcept
   {
      if( const auto iLocation = this->GetUniformLocation( a_sUniformName, GL_FLOAT_VEC4 ); iLocation != -1 )
         GLCHECK( glUniform4fv( iLocation, 1, glm::value_ptr( a_v4Value ) ) );
   }

   void Shader::SetUniform( const std::string_view a_sUniformName, const float a_fValue ) const noexcept
   {
      if( const auto iLocation = this->GetUniformLocation( a_sUniformName, GL_FLOAT ); iLocation != -1 )
         GLCHECK( glUniform1f( iLocation, a_fValue ) );
   }

   void Shader::SetUniform( const std::string_view a_sUniformName, const std::int32_t a_iValue, const GLenum a_eTypeSpecifier ) const noexcept
   {
      if( const auto iLocation = this->GetUniformLocation( a_sUniformName, a_eTypeSpecifier ); iLocation != -1 )
         GLCHECK( glUniform1i( iLocation, a_iValue ) );
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

   auto Shader::GetUniformLocation( const std::string_view a_sUniformName, const GLenum a_eType ) const noexcept -> GLint
   {
      const auto it = m_mUniformCache.find( a_sUniformName );
      if( it == m_mUniformCache.cend() )
         return -1;
      MKASSERT( it->second.eType == a_eType, "Uniform {} was searched with a wrong type", a_sUniformName );
      return it->second.iLocation;
   }
}
