#include "Resource.hpp"

namespace MK::Render
{
   namespace Helper
   {
      void DeleteProgram( const Handle a_Handle ) noexcept
      {
         glDeleteProgram( a_Handle );
      }

      void DeleteShader( const Handle a_Handle ) noexcept
      {
         glDeleteShader( a_Handle );
      }
   }
}
