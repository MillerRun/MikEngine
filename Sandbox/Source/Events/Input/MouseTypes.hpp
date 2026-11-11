#pragma once

#include <cstddef>

namespace MK
{
  enum class EMouseButton : std::size_t
  {
      LEFT
    , RIGHT
    , MIDDLE
    , BUTTON_4
    , BUTTON_5
    , BUTTON_6
    , BUTTON_7
    , BUTTON_8

    , MAX_OR_UNKNOWN // invalid
  };
}
