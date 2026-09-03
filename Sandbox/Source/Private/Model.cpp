#include "Model.hpp"

#include "Utils/Assert.hpp"
#include "Utils/File.hpp"

#include <json/json.hpp>
#include <optional>

using json = nlohmann::json;

namespace
{
   [[nodiscard]] void ReadNode( const std::int32_t a_iNodeIndex
      , const json &a_jNodesRoot
      , std::vector<SceneObjectInfo> &a_aOutData
      , const std::int32_t a_iParentIndex    = -1
      , const glm::mat4 &a_m4ParentTransfrom = glm::mat4( 1.f ) )
   {
      if( not a_jNodesRoot.is_array() or a_jNodesRoot.empty() )
      {
         MKASSERT( false, R"(Empty "nodes" json node)" );
         return;
      }
      
      if( a_iNodeIndex >= a_jNodesRoot.size() )
      {
         MKASSERT( false, "Wrong json node index" );
         return;
      }

      try
      {
         std::ignore = a_jNodesRoot.at( a_iNodeIndex );
      }
      catch( [[maybe_unused]] const std::exception &ex )
      {
         MKASSERT( false, "{}", ex.what() );
         return;
      }

      const json &jNode = a_jNodesRoot.at( a_iNodeIndex );

      // first, try to get an optional transform matrix
      const std::optional<glm::mat4> m4Transform = std::invoke( [&jNode]
      {
         std::optional<glm::mat4> m4RetMatrix = std::nullopt;
         if( const auto itNodeMatrix = jNode.find( "matrix" ); itNodeMatrix != jNode.cend() and itNodeMatrix->size() == 16 )
         {
            float aMatrixRawValues[16];
            for( auto i = 0uz; i < itNodeMatrix->size(); ++i )
               aMatrixRawValues[i] = itNodeMatrix->at( i );
            m4RetMatrix.emplace( glm::make_mat4( aMatrixRawValues ) );
         }
         return m4RetMatrix;
      } );

      // decompose matrix into TRS values and store them as an alternative
      struct AlternativeTransform
      {
         glm::vec3 v3Translation;
         glm::vec3 v3Scale = glm::vec3( 1.f, 1.f, 1.f );
         glm::quat qRotation;
         glm::vec3 v3Skew;
         glm::vec4 v4Perspective;
      };
      const AlternativeTransform alternative = std::invoke( [&m4Transform]
      {
         AlternativeTransform retValue{};
         if( m4Transform.has_value() )
         {
            const bool bSuccess = glm::decompose( m4Transform.value()
               , retValue.v3Scale
               , retValue.qRotation
               , retValue.v3Translation
               , retValue.v3Skew
               , retValue.v4Perspective
            );
            if( not bSuccess )
               return AlternativeTransform{};
         }
         return retValue;
      } );

      // then, get all TRS values and compare them with alternative
      SceneObjectInfo info;
      info.iParentIndex = a_iParentIndex;
      info.v3Translation = std::invoke( [&jNode, &alternative]
      {
         if( not jNode.contains( "translation" ) )
            return alternative.v3Translation;

         const json &jTranslation = jNode["translation"];
         float aRawTranslation[3];
         for( auto i = 0uz; i < jTranslation.size(); ++i )
            aRawTranslation[i] = jTranslation[i];
         return glm::make_vec3( aRawTranslation );
      } );
      info.qRotation = std::invoke( [&jNode, &alternative]
      {
         if( not jNode.contains( "rotation" ) )
            return alternative.qRotation;

         const json &jRotation = jNode["rotation"];
         const float aRawRotation[4] =
         {
            jRotation[3],
            jRotation[0],
            jRotation[1],
            jRotation[2],
         };
         return glm::make_quat( aRawRotation );
      } );
      info.v3Scale = std::invoke( [&jNode, &alternative]
      {
         if( not jNode.contains( "scale" ) )
            return alternative.v3Scale;

         const json &jScale = jNode["scale"];
         float aRawScale[3];
         for( auto i = 0uz; i < jScale.size(); ++i )
            aRawScale[i] = jScale[i];
         return glm::make_vec3( aRawScale );
      } );
      
      const glm::mat4 m4NodeLocalTransform = glm::translate( glm::mat4( 1.f ), info.v3Translation )
         * glm::mat4_cast( info.qRotation )
         * glm::scale( glm::mat4( 1.f ), info.v3Scale );

      info.m4WorldTransform = a_m4ParentTransfrom * m4NodeLocalTransform;

      if( const auto& itChildren = jNode.find( "children" ); itChildren != jNode.cend() and itChildren->is_array() and not itChildren->empty() )
      {
         for( const std::int32_t iChildNodeIndex : *itChildren )
            ReadNode( iChildNodeIndex, a_jNodesRoot, a_aOutData, a_iNodeIndex, info.m4WorldTransform );
      }

      if( const auto& itMesh = jNode.find( "mesh" ); itMesh != jNode.cend() )
      {
         info.iMeshIndex = *itMesh;
      }

      a_aOutData.push_back( std::move( info ) );
   }

