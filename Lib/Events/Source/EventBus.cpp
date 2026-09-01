#include "EventBus.hpp"

namespace MK::Event
{
   Token EventBus::AddListener( const std::type_index a_EventID, CallbackWrapper a_Listener )
   {
      m_aInsertionQueue.emplace_back( InsertionRequest{
         .EventID = a_EventID,
         .iCallbackIndex = m_iAvailableIndex++,
         .Callback = std::move( a_Listener )
      } );

      return Token {
         .EventID = m_aInsertionQueue.back().EventID,
         .iCallbackIndex = m_aInsertionQueue.back().iCallbackIndex
      };
   }

   void EventBus::RemoveListener( const std::type_index a_EventID, const std::size_t a_iListenerIndex )
   {
      if( not m_mListenerGroups.contains( a_EventID ) )
         return;
      
      if( not m_mListenerGroups[a_EventID].contains( a_iListenerIndex ) )
         return;

      m_aRemovalQueue.emplace_back( RemovalRequest{
         .EventID = a_EventID,
         .iCallbackIndex = a_iListenerIndex
      } );
   }

   void EventBus::Dispatch( const std::type_index a_EventID, const void *a_pArguments )
   {
      this->CheckRemovalQueue();
      this->CheckInsertionQueue();

      auto itGroup = m_mListenerGroups.find( a_EventID );
      if( itGroup == m_mListenerGroups.end() )
         return;

      for( auto &[_, Callback] : itGroup->second )
         Callback( a_pArguments );
   }

   void EventBus::CheckRemovalQueue()
   {
      for( const auto &req : m_aRemovalQueue )
      {
         m_mListenerGroups[req.EventID].erase( req.iCallbackIndex );
      }
      m_aRemovalQueue.clear();
   }

   void EventBus::CheckInsertionQueue()
   {
      for( auto &req : m_aInsertionQueue )
      {
         m_mListenerGroups[req.EventID].emplace( req.iCallbackIndex, std::move( req.Callback ) );
      }
      m_aInsertionQueue.clear();
   }
}