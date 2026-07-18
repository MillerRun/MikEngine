#pragma once

#include <expected>
#include <format>
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

      MAX
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

template<>
struct std::formatter<MK::File::EErrorCode> : std::formatter<std::string_view>
{
   [[nodiscard]] static constexpr
   std::string_view ToString( const MK::File::EErrorCode eCode )
   {
      using namespace MK::File;

      static_assert( static_cast<int>( EErrorCode::MAX ) == 5, "Unhandled enum case. Add statement below and update static_assert" );
      switch ( eCode )
      {
      case EErrorCode::UNINITIALIZED:  return "UNINITIALIZED";
      case EErrorCode::NOT_FOUND:      return "NOT_FOUND";
      case EErrorCode::ACCESS_DENIED:  return "ACCESS_DENIED";
      case EErrorCode::IO_ERROR:       return "IO_ERROR";
      case EErrorCode::FAILED_TO_OPEN: return "FAILED_TO_OPEN";
      }
      return "";
   }

   auto format( const MK::File::EErrorCode eCode, auto &ctx ) const
   {
      return std::formatter<std::string_view>::format( ToString( eCode ), ctx );
   }
};