#include "VAO.hpp"

VAO::VAO()
{
   glGenVertexArrays( 1, &m_iID );
}

void VAO::LinkVBO( Buffer<EBufferType::VERTEX, GLfloat> a_VBO, GLuint a_iLayout )
{
   a_VBO.Bind();
   glVertexAttribPointer( a_iLayout, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( GLfloat ), (void *)0 );
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
