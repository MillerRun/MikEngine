#pragma once

#include <vector>
#include <string>
#include <Utils/Math.hpp>

namespace MK
{
   struct Vertex
   {
      glm::vec3 v3Position{};
      glm::vec3 v3Normal{};
      glm::vec2 v2Texture{};
      glm::vec3 v3Color{ 1.f };
   };

   struct Mesh
   {
      using Index = std::uint32_t;

      std::vector<Vertex> aVertices;
      std::vector<Index> aIndices;
      int iMaterialIndex = -1;
   };

   struct MaterialDesc
   {
      std::string sTexturePath;
      glm::vec4 v4Color{ 1.f };
   };

   struct Node
   {
      glm::vec3 v3Translation{};
      glm::quat qRotation{ 1.f, 0.f, 0.f, 0.f };
      glm::vec3 v3Scale{ 1.f };
      int iMeshIndex = -1;
      int iParentIndex = -1;
   };

   struct Model
   {
      std::vector<Mesh> aMeshes;
      std::vector<MaterialDesc> aMaterials;
      std::vector<Node> aNodes;
   };
}
