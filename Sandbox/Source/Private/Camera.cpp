#include "Camera.hpp"

#include "Shader.hpp"

#include <GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

namespace
{
   constexpr glm::vec3 k_v3UpDirection{ 0.f, 1.f, 0.f };
}

Camera::Camera( const int a_iWidth, const int a_iHeight, const glm::vec3 a_v3Position )
   : m_iWidth( a_iWidth )
   , m_iHeight( a_iHeight )
   , m_v3Position( a_v3Position )
{
}

void Camera::Matrix( const float a_fFovAngle, const float a_fNearPlane, const float a_fFarPlane, const Shader &a_Shader, const char *const a_sUniform )
{
   const glm::mat4 v4View = glm::lookAt( m_v3Position, m_v3Position + m_v3Direction, k_v3UpDirection );
   const glm::mat4 v4Proj = glm::perspective( glm::radians( a_fFovAngle ), static_cast<float>( m_iWidth ) / m_iHeight, a_fNearPlane, a_fFarPlane );

   glUniformMatrix4fv( glGetUniformLocation( a_Shader.GetID(), a_sUniform ), 1, GL_FALSE, glm::value_ptr( v4Proj * v4View ) );
}

void Camera::Inputs( GLFWwindow * const a_pWindow )
{
   if( glfwGetKey( a_pWindow, GLFW_KEY_W ) == GLFW_PRESS )
   {
      m_v3Position += m_fSpeed * m_v3Direction;
   }
   if( glfwGetKey( a_pWindow, GLFW_KEY_A ) == GLFW_PRESS )
   {
      m_v3Position += m_fSpeed * -glm::normalize( glm::cross( m_v3Direction, k_v3UpDirection ) );
   }
   if( glfwGetKey( a_pWindow, GLFW_KEY_S ) == GLFW_PRESS )
   {
      m_v3Position += m_fSpeed * -m_v3Direction;
   }
   if( glfwGetKey( a_pWindow, GLFW_KEY_D ) == GLFW_PRESS )
   {
      m_v3Position += m_fSpeed * glm::normalize( glm::cross( m_v3Direction, k_v3UpDirection ) );;
   }

   if( glfwGetKey( a_pWindow, GLFW_KEY_SPACE ) == GLFW_PRESS )
   {
      m_v3Position += m_fSpeed * k_v3UpDirection;
   }
   else if( glfwGetKey( a_pWindow, GLFW_KEY_LEFT_CONTROL ) == GLFW_PRESS )
   {
      m_v3Position += m_fSpeed * -k_v3UpDirection;
   }

   if( glfwGetKey( a_pWindow, GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS )
   {
      m_fSpeed = 0.05;
   }
   else if( glfwGetKey( a_pWindow, GLFW_KEY_LEFT_SHIFT ) == GLFW_RELEASE )
   {
      m_fSpeed = 0.01;
   }

   if( glfwGetMouseButton( a_pWindow, GLFW_MOUSE_BUTTON_LEFT ) == GLFW_PRESS )
   {
      glfwSetInputMode( a_pWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN );

      if( m_bFirstClick )
      {
         glfwSetCursorPos( a_pWindow, m_iWidth / 2.f, m_iHeight / 2.f );
         m_bFirstClick = false;
      }

      glm::vec<2, double, glm::packed_highp> v2MousePosition;
      glfwGetCursorPos( a_pWindow, &v2MousePosition.x, &v2MousePosition.y );

      const glm::vec2 v2Rotation{
            m_fSensitivity * ( static_cast<float>( v2MousePosition.y ) - m_iHeight / 2.f ) / m_iHeight
         ,  m_fSensitivity * ( static_cast<float>( v2MousePosition.x ) - m_iHeight / 2.f ) / m_iHeight
      };

      const glm::vec3 v3NewDirection = glm::rotate( m_v3Direction, glm::radians( -v2Rotation.x ), glm::normalize( glm::cross( m_v3Direction, k_v3UpDirection ) ) );
      if( glm::angle( v3NewDirection, k_v3UpDirection ) > glm::radians( 5.f ) and glm::angle( v3NewDirection, -k_v3UpDirection ) > glm::radians( 5.f ) )
      {
         m_v3Direction = v3NewDirection;
      }
      m_v3Direction = glm::rotate( m_v3Direction, glm::radians( -v2Rotation.y ), k_v3UpDirection );
      glfwSetCursorPos( a_pWindow, m_iWidth / 2.f, m_iHeight / 2.f );
   }
   else if( glfwGetMouseButton( a_pWindow, GLFW_MOUSE_BUTTON_LEFT ) == GLFW_RELEASE )
   {
      glfwSetInputMode( a_pWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN );
      m_bFirstClick = true;
   }
}
