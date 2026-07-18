#pragma once

#include <expected>
#include <string>
#include <string_view>

namespace MK::Utils::File
{
   enum class EErrorCode : std::int32_t
   {
      UNINITIALIZED,
      NOT_FOUND,
      ACCESS_DENIED,
      IO_ERROR,
      FAILED_TO_OPEN,
   };

   void SetDefaultPath( const std::string_view a_sPath );

   [[nodiscard]]
   std::expected<std::string, EErrorCode> GetFileContent( const std::string_view a_sFilePath );
}
