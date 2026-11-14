#pragma once

#include "Events/IEventable.hpp"
#include "Events/Input/ButtonEvents.hpp"
#include "Events/Input/MouseEvents.hpp"
#include <glm/ext/vector_float2.hpp>
#include <array>
#include <utility>

namespace MK
{
   class Window;
}

namespace MK::Event
{
   class InputListener : public IEventable // IEventable for access to Button/Mouse events
   {
   private:
      friend class MK::Window;

      template<typename TButton>
      class ButtonState
      {
      public:
         [[nodiscard]] auto IsButtonDown( const TButton a_eButton ) const noexcept -> bool;
         [[nodiscard]] auto IsButtonHeld( const TButton a_eButton ) const noexcept -> bool;

      private:
         friend class MK::Window;

         [[nodiscard]] auto Timing( const TButton a_eButton ) const noexcept -> float;
         void Press( const TButton a_eButton ) noexcept;
         void Release( const TButton a_eButton ) noexcept;
         void Update( const float dt ) noexcept;

      private:
         std::array<float, std::to_underlying( TButton::MAX_OR_UNKNOWN )> m_aButtonHeldTimings{};
      };

      using KeyboardState = ButtonState<EButton>;

      class MouseState : public ButtonState<EMouseButton>
      {
      public:
         [[nodiscard]] auto Position() const noexcept -> glm::vec2;
         [[nodiscard]] auto Delta() const noexcept -> glm::vec2;
         [[nodiscard]] auto Scroll() const noexcept -> glm::vec2;

      private:
         friend class MK::Window;

         glm::vec2 m_v2Position{};
         glm::vec2 m_v2Delta{};
         glm::vec2 m_v2Scroll{};
         bool m_bHasPosition = false; // false until the first cursor event, or after re-entering the window
      };

      static inline KeyboardState keyboard;
      static inline MouseState mouse;

   protected:
      [[nodiscard]] static auto Keyboard() noexcept -> const KeyboardState &;
      [[nodiscard]] static auto Mouse() noexcept -> const MouseState &;
   };
}
