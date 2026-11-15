#pragma once

#include "Window.hpp"
#include "Render/Camera.hpp"
#include "Utils/Singleton.hpp"

#define ENGINE MK::Application::Get()

namespace MK
{
  class Application final : public MK::Utils::ISingleton<Application>
  {
     friend class MK::Utils::ISingleton<Application>;

  public:
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
