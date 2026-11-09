#pragma once

#ifdef NDEBUG
#  define DEBUGBREAK() ( void( 0 ) )
#else
#  ifdef _MSC_VER
#    define DEBUGBREAK() __debugbreak()
#  else
#    include <csignal>
#    define DEBUGBREAK() raise( SIGTRAP )
#  endif
#endif

#ifdef NDEBUG
#  define MKASSERT( expression, ... ) ( void( 0 ) )
#else
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
#endif // ASSERT