   [[nodiscard]] inline
   std::vector<std::byte> ReadBinData( const json &a_jDoc, const std::string_view a_sModelPath )
   {
      if( not a_jDoc.contains( "buffers" ) or a_jDoc["buffers"].empty() or not a_jDoc["buffers"].is_array() )
      {
         MKASSERT( false, "Failed to read buffers" );
         return {};
      }
      const auto &jBuffersBegin = a_jDoc["buffers"][0];

      if( jBuffersBegin.is_null() or not jBuffersBegin.contains( "uri" ) or not jBuffersBegin["uri"].is_string() )
      {
         MKASSERT( false, "Failed to read uri string" );
         return {};
      }
      
      const std::string_view sURI = jBuffersBegin["uri"];
      if( const auto result = MK::File::GetFileContent( a_sModelPath, sURI ); result )
      {
         const auto sContent = result.value();
         return std::vector<std::byte>(
            reinterpret_cast<const std::byte *>( sContent.data() ),
            reinterpret_cast<const std::byte *>( sContent.data() + sContent.size() )
         );
      }
      else
      {
         MKASSERT( false, "Failed to read model file {} with error {}", sURI, result.error() );
         return {};
      }
   }

   [[nodiscard]] inline
   std::vector<float> GetAccessorFloatArray( const json &a_jRoot, const std::size_t a_iIndex, const std::vector<std::byte>& a_aBytes )
   {
      if( not a_jRoot.contains( "accessors" ) or not a_jRoot["accessors"].is_array() or a_jRoot["accessors"].size() <= a_iIndex )
      {
         MKASSERT( false, "Mesh does not contain valid \"accessors\" array" );
         return {};
      }
      const json &jAccessors = a_jRoot["accessors"][a_iIndex];

      if( jAccessors.is_null() or not jAccessors.contains( "count" ) or not jAccessors.contains( "type" ) )
      {
         MKASSERT( false, "Invalid accessor" );
         return {};
      }

      // get accessor properties
      const std::size_t iBufferViewIndex = jAccessors.value( "bufferView", 1 );
      const std::size_t iCount = jAccessors["count"];
      const std::size_t iByteOffset = jAccessors.value( "byteOffset", 0 );

      const std::string_view sType = jAccessors["type"];
      const std::size_t iNumPerVert = std::invoke( [&sType]
      {
         if( sType == "SCALAR" ) return 1;
         if( sType == "VEC2" ) return 2;
         if( sType == "VEC3" ) return 3;
         if( sType == "VEC4" ) return 4;
         return 0;
      } );
      if( iNumPerVert == 0 )
      {
         MKASSERT( false, "Verticies type is invalid" );
         return {};
      }

      const std::size_t iBufferViewOffset = a_jRoot["bufferViews"][iBufferViewIndex]["byteOffset"];

      std::vector<float> aRetVec;

      static_assert( sizeof( float ) == 4 );
      for( std::size_t iBegin = iBufferViewOffset + iByteOffset, i = iBegin
         ; i < iBegin + iCount * sizeof(float) * iNumPerVert
         ; i += 4 )
      {
         const std::byte aBytes[] = { a_aBytes[i], a_aBytes[i + 1], a_aBytes[i + 2], a_aBytes[i + 3] };
         float fFinalValue{};
         std::memcpy( &fFinalValue, aBytes, sizeof( float ) );
         aRetVec.push_back( fFinalValue );
      }
      return aRetVec;
   }
   
   template<std::size_t N, typename vecN = glm::vec<N, float, glm::defaultp>>
   [[nodiscard]] inline
   auto GroupFloats( const std::vector<float> a_aFloats ) -> std::vector<vecN>
   {
      std::vector<vecN> aRetVec;
      for( auto i = 0uz; i < a_aFloats.size(); i += N )
      {
         aRetVec.emplace_back();
         for( int j = 0; j < N; ++j )
            aRetVec.back()[j] = a_aFloats[i + j];
      }
      return aRetVec;
   }

