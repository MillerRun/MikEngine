#include "IKeyboardListener.hpp"
#include "Utils/Assert.hpp"

namespace MK::Event
{
   auto IKeyboardListener::KeyboardState::IsButtonHeld( const EButton a_eButton ) const noexcept -> bool
   {
      const auto iButtonIndex = std::to_underlying( a_eButton );
      if( iButtonIndex >= std::to_underlying( EButton::MAX_OR_UNKNOWN ) ) [[unlikely]]
      {
         MKASSERT( false, "Unknown button pressed: {}", iButtonIndex );
         return false;
      }
      constexpr float k_fHoldThresholdSec = .3f;
      return m_aButtonHeldTimings[iButtonIndex] > k_fHoldThresholdSec;
   }

   auto IKeyboardListener::Keyboard() noexcept -> const KeyboardState &
   {
      return keyboard;
   }
}
