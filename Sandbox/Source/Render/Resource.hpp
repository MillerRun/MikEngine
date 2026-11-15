#pragma once

#include "Common.hpp"

namespace MK::Render
{
   template<auto Deleter>
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

      Resource &operator=( Resource &&a_Other ) noexcept( noexcept( Deleter ) )
      {
         if( this != std::addressof( a_Other ) )
         {
            if( m_Handle )
            {
               Deleter( m_Handle );
            }
            m_Handle = std::exchange( a_Other.m_Handle, Handle{} );
         }
         return *this;
      }

      ~Resource() noexcept noexcept( noexcept( Deleter ) )
      {
         if( m_Handle )
         {
            Deleter( m_Handle );
         }
      }

      [[nodiscard]] auto Get() const noexcept -> Handle
      {
         return m_Handle;
      }

   private:
      Handle m_Handle{};
   };
}
