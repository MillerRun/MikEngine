#include "pch.hpp"

#include "File.hpp"
#include "Assert.hpp"
#include "Text.hpp"

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

   auto GetFileContent( const std::string_view a_sFilePath ) -> std::expected<std::string, EErrorCode>
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

   auto GetBinaryFileContent( const std::string_view a_sFilePath ) -> std::expected<std::vector<std::byte>, EErrorCode>
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

      if( std::ifstream file{ sPath, std::ios::binary | std::ios::ate } )
      {
         std::vector<std::byte> aContent;

         const std::streamsize iSize = file.tellg();
         if( iSize <= 0 )
            return aContent;

         aContent.resize( iSize );
         file.seekg( 0, std::ios::beg );
         if( not file.read( reinterpret_cast<char *>( aContent.data() ), aContent.size() ) )
            return std::unexpected{ EErrorCode::FAILED_TO_READ };
         return aContent;
      }
      return std::unexpected{ EErrorCode::FAILED_TO_OPEN };
   }

   auto GetFileLocation( const std::string_view a_sFilePath ) -> std::string
   {
      if( g_sDefaultPath.empty() )
         return {};
      
      const std::filesystem::path sPath{ g_sDefaultPath / a_sFilePath };
      return sPath.lexically_normal().string();
   }

   auto GetFileDirectory( const std::string_view a_sFilePath ) -> std::string
   {
      if( GetFileExtension( a_sFilePath ).empty() )
      {
         MKASSERT( false, "Wierd parameter: {}", a_sFilePath );
         return a_sFilePath.data();
      }
      const std::filesystem::path sPath{ g_sDefaultPath / a_sFilePath };
      const auto sParentPath = sPath.parent_path();
      return sParentPath.empty() ? "." : sParentPath.string();
   }

   auto GetFileExtension( const std::string_view a_sFilePath ) -> std::string
   {
      const std::filesystem::path sPath{ g_sDefaultPath / a_sFilePath };
      std::string sExtestion = sPath.extension().string();
      sExtestion = sExtestion.substr( 1uz ); // remove dot on the beginning
      sExtestion = Utils::ToLower( sExtestion );
      return sExtestion;
   }

   auto GetPath( const std::filesystem::path &a_Path ) -> std::filesystem::path
   {
      return std::filesystem::path{ g_sDefaultPath / a_Path };
   }
}
