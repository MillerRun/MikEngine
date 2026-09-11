#include "ParserGLTF.hpp"

#include "CommonTypes.hpp"
#include <Utils/Assert.hpp>
#include <Utils/File.hpp>
#include <json/json.hpp>

using json = nlohmann::json;

//namespace
//{
//   using Buffer = std::vector<std::byte>;
//
//   struct DocumentGLFT
//   {
//      std::string sBaseDirectory;
//      std::vector<Buffer> aBuffers;
//      json jSource{};
//   };
//
//   [[nodiscard]] std::optional<Buffer> Decode( const std::string &a_sEncoded )
//   {
//      static const std::unordered_map<char, int> s_mDecodeTable = std::invoke( []()
//      {
//         const std::string sAlphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
//         std::unordered_map<char, int> mRetTable;
//
//         for( auto i = 0uz; i < sAlphabet.size(); ++i )
//         {
//            mRetTable[sAlphabet[i]] = static_cast<int>( i );
//         }
//         return mRetTable;
//      } );
//
//      Buffer buffer;
//      buffer.reserve( a_sEncoded.size() * 3 / 4 );
//
//      int iAccumulator{}, iBitsCollected{};
//
//      for( char ch : a_sEncoded )
//      {
//         if( ch == '=' )
//            break;
//
//         const auto it = s_mDecodeTable.find( ch );
//         if( it == s_mDecodeTable.cend() )
//            continue;
//
//         iAccumulator = ( iAccumulator << 6 ) | it->second;
//         iBitsCollected += 6;
//
//         if( iBitsCollected >= 8 )
//         {
//            iBitsCollected -= 8;
//            buffer.push_back( static_cast<std::byte>( ( iAccumulator >> iBitsCollected ) & 0xFF ) );
//         }
//      }
//
//      return buffer;
//   }
//
//   [[nodiscard]] std::string RecolveURI( const std::string &a_sBaseDirectory, const std::string &a_sURI )
//   {
//      if( a_sURI.rfind( "data:", 0 ) == 0 )
//         return {};
//      const std::filesystem::path path = MK::File::GetPath( std::filesystem::path{ a_sBaseDirectory } / a_sURI );
//      return path.lexically_normal().string();
//   }
//
//   [[nodiscard]] std::optional<Buffer> LoadBuffer(
//      const json &a_jBuffersArray,
//      const std::string &a_sBaseDirectory,
//      std::size_t a_iBufferIndex,
//      const Buffer *a_pEmbeddedBin )
//   {
//      if( a_iBufferIndex >= a_jBuffersArray.size() )
//         return std::nullopt;
//
//      const json buffer = a_jBuffersArray[a_iBufferIndex];
//      if( not buffer.contains( "uri" ) )
//      {
//         if( not a_pEmbeddedBin )
//            return std::nullopt;
//         return std::optional<Buffer>{ *a_pEmbeddedBin };
//      }
//
//      const auto sURI = buffer["uri"].get<std::string>();
//      if( sURI.rfind( "data:", 0 ) == 0 )
//      {
//         const std::size_t iCommaPosition = sURI.find( ',' );
//         if( iCommaPosition == std::string::npos )
//            return std::nullopt;
//         return Decode( sURI.substr( iCommaPosition + 1 ) );
//      }
//
//      std::expected result = MK::File::GetBinaryFileContent( RecolveURI( a_sBaseDirectory, sURI ) );
//      if( result )
//         return std::optional<Buffer>{ std::move( *result ) };
//      return std::nullopt;
//   }
//
//   [[nodiscard]] std::optional<DocumentGLFT> LoadDocument( const std::string_view a_sPath )
//   {
//      DocumentGLFT doc;
//      doc.sBaseDirectory = MK::File::GetParentDirectory( a_sPath );
//
//      if( const std::string sExtension = MK::File::GetFileExtension( a_sPath ); sExtension == "glb" )
//      {
//
//      }
//
//      const std::expected sFileContent = MK::File::GetFileContent( a_sPath );
//      if( !sFileContent )
//         return std::nullopt;
//
//      doc.jSource = json::parse( *sFileContent, nullptr, false );
//      if( doc.jSource.is_discarded() )
//         return std::nullopt;
//
//      if( doc.jSource.contains( "buffers" ) )
//      {
//         const json aBuffers = doc.jSource["buffers"];
//         const std::size_t n = aBuffers.size();
//         doc.aBuffers.reserve( n );
//
//         for( auto i = 0uz; i < n; ++i )
//         {
//            const auto buffer = LoadBuffer( aBuffers, doc.sBaseDirectory, i, nullptr );
//            if( !buffer )
//               return std::nullopt;
//            doc.aBuffers.push_back( std::move( *buffer ) );
//         }
//      }
//
//      return doc;
//   }
//
//   [[nodiscard]] std::optional<MK::Model> ParseDocument( const DocumentGLFT& a_Doc )
//   {
//      return std::nullopt;
//   }
//}

