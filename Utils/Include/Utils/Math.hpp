#pragma once

#define GLM_FORCE_LEFT_HANDED
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#undef GLM_ENABLE_EXPERIMENTAL

namespace MK::Math
{
   [[nodiscard]] constexpr
   glm::mat4 RemapForOpenGL( const glm::mat4 &a_m4Value )
   {
      glm::mat4 m4Remap( 1.f );
      m4Remap[2][2] = 2.f;
      m4Remap[3][2] = -1.f;
      return m4Remap * a_m4Value;
   }
}
