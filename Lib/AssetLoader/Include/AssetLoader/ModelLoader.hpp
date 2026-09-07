#pragma once

#include <optional>
#include <vector>
#include <memory>
#include <string_view>

#include <Utils/Singleton.hpp>

#include "CommonTypes.hpp"

namespace MK
{
   class IModelParser;

   class ModelLoader final : public MK::Utils::ISingleton<ModelLoader>
   {
      friend class MK::Utils::ISingleton<ModelLoader>;

   protected:
      ModelLoader();
      ~ModelLoader();

   public:
      [[nodiscard]] std::optional<Model> Load( const std::string_view a_sFilePath ) const;

   private:
      void RegisterParser( std::unique_ptr<IModelParser> a_pParser );
      IModelParser *PickParser( const std::string_view a_sFileExtention ) const;

   private:
      std::vector<std::unique_ptr<IModelParser>> m_aParsers;
   };
}
