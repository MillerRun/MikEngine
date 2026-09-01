#include "Eventable.hpp"
#include "EventBus.hpp"

namespace MK::Event
{
   IEventable::~IEventable()
   {
      // remove all listeners
      m_aSubscriptionTokens.erase( std::remove_if( m_aSubscriptionTokens.begin(), m_aSubscriptionTokens.end(), []( const Token &a_Token )
      {
         EventBus::Get().RemoveListener( a_Token.EventID, a_Token.iCallbackIndex );
         return true;
      } ), m_aSubscriptionTokens.end() );
   }

   void IEventable::PassCallback( const std::type_index a_EventID, std::function<void( const void * )> a_WrappedCallback )
   {
      Token token = EventBus::Get().AddListener( a_EventID, a_WrappedCallback );
      m_aSubscriptionTokens.push_back( std::move( token ) );
   }

   void IEventable::ClearCallbacks( const std::type_index a_EventID )
   {
      m_aSubscriptionTokens.erase( std::remove_if( m_aSubscriptionTokens.begin(), m_aSubscriptionTokens.end(), [a_EventID]( const Token& a_Token )
      {
         if( a_Token.EventID != a_EventID )
            return false;

         // remove listener from the bus as well
         EventBus::Get().RemoveListener( a_Token.EventID, a_Token.iCallbackIndex );
         return true;
      } ), m_aSubscriptionTokens.end() );
   }

   void IEventable::CallCallbacks( const std::type_index a_EventID, const void *a_pArguments )
   {
      EventBus::Get().Dispatch( a_EventID, a_pArguments );
   }
}