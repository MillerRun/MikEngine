#pragma once

#include "Resource.hpp"
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
      void Activate() const;
      [[nodiscard]] auto Get() const noexcept -> Handle;

   private:
      void CacheUniforms();

   private:
      ProgramHandle m_Program;
      std::unordered_map<std::string, UniformInfo> m_mUniformCache;
   };
}
