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
      FAILED_TO_READ,

      MAX
   };
   
   template<typename T>
   concept CPathPart = std::convertible_to<T, std::string_view>;

   /// <summary> Records a default file directory </summary>
   /// <param name="a_sPath"> will be automaticaly inserted in a beggining of each following function call </param>
   /// <returns> void </returns>
   ///
   void SetDefaultPath( const std::string_view a_sPath );

   /// <summary> Try read a file with a given path </summary>
   /// <param name="a_sFilePath"> relative file path </param>
   /// <returns> std::expected with either std::string of file's content or EErrorCode value </returns>
   ///
   [[nodiscard]]
   auto GetFileContent( const std::string_view a_sFilePath ) -> std::expected<std::string, EErrorCode>;

   /// <summary> Try read a file with a given path. All arguments will be merged in a single string with '/' separators </summary>
   /// <param name="a_Params"> path elements of text type </param>
   /// <returns> std::expected with either std::string of file's content or EErrorCode value </returns>
   ///
   template<CPathPart... TPathParams> requires( sizeof...( TPathParams ) >= 2 )
   [[nodiscard]]
   auto GetFileContent( TPathParams&&... a_Params ) -> std::expected<std::string, EErrorCode>
   {
      std::string sResult; ( ( sResult += a_Params, sResult += '/' ), ... );
      sResult.pop_back(); // remove separator on end
      return GetFileContent( sResult );
   }

   [[nodiscard]]
   auto GetFileDirectory( const std::string_view a_sFilePath ) -> std::string;

   [[nodiscard]]
   auto GetFileExtension( const std::string_view a_sFilePath ) -> std::string;
}

template<>
struct std::formatter<MK::File::EErrorCode> : std::formatter<std::string_view>
{
private:
   [[nodiscard]] static constexpr
   auto ToString( const MK::File::EErrorCode eCode ) -> std::string_view
   {
      using namespace MK::File;

      static_assert( static_cast<int>( EErrorCode::MAX ) == 6, "Unhandled enum case. Add case statement below and update static_assert" );
      switch ( eCode )
      {
      case EErrorCode::UNINITIALIZED:  return "UNINITIALIZED";
      case EErrorCode::NOT_FOUND:      return "NOT_FOUND";
      case EErrorCode::ACCESS_DENIED:  return "ACCESS_DENIED";
      case EErrorCode::IO_ERROR:       return "IO_ERROR";
      case EErrorCode::FAILED_TO_OPEN: return "FAILED_TO_OPEN";
      case EErrorCode::FAILED_TO_READ: return "FAILED_TO_READ";
      case EErrorCode::MAX:
         return "";
      }
      std::unreachable();
   }

public:
   auto format( const MK::File::EErrorCode eCode, auto &ctx ) const
   {
      return std::formatter<std::string_view>::format( ToString( eCode ), ctx );
   }
};