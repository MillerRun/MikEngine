#pragma once

#include <typeindex>

namespace MK::Event
{
   struct Token
   {
      std::type_index EventID;
      std::size_t iCallbackIndex;
   };
}
