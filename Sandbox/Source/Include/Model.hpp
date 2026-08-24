#pragma once

#include <string_view>
#include <vector>
#include <Utils/Math.hpp>
#include "Mesh.hpp"

class Shader;
class Camera;

struct SceneObjectInfo
{
   glm::vec3 v3Translation;
   glm::quat qRotation;
   glm::vec3 v3Scale = glm::vec3( 1.f, 1.f, 1.f );
   glm::mat4 m4WorldTransform = glm::mat4( 1.f );
   std::int32_t iParentIndex = -1;
   std::int32_t iMeshIndex = -1;
};

class Model
{
public:
   explicit Model( const std::string_view a_sFilePath, const std::string_view a_sFileName );

   void Draw( Shader &a_Shader, Camera &a_Camera );

private:
   std::vector<Mesh> m_aMeshes;
   std::vector<SceneObjectInfo> m_aSceneObjectsInfo;
};
