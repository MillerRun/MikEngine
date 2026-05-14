#include <print>
#include <array>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Common.hpp"
#include "Shader.hpp"
#include "Buffer.hpp"
#include "VAO.hpp"
#include "Texture.hpp"
#include "Camera.hpp"

namespace
{
   constexpr int k_iWindowWidth = 800;
   constexpr int k_iWindowHeight = 800;
}

int main()
{
   // initialize GLFW
   glfwInit();
   glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
   glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
   glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

   // create and set window
   GLFWwindow * const window = glfwCreateWindow( k_iWindowWidth, k_iWindowHeight, "My window", nullptr, nullptr );
   if( not window )
   {
      std::println( "Failed to create window" );
      glfwTerminate();
      return EXIT_FAILURE;
   }
   glfwMakeContextCurrent( window );

   // make simple color BG with glad/opengl
   gladLoadGL();
   GLCHECK( glViewport( 0, 0, k_iWindowWidth, k_iWindowHeight ) );

   constexpr std::array aVerticies =
   {
      // <---POSITION--->     <-----COLOR RGB----->     <-TEXTURE->     <----NORMALS----->
      -0.5f,  0.0f,  0.5f,    0.83f,  0.70f,  0.44f,    0.0f,  0.0f,    0.0f, -1.0f,  0.0f, // Bottom side
      -0.5f,  0.0f, -0.5f,    0.83f,  0.70f,  0.44f,    0.0f,  5.0f,    0.0f, -1.0f,  0.0f, // Bottom side
       0.5f,  0.0f, -0.5f,    0.83f,  0.70f,  0.44f,    5.0f,  5.0f,    0.0f, -1.0f,  0.0f, // Bottom side
       0.5f,  0.0f,  0.5f,    0.83f,  0.70f,  0.44f,    5.0f,  0.0f,    0.0f, -1.0f,  0.0f, // Bottom side
                                                                                      
      -0.5f,  0.0f,  0.5f,    0.83f,  0.70f,  0.44f,    0.0f,  0.0f,   -0.8f,  0.5f,  0.0f, // Left Side
      -0.5f,  0.0f, -0.5f,    0.83f,  0.70f,  0.44f,    5.0f,  0.0f,   -0.8f,  0.5f,  0.0f, // Left Side
       0.0f,  0.8f,  0.0f,    0.92f,  0.86f,  0.76f,    2.5f,  5.0f,   -0.8f,  0.5f,  0.0f, // Left Side
                                                                                      
      -0.5f,  0.0f, -0.5f,    0.83f,  0.70f,  0.44f,    5.0f,  0.0f,    0.0f,  0.5f, -0.8f, // Non-facing side
       0.5f,  0.0f, -0.5f,    0.83f,  0.70f,  0.44f,    0.0f,  0.0f,    0.0f,  0.5f, -0.8f, // Non-facing side
       0.0f,  0.8f,  0.0f,    0.92f,  0.86f,  0.76f,    2.5f,  5.0f,    0.0f,  0.5f, -0.8f, // Non-facing side
                                                                                      
       0.5f,  0.0f, -0.5f,    0.83f,  0.70f,  0.44f,    0.0f,  0.0f,    0.8f,  0.5f,  0.0f, // Right side
       0.5f,  0.0f,  0.5f,    0.83f,  0.70f,  0.44f,    5.0f,  0.0f,    0.8f,  0.5f,  0.0f, // Right side
       0.0f,  0.8f,  0.0f,    0.92f,  0.86f,  0.76f,    2.5f,  5.0f,    0.8f,  0.5f,  0.0f, // Right side
                                                                                      
       0.5f,  0.0f,  0.5f,    0.83f,  0.70f,  0.44f,    5.0f,  0.0f,    0.0f,  0.5f,  0.8f, // Facing side
      -0.5f,  0.0f,  0.5f,    0.83f,  0.70f,  0.44f,    0.0f,  0.0f,    0.0f,  0.5f,  0.8f, // Facing side
       0.0f,  0.8f,  0.0f,    0.92f,  0.86f,  0.76f,    2.5f,  5.0f,    0.0f,  0.5f,  0.8f  // Facing side
   };

   constexpr std::array aIndicies =
   {
       0u,  1u,  2u, // Bottom side
       0u,  2u,  3u, // Bottom side
       4u,  6u,  5u, // Left side
       7u,  9u,  8u, // Non-facing side
      10u, 12u, 11u, // Right side
      13u, 15u, 14u  // Facing side
   };

   constexpr std::array aLightVerticies =
   {
      -0.1f, -0.1f,  0.1f,
      -0.1f, -0.1f, -0.1f,
       0.1f, -0.1f, -0.1f,
       0.1f, -0.1f,  0.1f,
      -0.1f,  0.1f,  0.1f,
      -0.1f,  0.1f, -0.1f,
       0.1f,  0.1f, -0.1f,
       0.1f,  0.1f,  0.1f,
   };

   constexpr std::array aLightIndicies =
   {
      0u, 1u, 2u,
      0u, 2u, 3u,
      0u, 4u, 7u,
      0u, 7u, 3u,
      3u, 7u, 6u,
      3u, 6u, 2u,
      2u, 6u, 5u,
      2u, 5u, 1u,
      1u, 5u, 4u,
      1u, 4u, 0u,
      4u, 5u, 6u,
      4u, 6u, 7u,
   };

   Shader shader{ "default" };

   VAO vao;
   vao.Bind();

   VertexBuffer vbo{ aVerticies.data(), sizeof( aVerticies ) };
   IndexBuffer ebo{ aIndicies.data(), sizeof( aIndicies ) };

   vao.LinkAttribute( vbo, 0, 3, GL_FLOAT, 11 * sizeof( GLfloat ), (void *)0 );
   vao.LinkAttribute( vbo, 1, 3, GL_FLOAT, 11 * sizeof( GLfloat ), (void *)( 3 * sizeof( GLfloat ) ) );
   vao.LinkAttribute( vbo, 2, 2, GL_FLOAT, 11 * sizeof( GLfloat ), (void *)( 6 * sizeof( GLfloat ) ) );
   vao.LinkAttribute( vbo, 3, 3, GL_FLOAT, 11 * sizeof( GLfloat ), (void *)( 8 * sizeof( GLfloat ) ) );
   vao.Unbind();
   vbo.Unbind();
   ebo.Unbind();

   Shader lightShader( "light" );
   VAO lightVAO;
   lightVAO.Bind();

   VertexBuffer lightVBO( aLightVerticies.data(), sizeof( aLightVerticies ) );
   IndexBuffer lightEBO{ aLightIndicies.data(), sizeof( aLightIndicies ) };

   lightVAO.LinkAttribute( lightVBO, 0, 3, GL_FLOAT, 3 * sizeof( aLightVerticies[0] ), (void *)0 );
   lightVAO.Unbind();
   lightVBO.Unbind();
   lightEBO.Unbind();

   glm::vec4 v4LightColor{ 1.f, 0.5f, 0.6f, 0.f };
   glm::vec3 v4LightPosition{ 0.5f, 0.5f, 0.5f };
   glm::mat4 m4LightModel{ 1.f };
   m4LightModel = glm::translate( m4LightModel, v4LightPosition );

   glm::vec3 v4PyramidPosition{ 0.f, 0.f, 0.f };
   glm::mat4 m4PyramidModel{ 1.f };
   m4PyramidModel = glm::translate( m4PyramidModel, v4PyramidPosition );

   lightShader.Activate();
   GLCHECK( glUniformMatrix4fv( glGetUniformLocation( lightShader.GetID(), "u_m4Model" ), 1, GL_FALSE, glm::value_ptr( m4LightModel ) ) );
   GLCHECK( glUniform4f( glGetUniformLocation( lightShader.GetID(), "u_v4LightColor" ), v4LightColor.x, v4LightColor.y, v4LightColor.z, v4LightColor.w ) );

   shader.Activate();
   GLCHECK( glUniformMatrix4fv( glGetUniformLocation( shader.GetID(), "u_m4Model" ), 1, GL_FALSE, glm::value_ptr( m4PyramidModel ) ) );
   GLCHECK( glUniform4f( glGetUniformLocation( shader.GetID(), "u_v4LightColor" ), v4LightColor.x, v4LightColor.y, v4LightColor.z, v4LightColor.w ) );
   GLCHECK( glUniform3f( glGetUniformLocation( shader.GetID(), "u_v3LightPosition" ), v4LightPosition.x, v4LightPosition.y, v4LightPosition.z ) );

   Texture tex{ "brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE };
   tex.TextureUnit( shader, "u_Tex0", 0 );

   GLCHECK( glEnable( GL_DEPTH_TEST ) );

   Camera hCamera( k_iWindowWidth, k_iWindowHeight, glm::vec3( 0.f, 0.f, 2.f ) );

   // main loop
   while( !glfwWindowShouldClose( window ) )
   {
      GLCHECK( glClearColor( 0.07f, 0.13f, 0.17f, 1.f ) );
      GLCHECK( glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) );
      //shader.Activate();

      hCamera.Inputs( window );
      hCamera.UpdateMatrix( 45.f, 0.001f, 100.f );
      shader.Activate();
      GLCHECK( glUniform3f( glGetUniformLocation( shader.GetID(), "u_v3CameraPosition" ), hCamera.GetPosition().x, hCamera.GetPosition().y, hCamera.GetPosition().z ) );
      hCamera.Matrix( shader, "u_m4Camera" );

      tex.Bind();
      vao.Bind();
      GLCHECK( glDrawElements( GL_TRIANGLES, static_cast<int>( aIndicies.size() ), GL_UNSIGNED_INT, 0 ) );

      lightShader.Activate();
      hCamera.Matrix( lightShader, "u_m4Camera" );
      lightVAO.Bind();
      GLCHECK( glDrawElements( GL_TRIANGLES, static_cast<int>( aLightIndicies.size() ), GL_UNSIGNED_INT, 0 ) );

      glfwSwapBuffers( window );
      glfwPollEvents();
   }

   // clear
   vao.Delete();
   vbo.Delete();
   ebo.Delete();
   tex.Delete();
   shader.Deactivate();

   glfwDestroyWindow( window );
   glfwTerminate();

   return EXIT_SUCCESS;
}
