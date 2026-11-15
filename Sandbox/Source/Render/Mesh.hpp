#pragma once

#include <vector>

#include "VAO.hpp"
#include "Texture.hpp"

namespace MK
{
   class Camera;

   namespace Render
   {
      class Shader;
   }
}

class Mesh
{
public:
   explicit Mesh( std::vector<Vertex> a_aVertices, std::vector<GLuint> a_aIndices, std::vector<Texture> a_aTextures );

   void Draw( MK::Render::Shader &a_Shader, MK::Camera &a_Camera );

private:
   std::vector<Vertex> m_aVertices;
   std::vector<GLuint> m_aIndices;
   std::vector<Texture> m_aTextures;

   VAO m_VAO;
};
