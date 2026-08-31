#pragma once

#include <vector>
#include <functional>

namespace MK::Event
{
   class Token;

   class EventListener
   {
   public:
      EventListener( EventListener & )                      = default;
      EventListener &operator=( EventListener & )           = default;
      EventListener( EventListener && ) noexcept            = default;
      EventListener &operator=( EventListener && ) noexcept = default;
      virtual ~EventListener()                              = default;

   public:
      
      template<typename TEventType>
      void On( std::function<void( const TEventType& )> a_Listener )
      {

      }

   private:
      std::vector<Token> m_aSubscriptionTokens;
   };


   struct EventMouseClick
   {
      float x{}, y{};
   };

   class MyClass : public EventListener
   {
      void f()
      {
         On<EventMouseClick>( []( const EventMouseClick& )
         {
            
         } );
      }
   };


}

