#pragma once

/*
* MK::Event::IEventable is a class designed to be inherited from.
* By doing so, the parent class gains access to:
*     1. 'Subscribe' method - subscription for events. Subscription means providing
*         some callable object with a defined through template parameter event type.
*         Returns an id, which can be used to discard this particular subscription;
*     2. 'Unsubscribe' method - discards either all subscriptions for a given event type,
*         or a single subscription by its id. Only THIS object's subscriptions are discarded;
*     3. 'Call' method - invocation of all callbacks, subscribed for a given event.
*         Calls ALL callable objects, subscribed for a given event type, including
*         ones from other classes. Callables are invoked in subscription order;
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
*     It is safe to subscribe, unsubscribe, destroy objects and call other events from
* inside of a callback:
*     - subscriptions made during a call become active after the outermost call ends;
*     - discarded subscriptions are never invoked again, even during the current call.
*
*     Copying or moving an MK::Event::IEventable does NOT transfer subscriptions, since
* callables usually capture the original 'this'. A new object starts with no subscriptions,
* an assigned object keeps its own ones.
*
* For example code scroll to the end of the file.
*
* Known issues/features:
*     -  Not thread safe, all methods must be called from the same thread
*     -  Callable's return type must be void
*/

#include <vector>
#include <functional>
#include <type_traits>
#include <typeindex>

namespace MK::Event
{
   struct Token;

   using SubscriptionID = std::size_t;

   class IEventable
   {
   protected:
      virtual ~IEventable();

      IEventable();
      IEventable( const IEventable & );
      IEventable &operator=( const IEventable & );
      IEventable( IEventable && ) noexcept;
      IEventable &operator=( IEventable && ) noexcept;

   public:
      /**
      * @brief Subscribes a given callable to an event of a given type.
      * @tparam TEventType Both subscription id and callable's argument type.
      * @param a_Listener Callable which must accept 'const TEventType&' as an argument and return void
      * @return Id which can be passed to 'Unsubscribe' to discard this subscription only.
      */
      template<typename TEventType>
      SubscriptionID Subscribe( std::function<void( const TEventType & )> a_Listener )
      {
         const std::type_index id = typeid( TEventType );
         auto wrappedCallback = [callback = std::move( a_Listener )]( const void *a_pData )
         {
            callback( *static_cast<const TEventType *>( a_pData ) );
         };
         return this->PassCallback( id, std::move( wrappedCallback ) );
      }

      /**
      * @brief Subscribes a given method of 'this' object to an event of a given type.
      * @tparam TEventType Both subscription id and method's argument type, can be deduced.
      * @param a_pMethod Method of a class derived from IEventable, which accepts 'const TEventType&'
      * @return Id which can be passed to 'Unsubscribe' to discard this subscription only.
      */
      template<typename TEventType, typename TClass>
      SubscriptionID Subscribe( void ( TClass:: *a_pMethod )( const TEventType & ) )
      {
         static_assert( std::is_base_of_v<IEventable, TClass>, "Method must belong to a class derived from IEventable" );
         return this->Subscribe<TEventType>( [this, a_pMethod]( const TEventType &a_Event )
         {
            ( static_cast<TClass *>( this )->*a_pMethod )( a_Event );
         } );
      }

      /**
      * @brief Subscribes a given const method of 'this' object to an event of a given type.
      * @tparam TEventType Both subscription id and method's argument type, can be deduced.
      * @param a_pMethod Const method of a class derived from IEventable, which accepts 'const TEventType&'
      * @return Id which can be passed to 'Unsubscribe' to discard this subscription only.
      */
      template<typename TEventType, typename TClass>
      SubscriptionID Subscribe( void ( TClass:: *a_pMethod )( const TEventType & ) const )
      {
         static_assert( std::is_base_of_v<IEventable, TClass>, "Method must belong to a class derived from IEventable" );
         return this->Subscribe<TEventType>( [this, a_pMethod]( const TEventType &a_Event )
         {
            ( static_cast<const TClass *>( this )->*a_pMethod )( a_Event );
         } );
      }

      /**
      * @brief Unsubscribes all callables provided by 'this' object from a given event type.
      * @tparam TEventType Event type to discard subscriptions for.
      */
      template<typename TEventType>
      void Unsubscribe()
      {
         const std::type_index id = typeid( TEventType );
         this->ClearCallbacks( id );
      }

      /**
      * @brief Unsubscribes a single callable provided by 'this' object.
      * @param a_ID Id returned by 'Subscribe'. Ids of other objects are ignored.
      */
      void Unsubscribe( const SubscriptionID a_ID );

      /**
      * @brief Invokes ALL callables for EACH subscribed object for a given event type.
      * @tparam TEventType Event type to call for, can be deduced.
      * @param a_Arguments Event arguments which will be passed to each subscriber
      */
      template<typename TEventType>
      void Call( const TEventType &a_Arguments )
      {
         const std::type_index id = typeid( TEventType );
         this->CallCallbacks( id, static_cast<const void *>( std::addressof( a_Arguments ) ) );
      }

   private:
      SubscriptionID PassCallback( const std::type_index a_EventID, std::function<void( const void * )> a_WrappedCallback );
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
*        // passing class method, event type is deduced
*        this->Subscribe( &MyClass::HandleButtonPressed );
*
*        // passing static method or global function
*        this->Subscribe<EventButtonPressed>( &MyClass::HandleButtonPressedStatic );
*
*        // passing lambda, keeping id to discard only this subscription later
*        const MK::Event::SubscriptionID id = this->Subscribe<EventButtonPressed>( []( const EventButtonPressed& e )
*        {
*           const int code = e.keycode;
*        } );
*
*        // calls all three (and each other subscriber) with a given event parameter
*        this->Call<EventButtonPressed>( { .keycode = 222 } );
*
*        // discard only the lambda subscription
*        this->Unsubscribe( id );
*
*        // discard all subscriptions for THIS object
*        this->Unsubscribe<EventButtonPressed>();
*     }
*
*     void HandleButtonPressed( const EventButtonPressed& e )
*     {
*        const int code = e.keycode;
*     }
*
*     static void HandleButtonPressedStatic( const EventButtonPressed& e )
*     {
*        const int code = e.keycode;
*     }
*  };
*
* End of example code.
*/
