#include <print>
#include <array>

#include "Shader.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"
#include "Model.hpp"
#include "Utils/File.hpp"

#include <GLFW/glfw3.h>
#include <Utils/Math.hpp>

namespace
{
   constexpr int k_iWindowWidth = 800;
   constexpr int k_iWindowHeight = 800;
}

int main()
{
   MK::File::SetDefaultPath( "../Resources/" );

   // initialize GLFW
   glfwInit();
   glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
   glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
   glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

   // create and set window
   GLFWwindow *const window = glfwCreateWindow( k_iWindowWidth, k_iWindowHeight, "My window", nullptr, nullptr );
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
      //                   <---POSITION--->                  <----NORMALS---->                <---COLOR RGB---->               <-TEXTURE->     
      Vertex( glm::vec3( -1.0f,  0.0f,  1.0f ),   glm::vec3( 0.0f, 1.0f, 0.0f ),   glm::vec3( 1.0f, 1.0f, 1.0f ),   glm::vec2( 0.0f, 0.0f ) ),
      Vertex( glm::vec3( -1.0f,  0.0f, -1.0f ),   glm::vec3( 0.0f, 1.0f, 0.0f ),   glm::vec3( 1.0f, 1.0f, 1.0f ),   glm::vec2( 0.0f, 1.0f ) ),
      Vertex( glm::vec3(  1.0f,  0.0f, -1.0f ),   glm::vec3( 0.0f, 1.0f, 0.0f ),   glm::vec3( 1.0f, 1.0f, 1.0f ),   glm::vec2( 1.0f, 1.0f ) ),
      Vertex( glm::vec3(  1.0f,  0.0f,  1.0f ),   glm::vec3( 0.0f, 1.0f, 0.0f ),   glm::vec3( 1.0f, 1.0f, 1.0f ),   glm::vec2( 1.0f, 0.0f ) ),
   };

   constexpr std::array aIndicies =
   {
      0u, 1u, 2u,
      0u, 2u, 3u
   };

   constexpr std::array aLightVerticies =
   {
      Vertex( glm::vec3( -0.1f, -0.1f,  0.1f ) ),
      Vertex( glm::vec3( -0.1f, -0.1f, -0.1f ) ),
      Vertex( glm::vec3(  0.1f, -0.1f, -0.1f ) ),
      Vertex( glm::vec3(  0.1f, -0.1f,  0.1f ) ),
      Vertex( glm::vec3( -0.1f,  0.1f,  0.1f ) ),
      Vertex( glm::vec3( -0.1f,  0.1f, -0.1f ) ),
      Vertex( glm::vec3(  0.1f,  0.1f, -0.1f ) ),
      Vertex( glm::vec3(  0.1f,  0.1f,  0.1f ) ),
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

   Model mdl( "bunny/", "scene.gltf" );

   const std::array aTextures =
   {
      Texture( "planks.png", Texture::EType::DIFFUSE, 0, GL_RGBA, GL_UNSIGNED_BYTE ),
      Texture( "planksSpec.png", Texture::EType::SPECULAR, 1, GL_RED, GL_UNSIGNED_BYTE )
   };

   Shader shader{ "default" };
   Mesh floorMesh{
        std::vector<Vertex>( aVerticies.cbegin(), aVerticies.cend() )
      , std::vector<GLuint>( aIndicies.cbegin(), aIndicies.cend() )
      , std::vector<Texture>( aTextures.cbegin(), aTextures.cend() )
   };

   Shader lightShader{ "light" };
   Mesh lightMesh{
        std::vector<Vertex>( aLightVerticies.cbegin(), aLightVerticies.cend() )
      , std::vector<GLuint>( aLightIndicies.cbegin(), aLightIndicies.cend() )
      , std::vector<Texture>( aTextures.cbegin(), aTextures.cend() )
   };

   //VAO lightVAO;
   //lightVAO.Bind();

   glm::vec4 v4LightColor( 1.f, 1.0f, 1.0f, 0.f );
   glm::vec3 v4LightPosition( 0.5f, 0.5f, 0.5f );
   glm::mat4 m4LightModel( 1.f );
   m4LightModel = glm::translate( m4LightModel, v4LightPosition );

   glm::vec3 v4PyramidPosition( 0.f, 0.f, 0.f );
   glm::mat4 m4PyramidModel( 1.f );
   m4PyramidModel = glm::translate( m4PyramidModel, v4PyramidPosition );

   lightShader.Activate();
   GLCHECK( glUniformMatrix4fv( glGetUniformLocation( lightShader.GetID(), "u_m4Model" ), 1, GL_FALSE, glm::value_ptr( m4LightModel ) ) );
   GLCHECK( glUniform4f( glGetUniformLocation( lightShader.GetID(), "u_v4LightColor" ), v4LightColor.x, v4LightColor.y, v4LightColor.z, v4LightColor.w ) );

   shader.Activate();
   GLCHECK( glUniformMatrix4fv( glGetUniformLocation( shader.GetID(), "u_m4Model" ), 1, GL_FALSE, glm::value_ptr( m4PyramidModel ) ) );
   GLCHECK( glUniform4f( glGetUniformLocation( shader.GetID(), "u_v4LightColor" ), v4LightColor.x, v4LightColor.y, v4LightColor.z, v4LightColor.w ) );
   GLCHECK( glUniform3f( glGetUniformLocation( shader.GetID(), "u_v3LightPosition" ), v4LightPosition.x, v4LightPosition.y, v4LightPosition.z ) );

   GLCHECK( glEnable( GL_DEPTH_TEST ) );

   Camera hCamera( k_iWindowWidth, k_iWindowHeight, glm::vec3( 0.f, 1.f, 2.f ) );

   // main loop
   while( !glfwWindowShouldClose( window ) )
   {
      GLCHECK( glClearColor( 0.07f, 0.13f, 0.17f, 1.f ) );
      GLCHECK( glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) );

      hCamera.Inputs( window );
      hCamera.UpdateMatrix( 45.f, 0.1f, 100.f );

      floorMesh.Draw( shader, hCamera );
      lightMesh.Draw( lightShader, hCamera );

      glfwSwapBuffers( window );
      glfwPollEvents();
   }

   // clear
   shader.Deactivate();
   lightShader.Deactivate();

   glfwDestroyWindow( window );
   glfwTerminate();

   return EXIT_SUCCESS;
}
