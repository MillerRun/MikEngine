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
   struct GltfDocument
   {
      std::string sSourceDirectory;
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

   [[nodiscard]] auto GetTexturePath( [[maybe_unused]] const GltfDocument &a_Doc, [[maybe_unused]] const std::size_t a_iTextureIndex ) -> std::string
   {
      return {};
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

   [[nodiscard]] auto ParseDocument( const GltfDocument &a_Doc ) -> std::expected<MK::Model, std::string>
   {
      MK::Model model;
      model.aMaterials = ParseMaterials( a_Doc );
      
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
