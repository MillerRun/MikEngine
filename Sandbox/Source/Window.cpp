#include "Window.hpp"

#include <format>

#include <Events/Input/ButtonEvents.hpp>
#include <GLFW/glfw3.h>
#include <Utils/Assert.hpp>

namespace
{
   constexpr int k_iWindowWidth = 800;
   constexpr int k_iWindowHeight = 800;
   constexpr float k_fHoldThresholdSec = .3f;

   [[nodiscard]] constexpr
   auto KeyEnumFromMacro( const int a_iKeyMacro ) -> MK::EButton
   {
      switch( a_iKeyMacro )
      {
      case GLFW_KEY_SPACE: return MK::EButton::SPACE;
      case GLFW_KEY_APOSTROPHE: return MK::EButton::APOSTROPHE;
      case GLFW_KEY_COMMA: return MK::EButton::COMMA;
      case GLFW_KEY_MINUS: return MK::EButton::MINUS;
      case GLFW_KEY_PERIOD: return MK::EButton::PERIOD;
      case GLFW_KEY_SLASH: return MK::EButton::SLASH;
      case GLFW_KEY_0: return MK::EButton::NUM_0;
      case GLFW_KEY_1: return MK::EButton::NUM_1;
      case GLFW_KEY_2: return MK::EButton::NUM_2;
      case GLFW_KEY_3: return MK::EButton::NUM_3;
      case GLFW_KEY_4: return MK::EButton::NUM_4;
      case GLFW_KEY_5: return MK::EButton::NUM_5;
      case GLFW_KEY_6: return MK::EButton::NUM_6;
      case GLFW_KEY_7: return MK::EButton::NUM_7;
      case GLFW_KEY_8: return MK::EButton::NUM_8;
      case GLFW_KEY_9: return MK::EButton::NUM_9;
      case GLFW_KEY_SEMICOLON: return MK::EButton::SEMICOLON;
      case GLFW_KEY_EQUAL: return MK::EButton::EQUAL;
      case GLFW_KEY_A: return MK::EButton::A;
      case GLFW_KEY_B: return MK::EButton::B;
      case GLFW_KEY_C: return MK::EButton::C;
      case GLFW_KEY_D: return MK::EButton::D;
      case GLFW_KEY_E: return MK::EButton::E;
      case GLFW_KEY_F: return MK::EButton::F;
      case GLFW_KEY_G: return MK::EButton::G;
      case GLFW_KEY_H: return MK::EButton::H;
      case GLFW_KEY_I: return MK::EButton::I;
      case GLFW_KEY_J: return MK::EButton::J;
      case GLFW_KEY_K: return MK::EButton::K;
      case GLFW_KEY_L: return MK::EButton::L;
      case GLFW_KEY_M: return MK::EButton::M;
      case GLFW_KEY_N: return MK::EButton::N;
      case GLFW_KEY_O: return MK::EButton::O;
      case GLFW_KEY_P: return MK::EButton::P;
      case GLFW_KEY_Q: return MK::EButton::Q;
      case GLFW_KEY_R: return MK::EButton::R;
      case GLFW_KEY_S: return MK::EButton::S;
      case GLFW_KEY_T: return MK::EButton::T;
      case GLFW_KEY_U: return MK::EButton::U;
      case GLFW_KEY_V: return MK::EButton::V;
      case GLFW_KEY_W: return MK::EButton::W;
      case GLFW_KEY_X: return MK::EButton::X;
      case GLFW_KEY_Y: return MK::EButton::Y;
      case GLFW_KEY_Z: return MK::EButton::Z;
      case GLFW_KEY_LEFT_BRACKET: return MK::EButton::LEFT_BRACKET;
      case GLFW_KEY_BACKSLASH: return MK::EButton::BACKSLASH;
      case GLFW_KEY_RIGHT_BRACKET: return MK::EButton::RIGHT_BRACKET;
      case GLFW_KEY_GRAVE_ACCENT: return MK::EButton::GRAVE_ACCENT;
      case GLFW_KEY_WORLD_1: return MK::EButton::WORLD_1;
      case GLFW_KEY_WORLD_2: return MK::EButton::WORLD_2;
      case GLFW_KEY_ESCAPE: return MK::EButton::ESCAPE;
      case GLFW_KEY_ENTER: return MK::EButton::ENTER;
      case GLFW_KEY_TAB: return MK::EButton::TAB;
      case GLFW_KEY_BACKSPACE: return MK::EButton::BACKSPACE;
      case GLFW_KEY_INSERT: return MK::EButton::INSERT;
      case GLFW_KEY_DELETE: return MK::EButton::DELETE;
      case GLFW_KEY_RIGHT: return MK::EButton::RIGHT;
      case GLFW_KEY_LEFT: return MK::EButton::LEFT;
      case GLFW_KEY_DOWN: return MK::EButton::DOWN;
      case GLFW_KEY_UP: return MK::EButton::UP;
      case GLFW_KEY_PAGE_UP: return MK::EButton::PAGE_UP;
      case GLFW_KEY_PAGE_DOWN: return MK::EButton::PAGE_DOWN;
      case GLFW_KEY_HOME: return MK::EButton::HOME;
      case GLFW_KEY_END: return MK::EButton::END;
      case GLFW_KEY_CAPS_LOCK: return MK::EButton::CAPS_LOCK;
      case GLFW_KEY_SCROLL_LOCK: return MK::EButton::SCROLL_LOCK;
      case GLFW_KEY_NUM_LOCK: return MK::EButton::NUM_LOCK;
      case GLFW_KEY_PRINT_SCREEN: return MK::EButton::PRINT_SCREEN;
      case GLFW_KEY_PAUSE: return MK::EButton::PAUSE;
      case GLFW_KEY_F1: return MK::EButton::F1;
      case GLFW_KEY_F2: return MK::EButton::F2;
      case GLFW_KEY_F3: return MK::EButton::F3;
      case GLFW_KEY_F4: return MK::EButton::F4;
      case GLFW_KEY_F5: return MK::EButton::F5;
      case GLFW_KEY_F6: return MK::EButton::F6;
      case GLFW_KEY_F7: return MK::EButton::F7;
      case GLFW_KEY_F8: return MK::EButton::F8;
      case GLFW_KEY_F9: return MK::EButton::F9;
      case GLFW_KEY_F10: return MK::EButton::F10;
      case GLFW_KEY_F11: return MK::EButton::F11;
      case GLFW_KEY_F12: return MK::EButton::F12;
      case GLFW_KEY_F13: return MK::EButton::F13;
      case GLFW_KEY_F14: return MK::EButton::F14;
      case GLFW_KEY_F15: return MK::EButton::F15;
      case GLFW_KEY_F16: return MK::EButton::F16;
      case GLFW_KEY_F17: return MK::EButton::F17;
      case GLFW_KEY_F18: return MK::EButton::F18;
      case GLFW_KEY_F19: return MK::EButton::F19;
      case GLFW_KEY_F20: return MK::EButton::F20;
      case GLFW_KEY_F21: return MK::EButton::F21;
      case GLFW_KEY_F22: return MK::EButton::F22;
      case GLFW_KEY_F23: return MK::EButton::F23;
      case GLFW_KEY_F24: return MK::EButton::F24;
      case GLFW_KEY_F25: return MK::EButton::F25;
      case GLFW_KEY_KP_0: return MK::EButton::KP_0;
      case GLFW_KEY_KP_1: return MK::EButton::KP_1;
      case GLFW_KEY_KP_2: return MK::EButton::KP_2;
      case GLFW_KEY_KP_3: return MK::EButton::KP_3;
      case GLFW_KEY_KP_4: return MK::EButton::KP_4;
      case GLFW_KEY_KP_5: return MK::EButton::KP_5;
      case GLFW_KEY_KP_6: return MK::EButton::KP_6;
      case GLFW_KEY_KP_7: return MK::EButton::KP_7;
      case GLFW_KEY_KP_8: return MK::EButton::KP_8;
      case GLFW_KEY_KP_9: return MK::EButton::KP_9;
      case GLFW_KEY_KP_DECIMAL: return MK::EButton::KP_DECIMAL;
      case GLFW_KEY_KP_DIVIDE: return MK::EButton::KP_DIVIDE;
      case GLFW_KEY_KP_MULTIPLY: return MK::EButton::KP_MULTIPLY;
      case GLFW_KEY_KP_SUBTRACT: return MK::EButton::KP_SUBTRACT;
      case GLFW_KEY_KP_ADD: return MK::EButton::KP_ADD;
      case GLFW_KEY_KP_ENTER: return MK::EButton::KP_ENTER;
      case GLFW_KEY_KP_EQUAL: return MK::EButton::KP_EQUAL;
      case GLFW_KEY_LEFT_SHIFT: return MK::EButton::LEFT_SHIFT;
      case GLFW_KEY_LEFT_CONTROL: return MK::EButton::LEFT_CONTROL;
      case GLFW_KEY_LEFT_ALT: return MK::EButton::LEFT_ALT;
      case GLFW_KEY_LEFT_SUPER: return MK::EButton::LEFT_SUPER;
      case GLFW_KEY_RIGHT_SHIFT: return MK::EButton::RIGHT_SHIFT;
      case GLFW_KEY_RIGHT_CONTROL: return MK::EButton::RIGHT_CONTROL;
      case GLFW_KEY_RIGHT_ALT: return MK::EButton::RIGHT_ALT;
      case GLFW_KEY_RIGHT_SUPER: return MK::EButton::RIGHT_SUPER;
      case GLFW_KEY_MENU: return MK::EButton::MENU;
      default: return MK::EButton::MAX_OR_UNKNOWN;
      }
   }
}

