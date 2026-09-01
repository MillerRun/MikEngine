#pragma once

#include "Token.hpp"
#include <Utils/Singleton.hpp>
#include <functional>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace MK::Event
{
   class EventBus final : public MK::Utils::ISingleton<EventBus>
   {
      friend class MK::Utils::ISingleton<EventBus>;
      using CallbackWrapper = std::function<void( const void * )>;
      using CallbackGroup = std::unordered_map<std::size_t, CallbackWrapper>;

      struct RemovalRequest
      {
         std::type_index EventID;
         std::size_t iCallbackIndex;
      };

      struct InsertionRequest
      {
         std::type_index EventID;
         std::size_t iCallbackIndex;
         CallbackWrapper Callback;
      };

   public:
      [[nodiscard]] Token AddListener( const std::type_index a_EventID, CallbackWrapper a_Listener );
      void RemoveListener( const std::type_index a_EventID, const std::size_t a_iListenerIndex );
      void Dispatch( const std::type_index a_EventID, const void *a_pArguments );

   private:
      void CheckInsertionQueue();
      void CheckRemovalQueue();

   private:
      std::unordered_map<std::type_index, CallbackGroup> m_mListenerGroups;
      std::vector<RemovalRequest>   m_aRemovalQueue;
      std::vector<InsertionRequest> m_aInsertionQueue;
      std::size_t m_iAvailableIndex{};
   };
}
