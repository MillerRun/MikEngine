#pragma once

#include <Events/IEventable.hpp>
#include <Events/Input/ButtonTypes.hpp>
#include <glm/ext/vector_int2.hpp>

#include <array>

struct GLFWwindow;

namespace MK
{
   class Window final : public MK::Event::IEventable
   {
   public:
      Window();
      ~Window();

      Window( const Window&) = delete;
      void operator=( const Window&) = delete;

   public:
      [[nodiscard]] auto GetSize() const -> glm::ivec2;
      [[nodiscard]] auto ShouldClose() const -> bool;
      [[nodiscard]] auto IsButtonHeld( const EButton a_eButton ) const -> bool;
      void SwapBuffers();
      void PollEvents();
      void Update( const float dt );

   private:
      void KeyCallback( GLFWwindow *const a_pWindow, const int a_iKey, const int a_iScanCode, const int a_iAction, const int a_iMods );
      void OnButtonPressed( const EButton a_eButton );
      void OnButtonReleased( const EButton a_eButton );

   private:
      static constexpr auto k_iTotalButtons = std::to_underlying( EButton::MAX_OR_UNKNOWN );
      std::array<float, k_iTotalButtons> m_aButtonHeldTimings{};
      GLFWwindow *m_pWindow = nullptr;
   };
}
