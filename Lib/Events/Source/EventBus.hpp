#pragma once

#include <Utils/Singleton.hpp>
#include <functional>
#include <typeindex>
#include <unordered_map>
#include <mutex>

namespace MK::Event
{
   class EventBus final : public MK::Utils::ISingleton<EventBus>
   {
      friend class MK::Utils::ISingleton<EventBus>;

      using CallbackWrapper = std::function<void( const void * )>;
      using CallbackID      = std::size_t;
      using CallbackGroup   = std::unordered_map<CallbackID, CallbackWrapper>;
      using EventIndex      = std::type_index;

   public:
      template<typename TEventType>
      int AddListener( std::function<void( const TEventType & )> a_Listener )
      {
         const EventIndex eventIndex = std::type_index( typeid( TEventType ) );
         const CallbackID id = m_iAvailableID++;
         CallbackGroup &group = m_mListenerGroups[eventIndex];
         group[id] = [callback = std::move( a_Listener )]( const void *a_pData )
         {
            callback( *static_cast<const TEventType *>( a_pData ) );
         };
         MKASSERT( false, "TODO: return type Token" );
         return (int)id;
      }

      template<typename TEventType>
      void Dispatch( const TEventType &a_EventArguments )
      {
         std::lock_guard( m_Mutex );
         const EventIndex eventIndex = std::type_index( typeid( TEventType ) );

         auto itGroup = m_mListenerGroups.find( eventIndex );
         if( itGroup == m_mListenerGroups.end() )
            return;

         for( auto &[iCallbackID, Callback] : itGroup->second )
            Callback( static_cast<const void *>( a_EventArguments ) );
      }

   private:
      std::unordered_map<EventIndex, CallbackGroup> m_mListenerGroups;
      std::mutex m_Mutex;
      CallbackID m_iAvailableID{};
   };
}
