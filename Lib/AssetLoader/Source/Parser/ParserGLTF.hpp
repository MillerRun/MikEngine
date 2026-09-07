#pragma once

#include "Parser.hpp"

namespace MK
{
   class ParserGLTF final : public IModelParser
   {
   public:
      [[nodiscard]] bool SupportsExtension( const std::string_view a_sExtension ) const override;
      [[nodiscard]] std::optional<Model> Parse( const std::string_view a_sPath ) const override;
   };
}
