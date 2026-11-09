#pragma once

#include "Token.hpp"
#include <Utils/Singleton.hpp>
#include <functional>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

namespace MK::Event
{
   /*
   * Containers are modified directly only when no dispatch is in progress. While
   * dispatching (including nested dispatches from inside callbacks):
   *     - new listeners are queued and become active once the outermost dispatch ends;
   *     - removed listeners are only marked as dead, so they are skipped right away,
   *       but destroyed once the outermost dispatch ends.
   * Removed callbacks are always destroyed after the containers are consistent again,
   * since their captures may own objects which call back into the bus on destruction.
   */
   class EventBus final : public MK::Utils::ISingleton<EventBus>
   {
      friend class MK::Utils::ISingleton<EventBus>;
      using CallbackWrapper = std::function<void( const void * )>;

      struct Listener
      {
         std::size_t iCallbackIndex;
         CallbackWrapper Callback;
         bool bAlive = true;
      };

      // sorted by callback index, which also is the subscription order
      using ListenerGroup = std::vector<Listener>;

   public:
      [[nodiscard]] Token AddListener( const std::type_index a_EventID, CallbackWrapper a_Listener );
      void RemoveListener( const Token &a_Token );
      void Dispatch( const std::type_index a_EventID, const void *a_pArguments );

   private:
      void FlushPending();
      static void InsertSorted( ListenerGroup &a_Group, Listener a_Listener );

   private:
      std::unordered_map<std::type_index, ListenerGroup> m_mListenerGroups;
      std::vector<std::pair<std::type_index, Listener>>  m_aPendingInsertions;
      std::size_t m_iAvailableIndex{};
      std::size_t m_iDispatchDepth{};
      bool m_bHasDeadListeners{};
   };
}
