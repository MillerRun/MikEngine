#include "EventListener.hpp"
#include "EventBus.hpp"

namespace MK::Event
{
   EventListener::~EventListener()
   {
      // remove all listeners
      m_aSubscriptionTokens.erase( std::remove_if( m_aSubscriptionTokens.begin(), m_aSubscriptionTokens.end(), []( const Token &a_Token )
      {
         EventBus::Get().RemoveListener( a_Token.EventID, a_Token.iCallbackIndex );
         return true;
      } ), m_aSubscriptionTokens.end() );
   }

   void EventListener::PassCallbackToBus( const std::type_index a_EventID, std::function<void( const void * )> a_WrappedCallback )
   {
      Token token = EventBus::Get().AddListener( a_EventID, a_WrappedCallback );
      m_aSubscriptionTokens.push_back( std::move( token ) );
   }

   void EventListener::ClearCallbacksFromBus( const std::type_index a_EventID )
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
}