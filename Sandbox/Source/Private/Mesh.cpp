#include "Mesh.hpp"

#include <string>

#include "Shader.hpp"
#include "Camera.hpp"

Mesh::Mesh( std::vector<Vertex> a_aVertices, std::vector<GLuint> a_aIndices, std::vector<Texture> a_aTextures )
   : m_aVertices( std::move( a_aVertices ) )
   , m_aIndices( std::move( a_aIndices ) )
   , m_aTextures( std::move( a_aTextures ) )
{
   m_VAO.Bind();

   VertexBuffer vbo{ m_aVertices };
   IndexBuffer ebo{ m_aIndices };

   m_VAO.LinkAttribute( vbo, 0, 3, GL_FLOAT, sizeof( Vertex ), (void *)0 );
   m_VAO.LinkAttribute( vbo, 1, 3, GL_FLOAT, sizeof( Vertex ), (void *)( 3 * sizeof( GLfloat ) ) );
   m_VAO.LinkAttribute( vbo, 2, 3, GL_FLOAT, sizeof( Vertex ), (void *)( 6 * sizeof( GLfloat ) ) );
   m_VAO.LinkAttribute( vbo, 3, 2, GL_FLOAT, sizeof( Vertex ), (void *)( 9 * sizeof( GLfloat ) ) );

   m_VAO.Unbind();
   vbo.Unbind();
   ebo.Unbind();
}

void Mesh::Draw( Shader &a_Shader, Camera &a_Camera )
{
   a_Shader.Activate();
   m_VAO.Bind();

   std::uint32_t iDiffuseAmount = 0, iSpecularAmount = 0;
   for( auto i = 0uz; i < m_aTextures.size(); ++i )
   {
      const std::string sUniformName = std::invoke( [&]
      {
         switch( [[maybe_unused]] const Texture::EType eType = m_aTextures[i].GetType() )
         {
         case Texture::EType::DIFFUSE: return "u_TexDiffuse" + std::to_string( iDiffuseAmount++ );
         case Texture::EType::SPECULAR: return "u_TexSpecular" + std::to_string( iSpecularAmount++ );
         }
         throw std::logic_error( "Unhandled texture type" );
      } );
      m_aTextures[i].TextureUnit( a_Shader, sUniformName.c_str(), static_cast<GLuint>( i ) );
      m_aTextures[i].Bind();
   }
   GLCHECK( glUniform3f( glGetUniformLocation( a_Shader.GetID(), "u_v3CameraPosition" ), a_Camera.GetPosition().x, a_Camera.GetPosition().y, a_Camera.GetPosition().z ) );
   a_Camera.Matrix( a_Shader, "u_m4Camera" );
   GLCHECK( glDrawElements( GL_TRIANGLES, static_cast<GLsizei>( m_aIndices.size() ), GL_UNSIGNED_INT, 0 ) );
}
