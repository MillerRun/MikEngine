#pragma once

#include <glad/glad.h>
#include "Buffer.hpp"

class VAO
{
public:
   VAO();

   void LinkVBO( Buffer<EBufferType::VERTEX, GLfloat> a_VBO, GLuint a_iLayout );
   void Bind();
   void Unbind();
   void Delete();

private:
   GLuint m_iID = 0;
};
