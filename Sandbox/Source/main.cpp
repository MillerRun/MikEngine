#include <print>
#include <array>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace
{
   constexpr int k_iWindowWidth = 800;
   constexpr int k_iWindowHeight = 600;
   constexpr int k_iDimension = 3;

   constexpr const char *k_sVertexShaderSource = ""
      "#version 330 core\n"
      "layout (location = 0) in vec3 v3Pos;\n"
      "void main()\n"
      "{\n"
      "  gl_Position = vec4( v3Pos, 1.0 );\n"
      "}\n";

   constexpr const char *k_sFragmentShaderSource = ""
      "#version 330 core\n"
      "out vec4 FragColor;\n"
      "void main()\n"
      "{\n"
      "  FragColor = vec4( 0.8, 0.3, 0.02, 1.0 );\n"
      "}\n";
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
   glClearColor( 0.07f, 0.13f, 0.17f, 1.f );
   glClear( GL_COLOR_BUFFER_BIT );

   // vertex shader
   const GLuint vs = glCreateShader( GL_VERTEX_SHADER );
   glShaderSource( vs, 1, &k_sVertexShaderSource, nullptr ); // 1 screen for the shader
   glCompileShader( vs );

   // fragment shader
   const GLuint fs = glCreateShader( GL_FRAGMENT_SHADER );
   glShaderSource( fs, 1, &k_sFragmentShaderSource, nullptr );
   glCompileShader( fs );

   // create program
   const GLuint program = glCreateProgram();
   glAttachShader( program, vs ); // attach vertex shader
   glAttachShader( program, fs ); // attach fragment shader
   glLinkProgram( program ); // assemble program

   // delete shaders, because program contains them already
   glDeleteShader( vs );
   glDeleteShader( fs );

   const std::array<GLfloat, 18> aVerticies =
   {
      -0.5f,        -0.5f * std::sqrtf( 3 ) / 3,      0, // left
       0.5f,        -0.5f * std::sqrtf( 3 ) / 3,      0, // right
       0.f,          0.5f * std::sqrtf( 3 ) * 2 / 3,  0, // top
       -0.5f / 2,    0.5f * std::sqrtf( 3 ) / 6,      0, // left middle
       0.5f / 2,     0.5f * std::sqrtf( 3 ) / 6,      0, // right middle
       0.f,         -0.5f * std::sqrtf( 3 ) / 3,      0, // bottom middle
   };
   
   /*
   *          2
   *         / \
   *        /   \
   *       3-----5
   *      / \   / \
   *     /   \ /   \
   *    0-----4-----1
   */

   constexpr std::array<GLuint, 9> aIndicies =
   {
      0, 3, 5,
      3, 2, 4,
      5, 4, 1,
   };

   // create buffers for verticies
   constexpr int iObjectsAmount = 1;
   GLuint vao, vbo, ebo;
   glGenVertexArrays( iObjectsAmount, &vao );
   glGenBuffers( iObjectsAmount, &vbo );
   glGenBuffers( iObjectsAmount, &ebo );

   glBindVertexArray( vao );
   glBindBuffer( GL_ARRAY_BUFFER, vbo );
   glBufferData( GL_ARRAY_BUFFER, sizeof( aVerticies ), aVerticies.data(), GL_STATIC_DRAW );
   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo );
   glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( aIndicies ), aIndicies.data(), GL_STATIC_DRAW );
   glVertexAttribPointer( 0, k_iDimension, GL_FLOAT, GL_FALSE, 3 * sizeof( GLfloat ), (void *)0 );
   glEnableVertexAttribArray( 0 );
   glBindBuffer( GL_ARRAY_BUFFER, 0 );
   glBindVertexArray( 0 );
   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

   // swap colored chain buffer with current one
   glfwSwapBuffers( window );

   // main loop
   while( !glfwWindowShouldClose( window ) )
   {
      glClearColor( 0.07f, 0.13f, 0.17f, 1.f );
      glClear( GL_COLOR_BUFFER_BIT );
      glUseProgram( program );
      glBindVertexArray( vao );
      glDrawElements( GL_TRIANGLES, static_cast<int>( aIndicies.size() ), GL_UNSIGNED_INT, 0 );
      glfwSwapBuffers( window );
      glfwPollEvents();
   }

   // clear
   glDeleteVertexArrays( 1, &vao );
   glDeleteBuffers( 1, &vbo );
   glDeleteBuffers( 1, &ebo );
   glDeleteProgram( program );
   glfwDestroyWindow( window );
   glfwTerminate();

   return EXIT_SUCCESS;
}