namespace
{
   enum class EComponentType : int
   {
      Byte          = 5120,
      UnsignedByte  = 5121,
      Short         = 5122,
      UnsignedShort = 5123,
      UnsignedInt   = 5125,
      Float         = 5126,
   };

   using Buffer = std::vector<std::byte>;

   struct GltfDocument
   {
      std::string sSourceDirectory;
      std::vector<Buffer> aBuffers;
      json jSource{};
   };

   [[nodiscard]] auto ReadJson( const std::string_view a_sPath ) -> std::expected<json, std::string>
   {
      json jRetValue;

      // TODO: glb extension

      const std::expected sFileContent = MK::File::GetFileContent( a_sPath );
      if( !sFileContent )
         return std::unexpected{ "empty file content" };

      jRetValue = json::parse( *sFileContent, nullptr, false );
      if( jRetValue.is_discarded() )
         return std::unexpected{ "failed to parse json" };
      
      return jRetValue; // TODO: check nrvo
   }

   [[nodiscard]] auto LoadDocument( const std::string_view a_sPath ) -> std::expected<GltfDocument, std::string>
   {
      GltfDocument doc;

      if( std::expected data = ReadJson( a_sPath ); data.has_value() )
      {
         doc.jSource = std::move( *data );
      }
      else
      {
         return std::unexpected{ data.error() };
      }

      doc.sSourceDirectory = MK::File::GetFileDirectory( a_sPath );

      // doc.buffers

      return doc; // TODO: check nrvo
   }

   [[nodiscard]] auto ResolveUri( const std::string_view a_sBaseDirectory, const std::string_view a_sUri ) -> std::string
   {
      if( a_sUri.rfind( "data:", 0 ) == 0 )
         return {};

      std::string sResult = MK::File::GetFileLocation( a_sBaseDirectory, a_sUri );
      return sResult;
   }

   [[nodiscard]] auto GetTexturePath( const GltfDocument &a_Doc, const std::size_t a_iTextureIndex ) -> std::string
   {
      const auto itTexturesNode = a_Doc.jSource.find( "textures" );
      if( itTexturesNode == a_Doc.jSource.cend() or not itTexturesNode->is_array() or a_iTextureIndex >= itTexturesNode->size() )
         return {};

      const json &jTexture = itTexturesNode->at( a_iTextureIndex );
      const auto itSourceNode = jTexture.find( "source" );
      if( itSourceNode == jTexture.cend() or not itSourceNode->is_number_unsigned() )
         return {};

      const std::size_t iImageIndex = itSourceNode->get<std::size_t>();

      const auto itImagesNode = a_Doc.jSource.find( "images" );
      if( itImagesNode == a_Doc.jSource.cend() or not itImagesNode->is_array() or iImageIndex >= itImagesNode->size() )
         return {};

      const json &jImage = itImagesNode->at( iImageIndex );
      const auto itUri = jImage.find( "uri" );
      if( itUri == jImage.cend() or not itUri->is_string() )
         return {};

      const std::string sUri = itUri->get<std::string>();
      return ResolveUri( a_Doc.sSourceDirectory, sUri );
   }

