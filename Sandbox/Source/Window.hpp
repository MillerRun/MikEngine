#pragma once

#include <Events/Input/InputListener.hpp>
#include <glm/ext/vector_int2.hpp>

struct GLFWwindow;

namespace MK
{
   class Window final : public MK::Event::InputListener
   {
   public:
      Window();
      ~Window();

      Window( const Window & ) = delete;
      void operator=( const Window & ) = delete;

   public:
      [[nodiscard]] auto Size() const -> glm::ivec2;
      [[nodiscard]] auto ShouldClose() const -> bool;
      void SwapBuffers();
      void PollEvents();
      void Update( const float dt );

   private:
      void KeyCallback( GLFWwindow *const a_pWindow, const int a_iKey, const int a_iScanCode, const int a_iAction, const int a_iMods );
      void MouseButtonCallback( GLFWwindow *const a_pWindow, const int a_iButton, const int a_iAction, const int a_iMods );
      void CursorPositionCallback( GLFWwindow *const a_pWindow, const double a_dX, const double a_dY );
      void CursorEnterCallback( GLFWwindow *const a_pWindow, const int a_iEntered );
      void ScrollCallback( GLFWwindow *const a_pWindow, const double a_dOffsetX, const double a_dOffsetY );

   private:
      GLFWwindow *m_pWindow = nullptr;
   };
}
