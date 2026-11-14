#include "InputListener.hpp"
#include "Utils/Assert.hpp"

namespace MK::Event
{
   namespace
   {
      constexpr float k_fHoldThresholdSec = .3f;
      constexpr float k_fPressedTiming = .01f; // non-zero marks the button as down
   }

   template<typename TButton>
   auto InputListener::ButtonState<TButton>::IsButtonDown( const TButton a_eButton ) const noexcept -> bool
   {
      return this->Timing( a_eButton ) > 0.f;
   }

   template<typename TButton>
   auto InputListener::ButtonState<TButton>::IsButtonHeld( const TButton a_eButton ) const noexcept -> bool
   {
      return this->Timing( a_eButton ) > k_fHoldThresholdSec;
   }

   template<typename TButton>
   auto InputListener::ButtonState<TButton>::Timing( const TButton a_eButton ) const noexcept -> float
   {
      const auto iButtonIndex = std::to_underlying( a_eButton );
      if( iButtonIndex >= std::to_underlying( TButton::MAX_OR_UNKNOWN ) ) [[unlikely]]
      {
         MKASSERT( false, "Unknown button requested: {}", iButtonIndex );
         return 0.f;
      }
      return m_aButtonHeldTimings[iButtonIndex];
   }

   template<typename TButton>
   void InputListener::ButtonState<TButton>::Press( const TButton a_eButton ) noexcept
   {
      m_aButtonHeldTimings[std::to_underlying( a_eButton )] = k_fPressedTiming;
   }

   template<typename TButton>
   void InputListener::ButtonState<TButton>::Release( const TButton a_eButton ) noexcept
   {
      m_aButtonHeldTimings[std::to_underlying( a_eButton )] = 0.f;
   }

   template<typename TButton>
   void InputListener::ButtonState<TButton>::Update( const float dt ) noexcept
   {
      for( auto &fTimer : m_aButtonHeldTimings )
      {
         if( fTimer > 0.f )
            fTimer += dt;
      }
   }

   template class InputListener::ButtonState<EButton>;
   template class InputListener::ButtonState<EMouseButton>;

   auto InputListener::MouseState::Position() const noexcept -> glm::vec2
   {
      return m_v2Position;
   }

   auto InputListener::MouseState::Delta() const noexcept -> glm::vec2
   {
      return m_v2Delta;
   }

   auto InputListener::MouseState::Scroll() const noexcept -> glm::vec2
   {
      return m_v2Scroll;
   }

   auto InputListener::Keyboard() noexcept -> const KeyboardState &
   {
      return keyboard;
   }

   auto InputListener::Mouse() noexcept -> const MouseState &
   {
      return mouse;
   }
}