   [[nodiscard]] auto ParseMaterials( const GltfDocument &a_Doc ) -> std::vector<MK::MaterialDesc>
   {
      const auto itMaterialsNode = a_Doc.jSource.find( "materials" );
      if( itMaterialsNode == a_Doc.jSource.cend() or not itMaterialsNode->is_array() )
         return {};

      std::vector<MK::MaterialDesc> aRetVec;
      aRetVec.reserve( itMaterialsNode->size() );

      for( const json &jMaterial : *itMaterialsNode )
      {
         if( const auto itMetalic = jMaterial.find( "pbrMetallicRoughness" ); itMetalic != jMaterial.cend() )
         {
            MK::MaterialDesc desc;
            bool bContainsAnything = false;

            // color
            if( const auto itColor = itMetalic->find( "baseColorFactor" )
               ; itColor != itMetalic->cend()
               and itColor->is_array()
               and itColor->size() >= 4 )
            {
               bContainsAnything = true;

               desc.v4Color = glm::vec4{
                  itColor->at( 0 ).get<float>(),
                  itColor->at( 1 ).get<float>(),
                  itColor->at( 2 ).get<float>(),
                  itColor->at( 3 ).get<float>()
               };
            }

            // texture
            if( const auto itTexture = itMetalic->find( "baseColorTexture" ); itTexture != itMetalic->cend() )
            {
               const auto itTextureIndex = itTexture->find( "index" );
               if( itTextureIndex == itTexture->cend() or not itTextureIndex->is_number_integer() )
                  goto Label_NoTextureFound;

               const int iIndex = itTextureIndex->get<int>();
               if( iIndex < 0 )
                  goto Label_NoTextureFound;

               auto sTexturePath = GetTexturePath( a_Doc, static_cast<std::size_t>( iIndex ) );
               if( sTexturePath.empty() )
                  goto Label_NoTextureFound;

               bContainsAnything = true;
               desc.sTexturePath = std::move( sTexturePath );
               
            }
            Label_NoTextureFound:

            if( not bContainsAnything )
               continue;

            aRetVec.push_back( std::move( desc ) );
         }
         
         // TODO: normalTexture
         // TODO: occlusionTexture
         // TODO: emissiveTexture
         // TODO: emissiveFactor
      }

      return aRetVec;
   }

   [[nodiscard]] constexpr
   auto GetComponentSize( const int a_iComponentType ) -> int
   {
      switch( const EComponentType eType = (EComponentType)a_iComponentType )
      {
      case EComponentType::Byte:
      case EComponentType::UnsignedByte:
         return 1;
      case EComponentType::Short:
      case EComponentType::UnsignedShort:
         return 2;
      case EComponentType::UnsignedInt:
      case EComponentType::Float:
         return 4;
      default:
         return 0;
      }
   }

   [[nodiscard]] auto GetAccessorDataPointer( const GltfDocument &a_Doc, const json &a_jAccessor ) -> const std::byte *
   {
      const int iBufferViewIndex = a_jAccessor.value( "bufferView", -1 );

      const auto itViewsNode = a_Doc.jSource.find( "bufferViews" );
      if( itViewsNode == a_Doc.jSource.cend() or not itViewsNode->is_array() or iBufferViewIndex >= itViewsNode->size() )
         return nullptr;

      const json jView = itViewsNode->at( iBufferViewIndex );

      const int iBufferIndex = jView.value( "buffer", -1 );
      if( iBufferIndex < 0 or iBufferIndex >= a_Doc.aBuffers.size() )
         return nullptr;

      const Buffer &aBuffer = a_Doc.aBuffers[iBufferIndex ];
      const std::size_t iByteOffset = jView.value( "byteOffset", 0 );
      const std::size_t iAccessorOffset = jView.value( "byteOffset", 0 );
      const std::size_t iTotalOffset = iByteOffset + iAccessorOffset;
      if( iTotalOffset >= aBuffer.size() )
         return nullptr;

      return aBuffer.data() + iTotalOffset;
   }

   [[nodiscard]] auto ReadComponentAsFloat( const std::byte *a_pData, const int a_iComponentType, const bool a_bNormalized ) -> float
   {
      switch( const EComponentType eType = (EComponentType)a_iComponentType )
      {
      case EComponentType::Byte:
      {
         const auto iValue = static_cast<std::int8_t>( a_pData[0] );
         return a_bNormalized ? static_cast<float>( iValue ) / 127.0f : static_cast<float>( iValue );
      }
      case EComponentType::UnsignedByte:
      {
         const auto iValue = a_pData[0];
         return a_bNormalized ? static_cast<float>( iValue ) / 255.0f : static_cast<float>( iValue );
      }
      case EComponentType::Short:
      {
         std::int16_t iValue = 0;
         std::memcpy( &iValue, a_pData, sizeof( iValue ) );
         return a_bNormalized ? static_cast<float>( iValue ) / 32767.0f : static_cast<float>( iValue );
      }
      case EComponentType::UnsignedShort:
      {
         std::uint16_t iValue = 0;
         std::memcpy( &iValue, a_pData, sizeof( iValue ) );
         return a_bNormalized ? static_cast<float>( iValue ) / 65535.0f : static_cast<float>( iValue );
      }
      case EComponentType::UnsignedInt:
      {
         std::uint32_t iValue = 0;
         std::memcpy( &iValue, a_pData, sizeof( iValue ) );
         return static_cast<float>( iValue );
      }
      case EComponentType::Float:
      {
         float fValue = 0.0f;
         std::memcpy( &fValue, a_pData, sizeof( fValue ) );
         return fValue;
      }
      default:
         return 0.0f;
      }
   }

