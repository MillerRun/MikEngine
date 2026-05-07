#include "VAO.hpp"

VAO::VAO()
{
   glGenVertexArrays( 1, &m_iID );
}

void VAO::LinkAttribute( Buffer<EBufferType::VERTEX, GLfloat>& a_VBO
   , GLuint a_iLayout
   , GLuint a_iSize
   , GLenum a_eType
   , GLsizei a_iStride
   , void *a_pOffset )
{
   a_VBO.Bind();
   glVertexAttribPointer( a_iLayout, a_iSize, a_eType, GL_FALSE, a_iStride, a_pOffset );
   glEnableVertexAttribArray( a_iLayout );
   a_VBO.Unbind();
}

void VAO::Bind()
{
   glBindVertexArray( m_iID );
}

void VAO::Unbind()
{
   glBindVertexArray( 0 );
}

void VAO::Delete()
{
   glDeleteVertexArrays( 1, &m_iID );
}
