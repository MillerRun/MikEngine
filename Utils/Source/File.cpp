#include "pch.hpp"

#include "File.hpp"
#include "Assert.hpp"

namespace
{
   std::filesystem::path g_sDefaultPath;
}

namespace MK::File
{
   void SetDefaultPath( const std::string_view a_sPath )
   {
      MKASSERT( g_sDefaultPath.empty(), "Trying to set default file path to {} when it was already set to {}", a_sPath, g_sDefaultPath.string() );
      g_sDefaultPath = a_sPath;
   }

   std::expected<std::string, EErrorCode> GetFileContent( const std::string_view a_sFilePath )
   {
      if( g_sDefaultPath.empty() )
         return std::unexpected{ EErrorCode::UNINITIALIZED };

      const std::filesystem::path sPath{ g_sDefaultPath / a_sFilePath };
      {
         std::error_code errc;
         const auto status = std::filesystem::status( sPath, errc );
         switch( const auto eType = status.type() )
         {
         case std::filesystem::file_type::not_found:
         {
            return std::unexpected{ EErrorCode::NOT_FOUND };
         }
         case std::filesystem::file_type::regular:
         {
            const auto permissions = status.permissions();
            if( ( permissions & std::filesystem::perms::others_read ) == std::filesystem::perms::none )
               return std::unexpected{ EErrorCode::ACCESS_DENIED };
            break;
         }
         default:
            break;
         }
      }

      if( std::ifstream file{ sPath, std::ios::binary } )
      {
         std::string sContent;
         file.seekg( 0, std::ios::end );
         sContent.resize( file.tellg() );
         file.seekg( 0, std::ios::beg );
         file.read( sContent.data(), sContent.size() );
         return sContent;
      }
      return std::unexpected{ EErrorCode::FAILED_TO_OPEN };
   }
}
