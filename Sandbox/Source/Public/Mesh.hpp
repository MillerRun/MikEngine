#pragma once

#include <vector>

#include "VAO.hpp"
#include "Texture.hpp"

class Shader;
class Camera;

class Mesh
{
public:
   explicit Mesh( std::vector<Vertex> a_aVertices, std::vector<GLuint> a_aIndices, std::vector<Texture> a_aTextures );

   void Draw( Shader &a_Shader, Camera &a_Camera );

private:
   std::vector<Vertex> m_aVertices;
   std::vector<GLuint> m_aIndices;
   std::vector<Texture> m_aTextures;

   VAO m_VAO;
};
