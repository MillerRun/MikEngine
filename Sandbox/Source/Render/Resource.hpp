#pragma once

namespace MK
{
   class Resource
   {
      Resource() = default;

      Resource( const Resource & ) = delete;
      void operator=( const Resource & ) = delete;

      Resource( Resource && ) noexcept = default;
      Resource& operator=( Resource && ) noexcept = default;
   };
}
