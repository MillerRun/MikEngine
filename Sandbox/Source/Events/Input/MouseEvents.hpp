#pragma once

#include "MouseTypes.hpp"

struct EventMousePressed
{
   MK::EMouseButton eCode{};
};

struct EventMouseReleased
{
   MK::EMouseButton eCode{};
};
