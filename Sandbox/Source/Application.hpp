#pragma once

#include "Window.hpp"
#include "Render/Camera.hpp"

#define ENGINE MK::Application::Get()

namespace MK
{
  class Application final
  {
  public:
    [[nodiscard]] static auto Get() noexcept -> Application*;

    void Run();
    [[nodiscard]] auto GetWindow() const noexcept -> const Window&;

  private:
    Application();
    void Update( const float dt );

  private:
    Window m_hWindow;
    Camera m_hCamera;
  };
}
