#pragma once

#include <Utils/Assert.hpp>

// GLCHECK - checks for gl errors
//
#ifdef NDEBUG
#  define GLCHECK( call ) call
#else
#  include <glad/glad.h>
#  define GLCHECK( call )                                                                               \
   do                                                                                                   \
   {                                                                                                    \
      while( glGetError() != GL_NO_ERROR ); /* clean previous errors first */                           \
      call;                                                                                             \
      if( const GLenum eError = glGetError(); eError != GL_NO_ERROR )                                   \
      {                                                                                                 \
         std::string sErrorText;                                                                        \
         switch( eError )                                                                               \
         {                                                                                              \
         case GL_INVALID_ENUM: sErrorText = "GL_INVALID_ENUM"; break;                                   \
         case GL_INVALID_VALUE: sErrorText = "GL_INVALID_VALUE"; break;                                 \
         case GL_INVALID_OPERATION: sErrorText = "GL_INVALID_OPERATION"; break;                         \
         case GL_OUT_OF_MEMORY: sErrorText = "GL_OUT_OF_MEMORY"; break;                                 \
         case GL_INVALID_FRAMEBUFFER_OPERATION: sErrorText = "GL_INVALID_FRAMEBUFFER_OPERATION"; break; \
         default: sErrorText = "unhandled case";                                                        \
         }                                                                                              \
         MKASSERT( false, "0x{:o} - {} at {}", eError, sErrorText, #call );                             \
      }                                                                                                 \
   } while( false )
#endif // GLCHECK
