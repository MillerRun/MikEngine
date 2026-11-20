#pragma once

#include "Resource.hpp"
#include "Utils/Math.hpp"
#include <string>
#include <string_view>
#include <unordered_map>

namespace MK::Render
{
   class Shader final
   {
      struct UniformInfo
      {
         GLint iLocation = -1;
         GLenum eType{};
         GLint iCount = 0;
      };

   public:
      explicit Shader( const std::string_view a_sShaderName );

      [[nodiscard]] auto Get() const noexcept -> Handle;
      void Activate() const;

      void SetUniform( const std::string_view a_sUniformName, const glm::mat3 &a_m3Value ) const noexcept;
      void SetUniform( const std::string_view a_sUniformName, const glm::mat4 &a_m4Value ) const noexcept;
      void SetUniform( const std::string_view a_sUniformName, const glm::vec2 &a_v2Value ) const noexcept;
      void SetUniform( const std::string_view a_sUniformName, const glm::vec3 &a_v3Value ) const noexcept;
      void SetUniform( const std::string_view a_sUniformName, const glm::vec4 &a_v4Value ) const noexcept;
      void SetUniform( const std::string_view a_sUniformName, const float a_fValue ) const noexcept;
      void SetUniform( const std::string_view a_sUniformName, const std::int32_t a_iValue, const GLenum a_eTypeSpecifier ) const noexcept;

   private:
      void CacheUniforms();
      [[nodiscard]] auto GetUniformLocation( const std::string_view a_sUniformName, const GLenum a_eType ) const noexcept -> GLint;

   private:
      struct StringHash
      {
         using is_transparent = void;
         std::size_t operator()( const std::string_view s ) const noexcept { return std::hash<std::string_view>{}( s ); }
      };
      std::unordered_map<std::string, UniformInfo, StringHash, std::equal_to<>> m_mUniformCache;
      ProgramHandle m_Program;
   };
}
