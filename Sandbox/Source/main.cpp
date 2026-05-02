#include <print>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace
{
   constexpr int k_iWindowWidth = 800;
   constexpr int k_iWindowHeight = 600;
}

int main()
{
   // initialize GLFW
   glfwInit();
   glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
   glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
   glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

   // create and set window
   GLFWwindow *window = glfwCreateWindow( k_iWindowWidth, k_iWindowHeight, "My window", nullptr, nullptr );
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
   glClearColor( 0.07f, 0.13f, 0.17f, 1.f );
   glClear( GL_COLOR_BUFFER_BIT );

   // swap colored chain buffer with current one
   glfwSwapBuffers( window );

   // main loop
   while( !glfwWindowShouldClose( window ) )
   {
      glfwPollEvents();
   }

   // clear
   glfwDestroyWindow( window );
   glfwTerminate();

   return EXIT_SUCCESS;
}