   [[nodiscard]] inline
   std::optional<Mesh> ReadMesh( const std::size_t a_iMeshIndex, const json& a_jRoot, const std::vector<std::byte> &a_aBytes )
   {
      struct Indices
      {
         std::size_t position{};
         std::size_t normal{};
         std::size_t texture{};
         std::size_t total{};
      };
      Indices indices;

      // read mesh primitive
      {
         if( not a_jRoot.contains( "meshes" ) or not a_jRoot["meshes"].is_array() or a_jRoot["meshes"].size() <= a_iMeshIndex )
         {
            MKASSERT( false, "Model file does not contain valid \"meshes\" node for index {}", a_iMeshIndex );
            return {};
         }
         const json &jMesh = a_jRoot["meshes"][a_iMeshIndex];

         if( not jMesh.contains( "primitives" ) or not jMesh["primitives"].is_array() or jMesh["primitives"].empty() )
         {
            MKASSERT( false, "Mesh data does not contain valid \"primitives\" array" );
            return {};
         }
         const json &jPrimitives = jMesh["primitives"].front();

         if( not jPrimitives.contains( "attributes" )
            or not jPrimitives["attributes"].contains( "POSITION" ) or not jPrimitives["attributes"]["POSITION"].is_number()
            or not jPrimitives["attributes"].contains( "NORMAL" ) or not jPrimitives["attributes"]["NORMAL"].is_number()
            or not jPrimitives["attributes"].contains( "TEXCOORD_0" ) or not jPrimitives["attributes"]["TEXCOORD_0"].is_number()
            or not jPrimitives.contains( "indices" ) or not jPrimitives["indices"].is_number() )
         {
            MKASSERT( false, "Mesh contains invalid primitive" );
            return {};
         }
         indices.position = jPrimitives["attributes"]["POSITION"];
         indices.normal = jPrimitives["attributes"]["NORMAL"];
         indices.texture = jPrimitives["attributes"]["TEXCOORD_0"];
         indices.total = jPrimitives["indices"];
      }

      // read mesh accessors
      auto aPositions = GroupFloats<3>( GetAccessorFloatArray( a_jRoot, indices.position, a_aBytes ) );
      auto aNormals = GroupFloats<3>( GetAccessorFloatArray( a_jRoot, indices.normal, a_aBytes ) );
      auto aTextureUVs = GroupFloats<2>( GetAccessorFloatArray( a_jRoot, indices.texture, a_aBytes ) );
      
      std::vector<Vertex> aVertices = std::invoke( [&]
      {
         std::vector<Vertex> aRetVec;
         aRetVec.reserve( aPositions.size() );
         for( auto i = 0uz; i < aPositions.size(); ++i )
         {
            aRetVec.emplace_back( Vertex {
               .v3Position = aPositions[i],
               .v3Normal = aNormals[i],
               .v3Color = {},
               .v2Texture = aTextureUVs[i]
            } );
         }
         return aRetVec;
      } );

      std::vector<GLuint> aIndices = std::invoke( [&]
      {
         std::vector<GLuint> aRetVec;
         
         return aRetVec;
      } );

      std::vector<Texture> aTextures = std::invoke( [&]
      {
         std::vector<Texture> aRetVec;
         
         return aRetVec;
      } );

      return {};
   }
}

Model::Model( const std::string_view a_sFilePath, const std::string_view a_sFileName )
{
   const std::string sRawFileContent = std::invoke( [a_sFilePath, a_sFileName]
   {
      if( const auto result = MK::File::GetFileContent( a_sFilePath, a_sFileName ); result )
      {
         return result.value();
      }
      else
      {
         MKASSERT( false, "Failed to read model file {} with error {}", a_sFileName, result.error() );
         return std::string{};
      }
   } );
   auto jRoot = json::parse( sRawFileContent );

   // read nodes recursively
   if( jRoot.is_null() or jRoot.empty() )
   {
      MKASSERT( false, "Empty json value obtained while parsing 3D model" );
      return;
   }
   if( not jRoot.contains( "nodes" ) or not jRoot["nodes"].is_array() or jRoot["nodes"].empty() )
   {
      MKASSERT( false, "Invalid json value obtained while parsing 3D model" );
      return;
   }
   const json &jNodesRoot = jRoot["nodes"];
   ReadNode( 0, jNodesRoot, m_aSceneObjectsInfo );

   // read meshes for each scene object
   const auto aBinData = ReadBinData( jRoot, a_sFilePath );

   for( const SceneObjectInfo &info : m_aSceneObjectsInfo )
   {
      if( const std::int32_t iMeshReferenceIndex = info.iMeshIndex; iMeshReferenceIndex >= 0 )
      {
         if( auto mesh = ReadMesh( iMeshReferenceIndex, jRoot, aBinData ); mesh.has_value() )
         {
            
         }
         // create mesh

         // write id down in m_aMeshes

         // give its array index to info.iMeshIndex
      }
   }
}

void Model::Draw( [[maybe_unused]] Shader &a_Shader, [[maybe_unused]] Camera &a_Camera )
{
   
}
