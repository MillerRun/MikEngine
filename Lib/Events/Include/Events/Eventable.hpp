#pragma once

#include <vector>
#include <functional>
#include <typeindex>

namespace MK::Event
{
   struct Token;

   class IEventable
   {
   public:
      virtual ~IEventable();
      IEventable( IEventable & )                      = default;
      IEventable &operator=( IEventable & )           = default;
      IEventable( IEventable && ) noexcept            = default;
      IEventable &operator=( IEventable && ) noexcept = default;

   public:
      
      template<typename TEventType>
      void Subscribe( std::function<void( const TEventType & )> a_Listener )
      {
         const std::type_index id = typeid( TEventType );
         auto wrappedCallback = [callback = std::move( a_Listener )]( const void *a_pData )
         {
            callback( *static_cast<const TEventType *>( a_pData ) );
         };
         this->PassCallback( id, wrappedCallback );
      }

      template<typename TEventType>
      void Unsubsribe()
      {
         const std::type_index id = typeid( TEventType );
         this->ClearCallbacks( id );
      }

      template<typename TEventType>
      void Call( const TEventType &a_Arguments )
      {
         const std::type_index id = typeid( TEventType );
         this->CallCallbacks( id, static_cast<const void *>( a_Arguments ) );
      }

   private:
      void PassCallback( const std::type_index a_EventID, std::function<void( const void * )> a_WrappedCallback );
      void ClearCallbacks( const std::type_index a_EventID );
      void CallCallbacks( const std::type_index a_EventID, const void *a_pArguments );

   private:
      std::vector<Token> m_aSubscriptionTokens;
   };
}
