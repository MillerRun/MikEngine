#pragma once

#include "Token.hpp"
#include <Utils/Singleton.hpp>
#include <functional>
#include <typeindex>
#include <unordered_map>

namespace MK::Event
{
   class EventBus final : public MK::Utils::ISingleton<EventBus>
   {
      friend class MK::Utils::ISingleton<EventBus>;
      using CallbackGroup = std::unordered_map<std::size_t, std::function<void( const void * )>>;

   public:
      [[nodiscard]] Token AddListener( const std::type_index a_EventID, std::function<void( const void * )> a_Listener );
      void RemoveListener( const std::type_index a_EventID, const std::size_t a_iListenerIndex );

      template<typename TEventType>
      void Dispatch( const TEventType &a_EventArguments )
      {
         const std::type_index id = typeid( TEventType );

         auto itGroup = m_mListenerGroups.find( id );
         if( itGroup == m_mListenerGroups.end() )
            return;

         for( auto &[_, Callback] : itGroup->second )
            Callback( static_cast<const void *>( a_EventArguments ) );
      }

   private:
      std::unordered_map<std::type_index, CallbackGroup> m_mListenerGroups;
      std::size_t m_iAvailableIndex{};
   };
}
