#include "pch.hpp"
#include "Text.hpp"

namespace Utils
{
   auto ToLower( const std::string_view a_sText ) -> std::string
   {
      std::string sRetValue{ a_sText };
      for( auto &ch : sRetValue )
         ch = static_cast<char>( std::tolower( static_cast<unsigned char>( ch ) ) );
      return sRetValue;
   }

   auto ToLower( const std::wstring_view a_sText ) -> std::wstring
   {
      std::wstring sRetValue{ a_sText };
      for( auto &ch : sRetValue )
         ch = static_cast<wchar_t>( std::tolower( static_cast<std::wint_t>( ch ) ) );
      return sRetValue;
   }
}