#include "EventBus.hpp"

namespace MK::Event
{
   Token EventBus::AddListener( const std::type_index a_EventID, std::function<void( const void * )> a_Listener )
   {
      const std::size_t index = m_iAvailableIndex++;
      CallbackGroup &group = m_mListenerGroups[a_EventID];
      group[index] = std::move( a_Listener );
      return Token {
         .EventID = a_EventID,
         .iCallbackIndex = index
      };
   }

   void EventBus::RemoveListener( const std::type_index a_EventID, const std::size_t a_iListenerIndex )
   {
      auto itGroup = m_mListenerGroups.find( a_EventID );
      if( itGroup == m_mListenerGroups.end() )
         return;

      CallbackGroup &callbacks = itGroup->second;
      callbacks.erase( a_iListenerIndex );
   }

   void EventBus::Dispatch( const std::type_index a_EventID, const void *a_pArguments )
   {
      auto itGroup = m_mListenerGroups.find( a_EventID );
      if( itGroup == m_mListenerGroups.end() )
         return;

      for( auto &[_, Callback] : itGroup->second )
         Callback( a_pArguments );
   }
}