   [[nodiscard]] auto ReadIndex( const std::byte *a_pData, const int a_iComponentType ) -> MK::Mesh::Index
   {
      switch( const EComponentType eType = static_cast<EComponentType>( a_iComponentType ) )
      {
      case EComponentType::UnsignedByte:
      {
         return static_cast<MK::Mesh::Index>( a_pData[0] );
      }
      case EComponentType::UnsignedShort:
      {
         std::uint16_t iValue = 0;
         std::memcpy( &iValue, a_pData, sizeof( iValue ) );
         return static_cast<MK::Mesh::Index>( iValue );
      }
      case EComponentType::UnsignedInt:
      {
         std::uint32_t iValue = 0;
         std::memcpy( &iValue, a_pData, sizeof( iValue ) );
         return static_cast<MK::Mesh::Index>( iValue );
      }
      default:
         return 0;
      }
   }

   [[nodiscard]] auto AccessorToIndices( const GltfDocument &a_Doc, const int a_iAccessorIndex ) -> std::vector<MK::Mesh::Index>
   {
      const auto itAccessorsNode = a_Doc.jSource.find( "accessors" );
      if( itAccessorsNode == a_Doc.jSource.cend() or not itAccessorsNode->is_array() )
         return {};

      if( a_iAccessorIndex < 0 or a_iAccessorIndex >= itAccessorsNode->size() )
         return {};

      const json &jAccessor = itAccessorsNode->at( a_iAccessorIndex );
      if( jAccessor.value( "type", "" ) != "SCALAR" )
         return {};

      const int iComponentType = jAccessor.value( "componentType", 0 );
      const int iComponentSize = GetComponentSize( iComponentType );
      if( iComponentSize == 0 )
         return {};

      const std::size_t iCount = jAccessor.value( "count", 0uz );
      const std::byte *pBase = GetAccessorDataPointer( a_Doc, jAccessor );
      if( not pBase )
         return {};

      int iBufferViewStride = iComponentSize;
      if( jAccessor.contains( "bufferView" ) )
      {
         const int iBufferViewIndex = jAccessor["bufferView"].get<int>();
         const json &jBufferView = a_Doc.jSource["bufferViews"][iBufferViewIndex];
         iBufferViewStride = jBufferView.value( "byteStride", iComponentSize );
      }

      std::vector<MK::Mesh::Index> aRetVec;
      aRetVec.reserve( iCount );

      for( std::size_t iIndex = 0; iIndex < iCount; ++iIndex )
      {
         const std::byte *pElement = pBase + iIndex * static_cast<std::size_t>( iBufferViewStride );
         aRetVec.emplace_back( ReadIndex( pElement, iComponentType ) );
      }

      return aRetVec;
   }

