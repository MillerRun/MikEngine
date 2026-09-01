#pragma once

#include <vector>

#include "EventBus.hpp"

namespace MK::Event
{
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
         this->PassCallbackToBus( id, wrappedCallback );
      }

      template<typename TEventType>
      void Unsubsribe()
      {
         const std::type_index id = typeid( TEventType );
         this->ClearCallbacksFromBus( id );
      }

   private:
      void PassCallbackToBus( const std::type_index a_EventID, std::function<void( const void * )> a_WrappedCallback );
      void ClearCallbacksFromBus( const std::type_index a_EventID );

   private:
      std::vector<Token> m_aSubscriptionTokens;
   };
}
