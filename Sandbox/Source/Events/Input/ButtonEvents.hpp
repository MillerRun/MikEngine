#pragma once

#include "ButtonTypes.hpp"

struct EventButtonPressed
{
   MK::EButton eCode{};
};

struct EventButtonReleased
{
   MK::EButton eCode{};
};
