#pragma once

#include "Resource.hpp"
#include <string_view>

namespace MK::Render
{
   class Shader final
   {
   public:
      explicit Shader( const std::string_view a_sShaderName );
      void Activate() const;
      [[nodiscard]] auto Get() const noexcept -> Handle;

   private:
      ProgramHandle m_Program;
   };
}
