#include "VAO.hpp"

#include "Common.hpp"

VAO::VAO()
{
   GLCHECK( glGenVertexArrays( 1, &m_iID ) );
}

void VAO::LinkAttribute( VertexBuffer& a_VBO
   , GLuint a_iLayout
   , GLuint a_iSize
   , GLenum a_eType
   , GLsizei a_iStride
   , void *a_pOffset )
{
   a_VBO.Bind();
   GLCHECK( glVertexAttribPointer( a_iLayout, a_iSize, a_eType, GL_FALSE, a_iStride, a_pOffset ) );
   GLCHECK( glEnableVertexAttribArray( a_iLayout ) );
   a_VBO.Unbind();
}

void VAO::Bind()
{
   GLCHECK( glBindVertexArray( m_iID ) );
}

void VAO::Unbind()
{
   GLCHECK( glBindVertexArray( 0 ) );
}

void VAO::Delete()
{
   GLCHECK( glDeleteVertexArrays( 1, &m_iID ) );
}
