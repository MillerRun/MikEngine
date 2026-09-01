#pragma once

/*
* MK::Event::IEventable is a class designed to be inherited from.
* By doing so, the parent class gains access to:
*     1. 'Subscribe' method - subscription for events. Subscription means providing
*         some callable object with a defined through template parameter event type;
*     2. 'Unsubsсribe' method - discards subcriptions for a given event type.
*         Only THIS object's subscriptions are discarded;
*     3. 'Call' method - invocation of all callbacks, subscribed for a given event.
*         Calls ALL callable objects, subscribed for a given event type, including
*         ones from other classes;
* 
*     Each MK::Event::IEventable's public method requires an event type as a
* template parameter. Event type can be any data type. Event type serves as both
* subscription id and callable's argument type, for example:
* 
*     Subscribe<MyEvent>( []( const MyEvent& ) {} );
*                 ^ id                 ^ callable argument
* 
*     MK::Event::IEventable automatically discards all event subscriptions upon its
* lifetime end, which generally speaking is the main reason for its existence and
* requirement to be inherited from.
* 
* For example code scroll to the end of the file.
* 
* Known issues/features:
*     -  No way to directly subscribe with non-static class method. However it can
*        be wrapped with a this-capturing lambda, or std::bind call, which is a bit
*        annoying
*     -  'Unsubsсribe' method discards all subscriptions in 'this' object for a
*        given event type, there is no way to discard only a particular one
*     -  Callable's return type must be void
*/

#include <vector>
#include <functional>
#include <typeindex>

namespace MK::Event
{
   struct Token;

   class IEventable
   {
   protected:
      virtual ~IEventable();

      IEventable();
      IEventable( IEventable & );
      IEventable &operator=( IEventable & );
      IEventable( IEventable && ) noexcept;
      IEventable &operator=( IEventable && ) noexcept;

   public:
      /**
      * @brief Subscribes a given callable to an event of a given type.
      * @tparam TEventType Both subscription id and callable's argument type.
      * @param a_Listener Callable which must accept 'const TEventType&' as an argument and return void
      */
      template<typename TEventType>
      void Subscribe( std::function<void( const TEventType & )> a_Listener )
      {
         const std::type_index id = typeid( TEventType );
         auto wrappedCallback = [callback = std::move( a_Listener )]( const void *a_pData )
         {
            callback( *static_cast<const TEventType *>( a_pData ) );
         };
         this->PassCallback( id, wrappedCallback );
      }

      /**
      * @brief Unsubscribes all callables provided by 'this' object from a given event type.
      * @tparam TEventType Event type to discard subscriptions for.
      */
      template<typename TEventType>
      void Unsubsсribe()
      {
         const std::type_index id = typeid( TEventType );
         this->ClearCallbacks( id );
      }

      /**
      * @brief Invokes ALL callables for EACH subscribed object for a given event type.
      * @tparam TEventType Event type to call for.
      * @param a_Arguments Event arguments which will be passes for each subscriber
      */
      template<typename TEventType>
      void Call( const TEventType a_Arguments )
      {
         const std::type_index id = typeid( TEventType );
         this->CallCallbacks( id, static_cast<const void *>( std::addressof( a_Arguments ) ) );
      }

   private:
      void PassCallback( const std::type_index a_EventID, std::function<void( const void * )> a_WrappedCallback );
      void ClearCallbacks( const std::type_index a_EventID );
      void CallCallbacks( const std::type_index a_EventID, const void *a_pArguments );

   private:
      std::vector<Token> m_aSubscriptionTokens;
   };
}

/*
* Example code:
*
*  struct EventButtonPressed
*  {
*     int keycode{};
*  };
*
*  struct MyClass : public MK::Event::IEventable
*  {
*     MyClass()
*     {
*        // passing class method or global function
*        this->Subscribe<EventButtonPressed>( &MyClass::HandleButtonPressed );
*
*        // passing lambda
*        this->Subscribe<EventButtonPressed>( []( const EventButtonPressed& e )
*        {
*           const int code = e.keycode;
*        } );
*        
*        // calls both method & lambda (and each other subscriber) with a given event parameter
*        this->Call<EventButtonPressed>( { .keycode = 222 } );
*        
*        // discard all subscriptions for THIS object
*        this->Unsubscribe<EventButtonPressed>();
*     }
*
*     static void HandleButtonPressed( const EventButtonPressed& e )
*     {
*        const int code = e.keycode;
*     }
*  };
*
* End of example code.
*/
