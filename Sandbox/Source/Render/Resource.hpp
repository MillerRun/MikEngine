#pragma once

#include "Common.hpp"

namespace MK::Render
{
   template<auto TDeleter>
   class Resource
   {
   public:
      // deleted copying
      Resource( const Resource & ) = delete;
      void operator=( const Resource & ) = delete;

      Resource() noexcept = default;
      explicit Resource( const Handle a_Handle ) noexcept
         : m_Handle{ a_Handle }
      {
      }

      Resource( Resource &&a_Other ) noexcept
         : m_Handle{ std::exchange( a_Other.m_Handle, Handle{} ) }
      {
      }

      Resource &operator=( Resource &&a_Other ) noexcept
      {
         if( this != std::addressof( a_Other ) )
         {
            if( m_Handle )
            {
               TDeleter( m_Handle );
            }
            m_Handle = std::exchange( a_Other.m_Handle, Handle{} );
         }
         return *this;
      }

      ~Resource() noexcept
      {
         if( m_Handle )
         {
            TDeleter( m_Handle );
         }
      }

      [[nodiscard]] auto Get() const noexcept -> Handle
      {
         return m_Handle;
      }

   protected:
      Handle m_Handle{};
   };
}
