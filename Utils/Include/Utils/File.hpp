#pragma once

#include <expected>
#include <string>
#include <string_view>

namespace MK::File
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

   template<typename T>
   concept CPathPart = std::convertible_to<T, std::string_view>;

   [[nodiscard]]
   std::expected<std::string, EErrorCode> GetFileContent( const std::string_view a_sFilePath );

   template<CPathPart... TPathParams> requires( sizeof...( TPathParams ) >= 2 )
   [[nodiscard]]
   std::expected<std::string, EErrorCode> GetFileContent( TPathParams&&... a_Params )
   {
      std::string sResult; ( ( sResult += a_Params, sResult += '/' ), ... );
      sResult.pop_back();
      return GetFileContent( sResult );
   }
}
