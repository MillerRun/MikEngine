#pragma once

#ifdef NDEBUG
#  define DEBUGBREAK()
#else
#  ifdef _MSC_VER
#    define DEBUGBREAK() __debugbreak()
#  else
#    include <csignal>
#    define DEBUGBREAK() raise( SIGTRAP )
#  endif
#endif

#ifdef NDEBUG
#  define MKASSERT( expression, ... )
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
