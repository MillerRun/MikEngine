#include "EventBus.hpp"
#include <algorithm>

namespace MK::Event
{
   Token EventBus::AddListener( const std::type_index a_EventID, CallbackWrapper a_Listener )
   {
      const Token token{
         .EventID = a_EventID,
         .iCallbackIndex = m_iAvailableIndex++
      };

      Listener listener{
         .iCallbackIndex = token.iCallbackIndex,
         .Callback = std::move( a_Listener )
      };

      if( m_iDispatchDepth > 0 )
         m_aPendingInsertions.emplace_back( a_EventID, std::move( listener ) );
      else
         InsertSorted( m_mListenerGroups[a_EventID], std::move( listener ) );

      return token;
   }

   void EventBus::RemoveListener( const Token &a_Token )
   {
      // keeps the callback alive until the containers are consistent again
      CallbackWrapper removedCallback;

      // listener may not be inserted yet
      auto itPending = std::find_if( m_aPendingInsertions.begin(), m_aPendingInsertions.end(), [&a_Token]( const auto &a_Request )
      {
         return a_Request.second.iCallbackIndex == a_Token.iCallbackIndex;
      } );
      if( itPending != m_aPendingInsertions.end() )
      {
         removedCallback = std::move( itPending->second.Callback );
         m_aPendingInsertions.erase( itPending );
         return;
      }

      auto itGroup = m_mListenerGroups.find( a_Token.EventID );
      if( itGroup == m_mListenerGroups.end() )
         return;

      ListenerGroup &group = itGroup->second;
      auto itListener = std::lower_bound( group.begin(), group.end(), a_Token.iCallbackIndex, []( const Listener &a_Listener, const std::size_t a_iIndex )
      {
         return a_Listener.iCallbackIndex < a_iIndex;
      } );
      if( itListener == group.end() or itListener->iCallbackIndex != a_Token.iCallbackIndex )
         return;

      // group is being iterated, so only mark the listener
      if( m_iDispatchDepth > 0 )
      {
         itListener->bAlive = false;
         m_bHasDeadListeners = true;
         return;
      }

      removedCallback = std::move( itListener->Callback );
      group.erase( itListener );
      if( group.empty() )
         m_mListenerGroups.erase( itGroup );
   }

   void EventBus::Dispatch( const std::type_index a_EventID, const void *a_pArguments )
   {
      // keeps the depth correct even if a callback throws
      struct DepthGuard
      {
         std::size_t &iDepth;
         DepthGuard( std::size_t &a_iDepth ) : iDepth( a_iDepth ) { ++iDepth; }
         ~DepthGuard() { --iDepth; }
      };

      {
         const DepthGuard guard( m_iDispatchDepth );

         auto itGroup = m_mListenerGroups.find( a_EventID );
         if( itGroup != m_mListenerGroups.end() )
         {
            for( Listener &listener : itGroup->second )
            {
               if( listener.bAlive )
                  listener.Callback( a_pArguments );
            }
         }
      }

      if( m_iDispatchDepth == 0 )
         this->FlushPending();
   }

   void EventBus::FlushPending()
   {
      // destroyed at the end of the scope, after the containers are consistent
      std::vector<CallbackWrapper> aRemovedCallbacks;

      if( m_bHasDeadListeners )
      {
         for( auto itGroup = m_mListenerGroups.begin(); itGroup != m_mListenerGroups.end(); )
         {
            ListenerGroup &group = itGroup->second;
            for( Listener &listener : group )
            {
               if( not listener.bAlive )
                  aRemovedCallbacks.push_back( std::move( listener.Callback ) );
            }
            std::erase_if( group, []( const Listener &a_Listener ) { return not a_Listener.bAlive; } );

            itGroup = group.empty() ? m_mListenerGroups.erase( itGroup ) : std::next( itGroup );
         }
         m_bHasDeadListeners = false;
      }

      auto aInsertions = std::move( m_aPendingInsertions );
      m_aPendingInsertions.clear();
      for( auto &[eventID, listener] : aInsertions )
         InsertSorted( m_mListenerGroups[eventID], std::move( listener ) );
   }

   void EventBus::InsertSorted( ListenerGroup &a_Group, Listener a_Listener )
   {
      // indices only grow, so this is almost always the end
      auto itPosition = std::upper_bound( a_Group.begin(), a_Group.end(), a_Listener.iCallbackIndex, []( const std::size_t a_iIndex, const Listener &a_Other )
      {
         return a_iIndex < a_Other.iCallbackIndex;
      } );
      a_Group.insert( itPosition, std::move( a_Listener ) );
   }
}
