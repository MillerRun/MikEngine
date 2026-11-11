#include "Events/IEventable.hpp"
#include "Events/Input/ButtonEvents.hpp"
#include <array>

namespace MK
{
   class Window;
}

namespace MK::Event
{
   class IKeyboardListener : public IEventable // IEventable for access to ButtonEvents
   {
   private:
      friend class MK::Window;

      static inline class KeyboardState
      {
      public:
         [[nodiscard]] auto IsButtonHeld( const EButton a_eButton ) const noexcept -> bool;

      private:
         friend class MK::Window;
         std::array<float, std::to_underlying( EButton::MAX_OR_UNKNOWN )> m_aButtonHeldTimings{};
      } keyboard;

   protected:
      [[nodiscard]] static auto Keyboard() noexcept -> const KeyboardState &;
   };
}
