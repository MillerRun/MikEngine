#pragma once

#include <Utils/Assert.hpp>

// GLCHECK - checks for gl errors
//
#ifdef _DEBUG
#  include <glad/glad.h>
#  define GLCHECK( call )                                                                               \
   do                                                                                                   \
   {                                                                                                    \
      while( glGetError() != GL_NO_ERROR );                                                             \
      call;                                                                                             \
      const GLenum eError = glGetError();                                                               \
      if( eError != GL_NO_ERROR )                                                                       \
      {                                                                                                 \
         std::string sErrorText;                                                                        \
         switch( eError )                                                                               \
         {                                                                                              \
         case GL_INVALID_ENUM: sErrorText = "GL_INVALID_ENUM"; break;                                   \
         case GL_INVALID_VALUE: sErrorText = "GL_INVALID_VALUE"; break;                                 \
         case GL_INVALID_OPERATION: sErrorText = "GL_INVALID_OPERATION"; break;                         \
         case GL_OUT_OF_MEMORY: sErrorText = "GL_OUT_OF_MEMORY"; break;                                 \
         case GL_INVALID_FRAMEBUFFER_OPERATION: sErrorText = "GL_INVALID_FRAMEBUFFER_OPERATION"; break; \
         default: sErrorText = std::format( "0x{:o}", eError );                                         \
         }                                                                                              \
         MKASSERT( false, "0x{:o} - {} at {}", eError, sErrorText, #call );                             \
      }                                                                                                 \
   } while( false )

#else // not _DEBUG
#  define GLCHECK( call ) call
#endif // GLCHECK
