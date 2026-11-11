#pragma once

#include <Events/Input/InputListener.hpp>
#include <Events/Input/ButtonTypes.hpp>
#include <glm/ext/vector_int2.hpp>

struct GLFWwindow;

namespace MK
{
   class Window final : public MK::Event::InputListener
   {
   public:
      Window();
      ~Window();

      Window( const Window&) = delete;
      void operator=( const Window&) = delete;

   public:
      [[nodiscard]] auto Size() const -> glm::ivec2;
      [[nodiscard]] auto ShouldClose() const -> bool;
      void SwapBuffers();
      void PollEvents();
      void Update( const float dt );

   private:
      void KeyCallback( GLFWwindow *const a_pWindow, const int a_iKey, const int a_iScanCode, const int a_iAction, const int a_iMods );
      void OnButtonPressed( const EButton a_eButton );
      void OnButtonReleased( const EButton a_eButton );

   private:
      GLFWwindow *m_pWindow = nullptr;
   };
}