namespace MK
{
   Window::Window()
   {
      glfwInit();
      glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
      glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
      glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

      m_pWindow = glfwCreateWindow( k_iWindowWidth, k_iWindowHeight, "My window", nullptr, nullptr );
      if( not m_pWindow )
      {
         glfwTerminate();
         throw std::runtime_error( std::format( "Failed to create window of size {}x{}", k_iWindowWidth, k_iWindowHeight ) );
      }
      glfwMakeContextCurrent( m_pWindow );
      glfwSetWindowUserPointer( m_pWindow, this );

      glfwSetKeyCallback( m_pWindow, []( GLFWwindow *const a_pWindow, const int a_iKey, const int a_iScanCode, const int a_iAction, const int a_iMods )
      {
         if( auto pWindow = static_cast<MK::Window *>( glfwGetWindowUserPointer( a_pWindow ) ) ) [[likely]]
            pWindow->KeyCallback( a_pWindow, a_iKey, a_iScanCode, a_iAction, a_iMods );
      } );
   }

   Window::~Window()
   {
      glfwSetKeyCallback( m_pWindow, nullptr );
      glfwDestroyWindow( m_pWindow );
      glfwTerminate();
   }

   auto Window::Size() const -> glm::ivec2
   {
      return { k_iWindowWidth, k_iWindowHeight };
   }

