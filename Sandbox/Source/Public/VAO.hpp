#pragma once

#include <glad/glad.h>
#include "Buffer.hpp"

class VAO
{
public:
   VAO();

   void LinkAttribute( Buffer<EBufferType::VERTEX, GLfloat> a_VBO
      , GLuint a_iLayout
      , GLuint a_iSize
      , GLenum a_eType
      , GLsizei a_iStride
      , void* a_pOffset
   );
   void Bind();
   void Unbind();
   void Delete();

private:
   GLuint m_iID = 0;
};
