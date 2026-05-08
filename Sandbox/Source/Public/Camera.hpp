#pragma once

#include <glm/glm.hpp>

struct GLFWwindow;
class Shader;

class Camera
{
public:
   explicit Camera( const int a_iWidth, const int a_iHeight, glm::vec3 a_v3Position );

   void Matrix( const float a_fFovAngle, const float a_fNearPlane, const float a_fFarPlane, const Shader &a_Shader, const char *const a_sUniform );
   void Inputs( GLFWwindow *a_pWindow );

private:
   glm::vec3 m_v3Position;
   glm::vec3 m_v3Direction{ 0.f, 0.f, -1.f };
   int m_iWidth = 0;
   int m_iHeight = 0;
   float m_fSpeed = 0.01f;
   float m_fSensitivity = 100.f;
   bool m_bFirstClick = false;
};
