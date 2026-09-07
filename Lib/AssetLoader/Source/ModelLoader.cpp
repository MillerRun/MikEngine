#include "ModelLoader.hpp"
#include "Parser/Parser.hpp"
#include "Parser/ParserGLTF.hpp"

#include <Utils/File.hpp>
#include <Utils/Assert.hpp>

namespace MK
{
   ModelLoader::ModelLoader()
   {
      this->RegisterParser( std::make_unique<ParserGLTF>() );
   }

   ModelLoader::~ModelLoader() = default;

   std::optional<Model> ModelLoader::Load( const std::string_view a_sFilePath ) const
   {
      const auto sExtension = MK::File::GetFileExtension( a_sFilePath );
      const IModelParser *pParser = this->PickParser( sExtension );
      if( not pParser )
      {
         MKASSERT( false, "Failed to find appropriate parser for extention '{}'", sExtension );
         return std::nullopt;
      }

      return pParser->Parse( a_sFilePath );
   }

   void ModelLoader::RegisterParser( std::unique_ptr<IModelParser> a_pParser )
   {
      m_aParsers.push_back( std::move( a_pParser ) );
   }

   IModelParser *ModelLoader::PickParser( const std::string_view a_sFileExtention ) const
   {
      for( const auto &pParser : m_aParsers )
         if( pParser->SupportsExtension( a_sFileExtention ) )
            return pParser.get();
      return nullptr;
   }
}