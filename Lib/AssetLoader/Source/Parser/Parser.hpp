#pragma once

#include <optional>
#include <string_view>

namespace MK
{
   struct Model;

   class IModelParser
   {
   public:
      virtual ~IModelParser() = default;

      [[nodiscard]] virtual bool SupportsExtension( const std::string_view a_sExtension ) const = 0;
      [[nodiscard]] virtual std::optional<Model> Parse( const std::string_view a_sPath ) const = 0;
   };
}
