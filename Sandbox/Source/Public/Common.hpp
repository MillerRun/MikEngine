#pragma once

#include <format>
#include <string>
#include <string_view>
#include <fstream>

// GLCHECK - checks for gl errors
//
#ifdef _DEBUG
#  include <glad/glad.h>
#  include <print>
#  define GLCHECK( call )                                                                                                 \
   do                                                                                                                     \
   {                                                                                                                      \
      while( glGetError() != GL_NO_ERROR );                                                                               \
      call;                                                                                                               \
      const GLenum eError = glGetError();                                                                                 \
      if( eError != GL_NO_ERROR )                                                                                         \
      {                                                                                                                   \
         std::string sErrorText;                                                                                          \
         switch( eError )                                                                                                 \
         {                                                                                                                \
         case GL_INVALID_ENUM: sErrorText = "GL_INVALID_ENUM"; break;                                                     \
         case GL_INVALID_VALUE: sErrorText = "GL_INVALID_VALUE"; break;                                                   \
         case GL_INVALID_OPERATION: sErrorText = "GL_INVALID_OPERATION"; break;                                           \
         case GL_OUT_OF_MEMORY: sErrorText = "GL_OUT_OF_MEMORY"; break;                                                   \
         case GL_INVALID_FRAMEBUFFER_OPERATION: sErrorText = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;                   \
         default: sErrorText = std::format( "0x{:o}", eError );                                                           \
         }                                                                                                                \
         std::println( stderr, "[ERROR OpenGL] {}:{}: {}(0x{:o}) at {}", __FILE__, __LINE__, sErrorText, eError, #call ); \
         __debugbreak();                                                                                                  \
      }                                                                                                                   \
   } while( false )

#else // not _DEBUG
#  define GLCHECK( call ) call
#endif // GLCHECK

// ASSERT
//
#ifdef _DEBUG

   // DEBUGBREAK
   //
#  ifdef _MSC_VER
#    define DEBUGBREAK() __debugbreak()
#  else
#    include <csignal>
#    define DEBUGBREAK() raise( SIGTRAP )
#  endif // // DEBUGBREAK

   // MKASSERT
   //
#  include <print>
#  include <format>
#  define MKASSERT( expression, ... )                                                                  \
   do                                                                                                  \
   {                                                                                                   \
      if( not( expression ) )                                                                          \
      {                                                                                                \
         std::println( stderr, "[ASSERT] {}:{}: {}", __FILE__, __LINE__, std::format( __VA_ARGS__ ) ); \
         DEBUGBREAK();                                                                                 \
      }                                                                                                \
   }while( false )

#else
#  define DEBUGBREAK()
#  define MKASSERT( expression, ... )
#endif // ASSERT

class Utils
{
public:
   static constexpr std::string_view k_sResoursesDir = "../Resources/";
   static constexpr std::string_view k_sShadersDir = "../Resources/Shaders/";

public:
   [[nodiscard]] static
   std::string GetFileContent( const std::string_view a_sFilePath )
   {
      if( std::ifstream file{ a_sFilePath.data(), std::ios::binary } )
      {
         std::string sContent;
         file.seekg( 0, std::ios::end );
         sContent.resize( file.tellg() );
         file.seekg( 0, std::ios::beg );
         file.read( sContent.data(), sContent.size() );
         return sContent;
      }
      const std::string sErrorMessage = "Failed to open file " + std::string{ a_sFilePath };
      throw std::runtime_error( sErrorMessage );
   }
};
