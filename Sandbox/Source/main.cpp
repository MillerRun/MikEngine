#include <print>
#include <array>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Public/Shader.hpp"
#include "Public/Buffer.hpp"
#include "Public/VAO.hpp"

namespace
{
   constexpr int k_iWindowWidth = 800;
   constexpr int k_iWindowHeight = 600;
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

   /*
   *          2
   *         / \
   *        /   \
   *       3-----5
   *      / \   / \
   *     /   \ /   \
   *    0-----4-----1
   */

   const std::array aVerticies =
   {
      -0.5f,        -0.5f * std::sqrtf( 3 ) / 3.f,          0.f, // left
       0.5f,        -0.5f * std::sqrtf( 3 ) / 3.f,          0.f, // right
       0.f,          0.5f * std::sqrtf( 3 ) * 2.f / 3.f,    0.f, // top
      -0.5f / 2.f,   0.5f * std::sqrtf( 3 ) / 6.f,          0.f, // left middle
       0.5f / 2.f,   0.5f * std::sqrtf( 3 ) / 6.f,          0.f, // right middle
       0.f,         -0.5f * std::sqrtf( 3 ) / 3.f,          0.f, // bottom middle
   };

   constexpr std::array aIndicies =
   {
      0u, 3u, 5u,
      3u, 2u, 4u,
      5u, 4u, 1u,
   };

   Shader shader{ "default" };

   VAO vao;
   vao.Bind();

   Buffer<EBufferType::VERTEX, GLfloat> vbo{ aVerticies.data(), sizeof( aVerticies ) };
   Buffer<EBufferType::INDEX, GLuint> ebo{ aIndicies.data(), sizeof( aIndicies ) };

   vao.LinkVBO( vbo, 0 );
   vao.Unbind();
   vbo.Unbind();
   ebo.Unbind();

   // main loop
   while( !glfwWindowShouldClose( window ) )
   {
      glClearColor( 0.07f, 0.13f, 0.17f, 1.f );
      glClear( GL_COLOR_BUFFER_BIT );
      shader.Activate();
      vao.Bind();
      glDrawElements( GL_TRIANGLES, static_cast<int>( aIndicies.size() ), GL_UNSIGNED_INT, 0 );
      glfwSwapBuffers( window );
      glfwPollEvents();
   }

   // clear
   vao.Delete();
   vbo.Delete();
   ebo.Delete();
   shader.Deactivate();

   glfwDestroyWindow( window );
   glfwTerminate();

   return EXIT_SUCCESS;
}
