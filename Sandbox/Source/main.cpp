#include <print>
#include <array>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.hpp"
#include "Buffer.hpp"
#include "VAO.hpp"
#include "Texture.hpp"

namespace
{
   constexpr int k_iWindowWidth = 800;
   constexpr int k_iWindowHeight = 800;
   constexpr int k_iDimension = 3;
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
   if( !window )
   {
      std::println( "Failed to create window" );
      glfwTerminate();
      return EXIT_FAILURE;
   }
   glfwMakeContextCurrent( window );

   // make simple color BG with glad/opengl
   gladLoadGL();
   glViewport( 0, 0, k_iWindowWidth, k_iWindowHeight );

   constexpr std::array aVerticies =
   {
      // <--POSITION-->       <-COLOR RGB-->     <TEXTURE>
      -0.5f, -0.5f, 0.f,      1.f, 0.f,  0.f,    0.f,  0.f,    // 0
      -0.5f,  0.5f, 0.f,      0.f, 1.f,  0.f,    0.f,  1.f,    // 1
       0.5f,  0.5f, 0.f,      0.f, 0.f,  1.f,    1.f,  1.f,    // 2
       0.5f, -0.5f, 0.f,      1.f, 1.0f, 1.f,    1.f,  0.f,    // 3
   };

   constexpr std::array aIndicies =
   {
      0u, 2u, 1u,
      0u, 3u, 2u,
   };

   Shader shader{ "default" };

   VAO vao;
   vao.Bind();

   Buffer<EBufferType::VERTEX, GLfloat> vbo{ aVerticies.data(), sizeof( aVerticies ) };
   Buffer<EBufferType::INDEX, GLuint> ebo{ aIndicies.data(), sizeof( aIndicies ) };

   vao.LinkAttribute( vbo, 0, 3, GL_FLOAT, 8 * sizeof( GLfloat ), (void *)0 );
   vao.LinkAttribute( vbo, 1, 3, GL_FLOAT, 8 * sizeof( GLfloat ), (void *)( 3 * sizeof( GLfloat ) ) );
   vao.LinkAttribute( vbo, 2, 2, GL_FLOAT, 8 * sizeof( GLfloat ), (void *)( 6 * sizeof( GLfloat ) ) );
   vao.Unbind();
   vbo.Unbind();
   ebo.Unbind();

   GLuint scaleUniform = glGetUniformLocation( shader.GetID(), "u_fScale" );
    
   Texture tex{ "kotya.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE };
   tex.TextureUnit( shader, "u_Tex0", 0 );

   // main loop
   while( !glfwWindowShouldClose( window ) )
   {
      glClearColor( 0.07f, 0.13f, 0.17f, 1.f );
      glClear( GL_COLOR_BUFFER_BIT );
      shader.Activate();
      glUniform1f( scaleUniform, 1.5f );
      tex.Bind();
      vao.Bind();
      glDrawElements( GL_TRIANGLES, static_cast<int>( aIndicies.size() ), GL_UNSIGNED_INT, 0 );
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