   auto Window::ShouldClose() const -> bool
   {
      return glfwWindowShouldClose( m_pWindow );
   }

   void Window::SwapBuffers()
   {
      glfwSwapBuffers( m_pWindow );
   }

   void Window::PollEvents()
   {
      glfwPollEvents();
   }

   void Window::Update( const float dt )
   {
      for( auto &fTimer : keyboard.m_aButtonHeldTimings )
      {
         if( fTimer > 0.f )
            fTimer += dt;
      }
   }

   void Window::KeyCallback( GLFWwindow *const a_pWindow, const int a_iKey, const int a_iScanCode, const int a_iAction, const int a_iMods )
   {
      const auto eButton = KeyEnumFromMacro( a_iKey );
      if( eButton == EButton::MAX_OR_UNKNOWN ) [[unlikely]]
      {
         MKASSERT( false, "Unknown button pressed: {}", a_iKey );
         return;
      }

      if( a_iAction == GLFW_PRESS )
      {
         this->OnButtonPressed( eButton );
         this->Call<EventButtonPressed>( EventButtonPressed{ .eCode = eButton } );
      }
      else if( a_iAction == GLFW_RELEASE )
      {
         this->OnButtonReleased( eButton );
         this->Call<EventButtonReleased>( EventButtonReleased{ .eCode = eButton } );
      }
   }

   void Window::OnButtonPressed( const EButton a_eButton )
   {
      keyboard.m_aButtonHeldTimings[std::to_underlying( a_eButton )] = .01f;
   }

   void Window::OnButtonReleased( const EButton a_eButton )
   {
      keyboard.m_aButtonHeldTimings[std::to_underlying( a_eButton )] = 0.f;
   }
}