   template<int iVecLenght>
   [[nodiscard]] constexpr auto AccessorToVec( const GltfDocument& a_Doc, const int a_iAccessorIndex ) -> std::vector<glm::vec<iVecLenght, float>>
   {
      static_assert( iVecLenght >= 2 && iVecLenght <= 4, "AccessorToVec: supported vector lenghts are: 2, 3, 4" );
      static const std::string k_sVectorTypeName = "VEC" + std::to_string( iVecLenght );

      const auto itAccessorsNode = a_Doc.jSource.find( "accessors" );
      if( itAccessorsNode == a_Doc.jSource.cend() or not itAccessorsNode->is_array() )
         return {};

      if( a_iAccessorIndex < 0 or a_iAccessorIndex >= itAccessorsNode->size() )
         return {};
      
      const json &jAccessor = itAccessorsNode->at( a_iAccessorIndex );
      if( jAccessor.value( "type", "" ) != k_sVectorTypeName )
         return {};

      const int iComponentType = jAccessor.value( "componentType", 0 );
      const int iComponentSize = GetComponentSize( iComponentType );
      const int iStride = iComponentSize * iVecLenght;
      if( iComponentSize == 0 )
         return {};

      const std::size_t iCount = jAccessor.value( "count", 0uz );
      const bool bNormalized = jAccessor.value( "normalized", false );
      const std::byte *pBase = GetAccessorDataPointer( a_Doc, jAccessor );
      if( not pBase )
         return {};

      int iBufferViewStride = iStride;
      if( jAccessor.contains( "bufferView" ) )
      {
         const int iBufferViewIndex = jAccessor["bufferView"].get<int>();
         const json &oBufferView = a_Doc.jSource["bufferViews"][iBufferViewIndex];
         iBufferViewStride = oBufferView.value( "byteStride", iStride );
      }

      using vec = glm::vec<iVecLenght, float>;
      std::vector<vec> aRetVec;
      aRetVec.reserve( iCount );

      for( std::size_t iIndex = 0; iIndex < iCount; ++iIndex )
      {
         const std::byte *pElement = pBase + iIndex * static_cast<std::size_t>( iBufferViewStride );

         vec vValue;
         vValue.x = ReadComponentAsFloat( 0 * iComponentSize + pElement, iComponentType, bNormalized );
         vValue.y = ReadComponentAsFloat( 1 * iComponentSize + pElement, iComponentType, bNormalized );;
         if constexpr( iVecLenght == 3 )
            vValue.z = ReadComponentAsFloat( 2 * iComponentSize + pElement, iComponentType, bNormalized );;
         if constexpr( iVecLenght == 4 )
            vValue.w = ReadComponentAsFloat( 3 * iComponentSize + pElement, iComponentType, bNormalized );;

         aRetVec.push_back( std::move( vValue ) );
      }

      return aRetVec;
   }

   [[nodiscard]] auto ParseMeshes( const GltfDocument &a_Doc ) -> std::vector<MK::Mesh>
   {
      const auto itMeshesNode = a_Doc.jSource.find( "meshes" );
      if( itMeshesNode == a_Doc.jSource.cend() or not itMeshesNode->is_array() )
         return {};
      
      std::vector<MK::Mesh> aRetVec;
      for( const json &jMesh : *itMeshesNode )
      {
         const auto itPrimitives = jMesh.find( "primitives" );
         if( itPrimitives == jMesh.cend() or not itPrimitives->is_array() )
            continue;

         for( const json &jPrimitive : *itPrimitives )
         {
            const auto itAttributes = jPrimitive.find( "attributes" );
            if( itAttributes == jPrimitive.cend() or not itAttributes->is_object() )
               continue;

            std::vector<glm::vec3> aPositions;
            if( const int iPositionIndex = itAttributes->value( "POSITION", -1 ); iPositionIndex == -1 )
               continue;
            else
               aPositions = AccessorToVec<3>( a_Doc, iPositionIndex );
            
            std::vector<glm::vec3> aNormals;
            if( const int iNormalIndex = itAttributes->value( "NORMAL", -1 ); iNormalIndex > 0 )
            {
               aNormals = AccessorToVec<3>( a_Doc, iNormalIndex );
            }
            
            std::vector<glm::vec2> aTextureUVs;
            if( const int iTextureIndex = itAttributes->value( "TEXCOORD_0", -1 ); iTextureIndex > 0 )
            {
               aTextureUVs = AccessorToVec<2>( a_Doc, iTextureIndex );
            }
            
            std::vector<glm::vec3> aColors;
            if( const int iColorIndex = itAttributes->value( "COLOR_0", -1 ); iColorIndex > 0 )
            {
               aColors = AccessorToVec<3>( a_Doc, iColorIndex );
            }

            MK::Mesh mesh{
               .iMaterialIndex = jPrimitive.value( "material", -1 )
            };
            mesh.aVertices.reserve( aPositions.size() );

            for( auto iVertexIndex = 0uz; iVertexIndex < aPositions.size(); ++iVertexIndex )
            {
               MK::Vertex vertex{
                  .v3Position = aPositions[iVertexIndex],
                  .v3Normal =   iVertexIndex < aNormals.size() ? aNormals[iVertexIndex] : glm::vec3{},
                  .v2Texture =  iVertexIndex < aTextureUVs.size() ? aTextureUVs[iVertexIndex] : glm::vec2{},
                  .v3Color =    iVertexIndex < aColors.size() ? aColors[iVertexIndex] : glm::vec3{}
               };
               mesh.aVertices.push_back( std::move( vertex ) );
            }

            if( const int iIndicesIndex = jPrimitive.value( "indices", -1 ); iIndicesIndex >= 0 )
            {
               mesh.aIndices = AccessorToIndices( a_Doc, iIndicesIndex );
            }
            else
            {
               mesh.aIndices.reserve( aPositions.size() );
               for( auto i = 0uz; i < aPositions.size(); ++i )
                  mesh.aIndices[i] = static_cast<MK::Mesh::Index>( i );
            }

            aRetVec.push_back( std::move( mesh ) );
         }
      }
      return aRetVec;
   }

