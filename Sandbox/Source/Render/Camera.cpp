#include "Camera.hpp"

#include "Common.hpp"
#include "Shader.hpp"
#include <algorithm>

namespace
{
   constexpr glm::vec3 k_v3UpDirection{ 0.f, 1.f, 0.f };
   constexpr float k_fSprintMultiplier = 5.f;
}

namespace MK
{
   Camera::Camera( const glm::ivec2 a_v2Resolution, const glm::vec3 a_v3Position )
      : m_v3Position{ a_v3Position }
      , m_v3StartPosition{ a_v3Position }
      , m_v2Resolution{ a_v2Resolution }
   {
      this->Subscribe<EventMouseScrolled>( &Camera::OnMouseScrolled );
      this->Subscribe<EventButtonPressed>( &Camera::OnButtonPressed );
   }

   void Camera::Update( const float dt )
   {
      this->Move( dt );

      if( Mouse().IsButtonDown( EMouseButton::LEFT ) )
         this->Rotate( Mouse().Delta() );
   }

   void Camera::Matrix( const Shader & a_Shader, const char* a_sUniformName )
   {
      GLCHECK( glUniformMatrix4fv( glGetUniformLocation( a_Shader.Get(), a_sUniformName ), 1, GL_FALSE, glm::value_ptr( m_m4Camera ) ) );
   }

   void Camera::UpdateMatrix( const float a_fFov, const float a_fNearPlane, const float a_fFarPlane )
   {
      const glm::mat4 v4View = glm::lookAt( m_v3Position, m_v3Position + m_v3Direction, k_v3UpDirection );
      const glm::mat4 v4Proj = glm::perspective( glm::radians( a_fFov ), static_cast<float>( m_v2Resolution.x ) / m_v2Resolution.y, a_fNearPlane, a_fFarPlane );
      m_m4Camera = MK::Math::RemapForOpenGL( v4Proj ) * v4View;
   }

   void Camera::Move( const float dt )
   {
      const glm::vec3 v3Side = glm::normalize( glm::cross( k_v3UpDirection, m_v3Direction ) );

      glm::vec3 v3Input{};
      if( Keyboard().IsButtonDown( EButton::W ) )            v3Input += m_v3Direction;
      if( Keyboard().IsButtonDown( EButton::S ) )            v3Input -= m_v3Direction;
      if( Keyboard().IsButtonDown( EButton::A ) )            v3Input -= v3Side;
      if( Keyboard().IsButtonDown( EButton::D ) )            v3Input += v3Side;
      if( Keyboard().IsButtonDown( EButton::SPACE ) )        v3Input += k_v3UpDirection;
      if( Keyboard().IsButtonDown( EButton::LEFT_CONTROL ) ) v3Input -= k_v3UpDirection;

      if( v3Input == glm::vec3{ 0.f } )
         return;

      const float fSpeed = Keyboard().IsButtonDown( EButton::LEFT_SHIFT )
         ? m_fSpeed * k_fSprintMultiplier
         : m_fSpeed;

      m_v3Position += glm::normalize( v3Input ) * fSpeed * dt;
   }

   void Camera::Rotate( const glm::vec2 a_v2MouseDelta )
   {
      const float fYaw = m_fSensitivity * a_v2MouseDelta.x;
      const float fPitch = m_fSensitivity * a_v2MouseDelta.y;

      const glm::vec3 v3Side = glm::normalize( glm::cross( k_v3UpDirection, m_v3Direction ) );
      const glm::vec3 v3NewDirection = glm::rotate( m_v3Direction, glm::radians( fPitch ), v3Side );

      if( glm::angle( v3NewDirection, k_v3UpDirection ) > glm::radians( 5.f ) and glm::angle( v3NewDirection, -k_v3UpDirection ) > glm::radians( 5.f ) )
         m_v3Direction = v3NewDirection;

      m_v3Direction = glm::rotate( m_v3Direction, glm::radians( fYaw ), k_v3UpDirection );
   }

   void Camera::OnMouseScrolled( const EventMouseScrolled & a_Event )
   {
      m_fSpeed = std::clamp( m_fSpeed * std::pow( 1.2f, a_Event.v2Offset.y ), 0.1f, 50.f );
   }

   void Camera::OnButtonPressed( const EventButtonPressed & a_Event )
   {
      switch( a_Event.eCode )
      {
      case EButton::R: // reset camera
      {
         m_v3Position = m_v3StartPosition;
         m_v3Direction = glm::vec3( 0.f, 0.f, -1.f );
         break;
      }
      }
   }
}
