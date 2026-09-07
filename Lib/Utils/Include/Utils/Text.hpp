#pragma once

#include <string_view>
#include <string>

namespace Utils
{
   [[nodiscard]]
   auto ToLower( const std::string_view a_sText ) -> std::string;

   [[nodiscard]]
   auto ToLower( const std::wstring_view a_sText ) -> std::wstring;
}
