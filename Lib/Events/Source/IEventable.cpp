#include "IEventable.hpp"
#include "EventBus.hpp"

namespace MK::Event
{
   // Each constructor touches the bus, so it is fully constructed before any IEventable.
   // Hence the bus is destroyed after all of them, including static ones.
   IEventable::IEventable()
   {
      std::ignore = EventBus::Get();
   }

   // Callables capture the original 'this', so subscriptions are never copied or moved
   IEventable::IEventable( const IEventable & )
   {
      std::ignore = EventBus::Get();
   }

   IEventable &IEventable::operator=( const IEventable & )
   {
      return *this;
   }

   IEventable::IEventable( IEventable && ) noexcept
   {
      std::ignore = EventBus::Get();
   }

   IEventable &IEventable::operator=( IEventable && ) noexcept
   {
      return *this;
   }

   IEventable::~IEventable()
   {
      for( const Token &token : m_aSubscriptionTokens )
         EventBus::Get().RemoveListener( token );
   }

   void IEventable::Unsubscribe( const SubscriptionID a_ID )
   {
      auto itToken = std::find_if( m_aSubscriptionTokens.begin(), m_aSubscriptionTokens.end(), [a_ID]( const Token &a_Token )
      {
         return a_Token.iCallbackIndex == a_ID;
      } );
      if( itToken == m_aSubscriptionTokens.end() )
         return;

      const Token token = *itToken;
      m_aSubscriptionTokens.erase( itToken );
      EventBus::Get().RemoveListener( token );
   }

   SubscriptionID IEventable::PassCallback( const std::type_index a_EventID, std::function<void( const void * )> a_WrappedCallback )
   {
      const Token token = EventBus::Get().AddListener( a_EventID, std::move( a_WrappedCallback ) );
      m_aSubscriptionTokens.push_back( token );
      return token.iCallbackIndex;
   }

   void IEventable::ClearCallbacks( const std::type_index a_EventID )
   {
      // tokens are detached first, so removal can't observe a half-updated vector
      std::vector<Token> aRemovedTokens;
      std::erase_if( m_aSubscriptionTokens, [a_EventID, &aRemovedTokens]( const Token &a_Token )
      {
         if( a_Token.EventID != a_EventID )
            return false;

         aRemovedTokens.push_back( a_Token );
         return true;
      } );

      for( const Token &token : aRemovedTokens )
         EventBus::Get().RemoveListener( token );
   }

   void IEventable::CallCallbacks( const std::type_index a_EventID, const void *a_pArguments )
   {
      EventBus::Get().Dispatch( a_EventID, a_pArguments );
   }
}