   [[nodiscard]] auto ParseNodes( const GltfDocument &a_Doc ) -> std::vector<MK::Node>
   {
      const auto itNodes = a_Doc.jSource.find( "nodes" );
      if( itNodes == a_Doc.jSource.cend() or not itNodes->is_array() or itNodes->empty() )
         return {};

      std::vector<MK::Node> aRetVec;
      aRetVec.resize( itNodes->size() );

      for( auto i = 0uz, n = itNodes->size(); i < n; ++i )
      {
         const json &jNode = itNodes->at( i );
         MK::Node node = aRetVec[i];

         if( const auto itTranslation = jNode.find( "translation" ); itTranslation != jNode.cend() && itTranslation->is_array() && itTranslation->size() == 3 )
         {
            node.v3Translation = glm::vec3{
               itTranslation->at( 0 ).get<float>(),
               itTranslation->at( 1 ).get<float>(),
               itTranslation->at( 2 ).get<float>()
            };
         }
         if( const auto itRotation = jNode.find( "rotation" ); itRotation != jNode.cend() && itRotation->is_array() && itRotation->size() == 4 )
         {
            node.qRotation = glm::quat{
               itRotation->at( 3 ).get<float>(),
               itRotation->at( 0 ).get<float>(),
               itRotation->at( 1 ).get<float>(),
               itRotation->at( 2 ).get<float>()
            };
         }
         if( const auto itScale = jNode.find( "scale" ); itScale != jNode.cend() && itScale->is_array() && itScale->size() == 3 )
         {
            node.v3Scale = glm::vec3{
               itScale->at( 0 ).get<float>(),
               itScale->at( 1 ).get<float>(),
               itScale->at( 3 ).get<float>()
            };
         }
         if( const int iMeshIndex = jNode.value( "mesh", -1 ); iMeshIndex >= 0 )
         {
            node.iMeshIndex = iMeshIndex;
         }
         if( const auto itChildren = jNode.find( "children" ); itChildren != jNode.cend() && itChildren->is_array() )
         {
            for( const json &jChild : *itChildren )
            {
               const int iChildIndex = jChild.get<int>();
               if( iChildIndex < 0 or iChildIndex >= aRetVec.size() )
                  continue;

               MKASSERT( aRetVec[iChildIndex].iParentIndex == -1 or aRetVec[iChildIndex].iParentIndex == i, "Overriding parent index" );
               aRetVec[iChildIndex].iParentIndex = i;
            }
         }
      }

      return aRetVec;
   }

   [[nodiscard]] auto ParseDocument( const GltfDocument &a_Doc ) -> std::expected<MK::Model, std::string>
   {
      MK::Model model;
      model.aMaterials = ParseMaterials( a_Doc );
      model.aMeshes = ParseMeshes( a_Doc );
      model.aNodes = ParseNodes( a_Doc );
      return model;
   }
}

namespace MK
{
   bool ParserGLTF::SupportsExtension( const std::string_view a_sExtension ) const
   {
      return a_sExtension == "gltf" || a_sExtension == "glb";
   }

   std::optional<Model> ParserGLTF::Parse( const std::string_view a_sPath ) const
   {
      const auto doc = LoadDocument( a_sPath );
      if( not doc )
      {
         MKASSERT( false, "Failed to load model file '{}' with message: {}", a_sPath, doc.error() );
         return std::nullopt;
      }

      auto model = ParseDocument( *doc );
      if( not model )
      {
         MKASSERT( false, "Failed to parse file '{}' with message: {}", a_sPath, model.error() );
         return std::nullopt;
      }

      return std::make_optional( std::move( model.value() ) ); // TODO: check nrvo
   }
}
