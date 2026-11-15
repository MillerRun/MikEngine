#pragma once

#include "Utils/Math.hpp"
#include "Events/Input/InputListener.hpp"


namespace MK
{
   namespace Render
   {
      class Shader;
   }

   class Camera final: public MK::Event::InputListener
   {
   public:
      explicit Camera( const glm::ivec2 a_v2Resolution, const glm::vec3 a_v3Position );

      void Update( const float dt );
      void Matrix( const Render::Shader &a_Shader, const char* a_sUniformName );
      void UpdateMatrix( const float a_fFov, const float a_fNearPlane, const float a_fFarPlane );

      glm::vec3 GetPosition() const
      {
         return m_v3Position;
      }

   private:
      void Move( const float dt );
      void Rotate( const glm::vec2 a_v2MouseDelta );

      void OnMouseScrolled( const EventMouseScrolled &a_Event );
      void OnButtonPressed( const EventButtonPressed &a_Event );

   private:
      glm::vec3 m_v3Position{};
      glm::vec3 m_v3StartPosition{};
      glm::vec3 m_v3Direction = glm::vec3( 0.f, 0.f, -1.f );
      glm::mat4 m_m4Camera = glm::mat4( 1.f );
      glm::ivec2 m_v2Resolution{};
      float m_fSpeed = 1.f;
      float m_fSensitivity = 0.125f;
   };
}
